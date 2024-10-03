#include "ObjectManager.h"
#include "Player.h"
#include "Camera.h"
#include "game.h"
#include "TransparentBlockChip.h"
#include "BossSpawnFlagChip.h"
#include "Pause.h"
#include "MainScreen.h"
#include "Sound.h"
#include "MessageWindow.h"
#include "Controller.h"
#include "GameOver.h"

#include <cassert>
#include <string>
#include <filesystem>
#include <DxLib.h>



namespace
{
	// マップデータファイルパス
	const std::string kFieldDataFilePath = "Data/MapData/mapData_";

	// ファイルパス拡張子
	const std::string kFilePathExtension = ".fmf";
}


namespace
{
	// 画像データ
	struct GraphData
	{
		// グラフィックファイルパス
		const char* const kFilePath;
		// 分割数
		const EvoLib::Load::DivNum kDivNum;
	};
}


namespace PlayerGraph
{
	// プレイヤーグラフィックデータ
	const GraphData kPlayerGraphData[] =
	{
		{ "Data/Graphic/Character/GameMain/Marisa.png", EvoLib::Load::DivNum(3, 4) },
	};
}

namespace TalkData
{
	const std::string kDataFilePath = "Data/Csv/Talk/TalkData_1.csv";
}

namespace ScoreGraph_GameMain
{
	// 番号グラフィックのファイル名
	const char* kNumberGraphFileName = "Data/Graphic/Number/Number.png";
	// スコアグラフィックの分割数
	const EvoLib::Load::DivNum kNumberGraphDivNum = EvoLib::Load::DivNum(10, 1);

	// 仕切りグラフィックのファイル名
	const char* const kSeparateGraphFileName = "Data/Graphic/Number/Separate.png";
	// カウントグラフィックのファイル名
	const char* const kCountGraphFileName = "Data/Graphic/Number/Count.png";

	// クリアリトライ回数テキストグラフィックのファイル名
	const char* const kClearRetryTextGraphFileName = "Data/Graphic/GameMain/CountText.png";


	// タイムスコアの位置
	const Vec2 kTimeScorePos = Vec2(150.0f, 20.0f);
	// デスカウントの位置
	const Vec2 kDeathCountPos = Vec2(kTimeScorePos.x, kTimeScorePos.y + 30.0f);

	// グラフィック同士の間隔
	const float kTextGraphInterval = -80.0f;

	// クリアリトライ回数テキストの位置
	const Vec2 kClearTextPos = Vec2(kTimeScorePos.x + kTextGraphInterval, kTimeScorePos.y);
	const Vec2 kRetryTextPos = Vec2(kDeathCountPos.x + kTextGraphInterval, kDeathCountPos.y);


	// テキストグラフィックサイズ
	constexpr double kTextGraphSize = 0.4;

	// グラフィック同士の間隔
	static float kGraphInterval = 0.0f;
	// グラフィック同士の間隔の設定
	const float kGraphIntervalSetting = -15.0f;

	// グラフィックサイズ
	constexpr double kGraphSize = 0.4;
}

ObjectManager::ObjectManager() :
	m_object(),
	m_mapInfoData(),
	m_screenCircle(),
	m_testMapGraph(),
	m_pStateMachine(),
	m_playerGraphHandle(),
	m_scoreGraphData(),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>()),
	m_pCamera(std::make_shared<Camera>()),
	m_pPause(std::make_shared<Pause>()),
	m_pOpeningMessageWindow(std::make_shared<MessageWindow>()),
	m_pEndTalkMessageWindow(std::make_shared<MessageWindow>()),
	m_pGameOver(std::make_shared<GameOver>())
{
}

ObjectManager::~ObjectManager()
{
	for (auto& graph : m_testMapGraph)
	{
		DeleteGraph(graph);
	}

	for (auto& graph : m_playerGraphHandle)
	{
		DeleteGraph(graph);
	}
	m_mapInfoData.mapCollisionData.clear();

	m_object.clear();

	m_pPlatinumLoader.reset();

	m_pCamera.reset();

	m_pPause.reset();

	m_pOpeningMessageWindow.reset();

	m_pEndTalkMessageWindow.reset();

	m_pGameOver.reset();

	m_pMainScreen.reset();

	DeleteGraph(m_scoreGraphData.countGraphHandle);
	DeleteGraph(m_scoreGraphData.separateGraphHandle);
	for(auto& graph : m_scoreGraphData.clearRetryTextGraphHandle)
	{
		DeleteGraph(graph);
	}
}

void ObjectManager::Init()
{
	// ステート初期化
	StateInit();
}

void ObjectManager::Update()
{
	// カメラ更新
	m_pCamera->OffsetCalculation(GameData::GetInstance()->GetPlayerPos());

	// ポーズクラス更新
	m_pPause->Update();

	// ステートマシンの更新
	m_pStateMachine.Update();

}

void ObjectManager::Draw()
{
	// オブジェクト描画
	ObjectDraw();
	
	// マップ描画
	TestMapDraw();

	// ステートマシンの描画
	m_pStateMachine.Draw();

	// セーブスコア描画
	DrawSaveScore();
}

void ObjectManager::ChangeScene(const SceneMain::Scene& nextScene)
{
	// シーン変更
	m_pMainScreen->ChangeScene(nextScene);
}

void ObjectManager::StateInit()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};


	// 設定ステート
	{
		auto enter = [this]() { StateSettingInit(); };
		
		m_pStateMachine.AddState(State::Setting, enter, dummy, dummy, dummy);
	}

	// オープニングステート
	{
		auto enter = [this]() { StateOpeningEnter(); };
		auto update = [this]() { StateOpeningUpdate(); };
		auto draw = [this]() { StateOpeningDraw(); };
		auto exit = [this]() { StateOpeningExit(); };
		

		m_pStateMachine.AddState(State::Opening, enter, update, draw, exit);
	}

	// 通常ステート
	{
		auto update = [this]() { StateNormalUpdate(); };
		auto draw = [this]() { StateNormalDraw(); };
		

		m_pStateMachine.AddState(State::Normal, dummy, update, draw, dummy);
	}

	// 終了会話ステート
	{
		auto enter = [this]() { StateEndTalkEnter(); };
		auto update = [this]() { StateEndTalkUpdate(); };
		auto draw = [this]() { StateEndTalkDraw(); };
		auto exit = [this]() { StateEndTalkExit(); };
		
		m_pStateMachine.AddState(State::EndTalk, enter, update, draw, exit);
	}

	// ポーズステート
	{	
		auto update = [this]() { StatePauseUpdate(); };
		auto draw = [this]() { StatePauseDraw(); };
		

		m_pStateMachine.AddState(State::Pause, dummy, update, draw, dummy);
	}

	// エンディングステート
	{
		auto enter = [this]() { StateEndingEnter(); };


		m_pStateMachine.AddState(State::Ending, enter, dummy, dummy, dummy);
	}

	// 初期ステートを設定
	SetState(State::Setting);
}

void ObjectManager::StateSettingInit()
{
	// ステート
	State state = State::Opening;

	// ゲームデータが初期化されていなければ、ノーマルステートに変更
	if (!GameData::GetInstance()->GetSaveData().isInit)
	{
		state = State::Normal;
	}



	// ロード
	Load();

	// マップ関連初期設定
	InitMap();

	// スクリーンサークル初期化
	InitScreenCircle();


	// カメラクラスにオブジェクトファクトリークラスポインタを送る
	m_pCamera->SetObjectFactoryPointer(shared_from_this());

	// ポーズクラスにオブジェクトファクトリークラスポインタを送る
	m_pPause->SetObjectFactoryPointer(shared_from_this());

	// ポーズクラス初期化
	m_pPause->Init();

	// ゲームオーバークラス初期化
	m_pGameOver->Init();

	// マップグラフィック代入
	{
		EvoLib::Load::DivNum divNum = EvoLib::Load::DivNum(16, 16);

		m_testMapGraph = EvoLib::Load::LoadDivGraph_EvoLib_Revision("Data/MapData/mapSetting.png", divNum);
	}


	// メッセージウィンドウデータロード
	m_pEndTalkMessageWindow->LoadData(TalkData::kDataFilePath);

	// BGM再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::BGM)]);

	// ステートを変更
	SetState(state);
}

void ObjectManager::StateOpeningEnter()
{
	// メッセージウィンドウデータロード
	m_pOpeningMessageWindow->LoadData(TalkData::kDataFilePath);
}

void ObjectManager::StateOpeningUpdate()
{

	// オブジェクト更新
	ObjectUpdate(true);

	// スクリーン内かどうかを調べる
	FieldCheck();

	// メッセージウィンドウ更新
	m_pOpeningMessageWindow->Update();

	// メッセージウィンドウが終了したら次のステートに変更
	if (m_pOpeningMessageWindow->IsAllTextEnd())
	{
		SetState(State::Normal);
	}
}

void ObjectManager::StateOpeningDraw()
{
	// メッセージウィンドウ描画
	m_pOpeningMessageWindow->Draw();
}

void ObjectManager::StateOpeningExit()
{
	// メッセージウィンドウ初期化
	m_pOpeningMessageWindow->InitData();
}

void ObjectManager::StateNormalUpdate()
{
	// オブジェクト更新
	ObjectUpdate();

	// スクリーン内かどうかを調べる
	FieldCheck();

	// オブジェクト削除
	ObjectErase();
	
	// 時間計測
	GameData::GetInstance()->TimeCount();
}

void ObjectManager::StateNormalDraw()
{
	// ゲームオーバークラス描画
	m_pGameOver->Draw();
}

void ObjectManager::StateEndTalkEnter()
{

	//m_pEndTalkMessageWindow->ResetData();
}

void ObjectManager::StateEndTalkUpdate()
{
	// スクリーン内かどうかを調べる
	FieldCheck();

	// カメラ更新
	m_pCamera->OffsetCalculation(GameData::GetInstance()->GetPlayerPos());

	// メッセージウィンドウ更新
	m_pEndTalkMessageWindow->Update();

	// メッセージウィンドウが終了したら次のステートに変更
	if (m_pEndTalkMessageWindow->IsAllTextEnd())
	{
		SetState(State::Ending);
	}
}

void ObjectManager::StateEndTalkDraw()
{
	// メッセージウィンドウ描画
	m_pEndTalkMessageWindow->Draw();
}

void ObjectManager::StateEndTalkExit()
{
}

void ObjectManager::StatePauseUpdate()
{
}

void ObjectManager::StatePauseDraw()
{
	// ポーズクラス描画
	m_pPause->Draw();
}

void ObjectManager::StateEndingEnter()
{
	// エンディングシーンに変更
	m_pMainScreen->ChangeScene(SceneMain::Scene::Ending);
}

void ObjectManager::SetSavePoint(const Cell& cell, const GameData::PlayerStatus& playerStatus)
{
	// セーブポイントデータを設定
	GameData::GetInstance()->SetSavePointData(m_mapInfoData.mapNumber, cell, playerStatus);
}

void ObjectManager::SetState(const State& state)
{
	// ステートを設定
	m_pStateMachine.SetState(state);
}

void ObjectManager::Load()
{
	// プレイヤーグラフィックロード
	{
		for (const auto& graphData : PlayerGraph::kPlayerGraphData)
		{
			m_playerGraphHandle = 
				(EvoLib::Load::LoadDivGraph_EvoLib_Revision(graphData.kFilePath, graphData.kDivNum));
		}
	}

	// サウンドロード
	{
		Sound::GetInstance()->Load(kSoundFileName,false);
	}

	// スコアグラフィックロード
	{
		m_scoreGraphData.numberGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph_GameMain::kNumberGraphFileName, ScoreGraph_GameMain::kNumberGraphDivNum);

		m_scoreGraphData.separateGraphHandle = LoadGraph(ScoreGraph_GameMain::kSeparateGraphFileName);
		m_scoreGraphData.countGraphHandle = LoadGraph(ScoreGraph_GameMain::kCountGraphFileName);

		// クリアリトライ回数テキストグラフィックロード
		m_scoreGraphData.clearRetryTextGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph_GameMain::kClearRetryTextGraphFileName, EvoLib::Load::DivNum(1, 2));

		{
			// グラフィックのサイズを取得
			const Vec2 size = EvoLib::Calculation::GetGraphSize_EvoLib(m_scoreGraphData.numberGraphHandle, ScoreGraph_GameMain::kGraphSize);

			// グラフィックの間隔を設定
			ScoreGraph_GameMain::kGraphInterval = size.x + ScoreGraph_GameMain::kGraphIntervalSetting;
		}
	}
}

void ObjectManager::PlayerCreate(const Vec2& pos)
{
	// プレイヤー生成
	m_object.push_back(std::make_shared<Player>());

	// ポインタを送る
	m_object.back()->SetObjectFactory(shared_from_this());

	// 描画ランクを代入
	m_object.back()->SetDrawRank(ObjectBase::DrawRank::Rank_1);

	// 座標を代入
	m_object.back()->SetPos(pos);

	// グラフィックハンドルを代入
	m_object.back()->SetGraphicHandle(m_playerGraphHandle);

	// 初期化処理
	m_object.back()->Init();
}

void ObjectManager::MapCollisionDataCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType)
{
	// マップデータ
	std::vector<std::vector<MapCollisionData>>mapCollisionData(
		m_mapInfoData.mapChip.mapWidth, 
		std::vector<MapCollisionData>(m_mapInfoData.mapChip.mapHeight));

	// 次のステージセル
	Cell nextStageCell = Cell();

	// 前のステージセル
	Cell previouseStageCell = Cell();

	// プレイヤーが生成されたかどうか
	bool isPlayerCreate = false;

	// スポーンチップ個数
	int spawnChip = 0;

	// 次のステージ座標チップ個数
	int nextPosChip = 0;

	// 前のステージ座標チップ個数
	int previousePosChip = 0;

	// ボススポーンフラグチップ個数
	int bossSpawnFragChip = 0;

	// ボススポーン座標チップ個数
	int bossSpawnPosChip = 0;



	// すべてのセルを見る
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// マップチップタイプに変換
			const ChipType mapChipType = ChipType(mapData[x][y]);

			// チップの数をカウントする
			{
				switch (mapChipType)
				{
				case ObjectManager::ChipType::NextStage:

				
					// 次のステージ座標セルを代入
					nextStageCell = Cell(x, y);
					break;
				case ObjectManager::ChipType::PreviouseStage:

					// 前のステージ座標セルを代入
					previouseStageCell = Cell(x, y);
					break;
				case ObjectManager::ChipType::NextPos:

					// 次のステージ座標チップ個数を増やす
					nextPosChip++;
					break;
				case ObjectManager::ChipType::PreviousePos:

					// 前のステージ座標チップ個数を増やす
					previousePosChip++;
					break;
				case ObjectManager::ChipType::SpawnPos:

					// スポーンチップ個数を増やす
					spawnChip++;
					break;
				default:
					break;
				}
			}


			// マップ判定データ設定
			{
				// チップタイプを入れる
				mapCollisionData[x][y].chipType = ChipType(mapData[x][y]);

				// 四角形情報計算
				mapCollisionData[x][y].square.A = Vec2((m_mapInfoData.mapChip.chipSize * x), (m_mapInfoData.mapChip.chipSize * y));
				mapCollisionData[x][y].square.B = Vec2((mapCollisionData[x][y].square.A.x + m_mapInfoData.mapChip.chipSize), mapCollisionData[x][y].square.A.y);
				mapCollisionData[x][y].square.C = Vec2(mapCollisionData[x][y].square.B.x, (mapCollisionData[x][y].square.B.y + m_mapInfoData.mapChip.chipSize));
				mapCollisionData[x][y].square.D = Vec2(mapCollisionData[x][y].square.A.x, mapCollisionData[x][y].square.C.y);

				// 円情報計算
				mapCollisionData[x][y].circle =
					EvoLib::Convert::SquareToCircle(mapCollisionData[x][y].square);
			}

			// プレイヤー生成
			{
				// スポーンするかどうか
				const bool isSpwan =
					mapChipType == ChipType::SpawnPos &&
					mapSwitchType == MapSwitchType::Spawn;

				// 次のステージ座標に移動するかどうか
				const bool isNextStagePos =
					mapChipType == ChipType::NextPos &&
					mapSwitchType == MapSwitchType::NextStage;

				// 前のステージ座標に移動するかどうか
				const bool isPreviouseStagePos =
					mapChipType == ChipType::PreviousePos &&
					mapSwitchType == MapSwitchType::PreviouseStage;

				// スポーン
				if (isSpwan)
				{
					// セーブ情報を送る
					GameData::GetInstance()->SetSaveData(GameData::SaveData(false, Time(), m_mapInfoData.mapNumber, Cell(x, y)));
				}

				// プレイヤー生成
				if (isSpwan ||
					isNextStagePos ||
					isPreviouseStagePos)
				{
					// キャラクター生成
					PlayerCreate(EvoLib::Convert::CellToPos(Cell(x, y), m_mapInfoData.mapChip.chipSize));

					// プレイヤー生成フラグ
					isPlayerCreate = true;
				}
			}

			// マップチップ生成
			{
				// チップ生成フラグ
				const bool isCreateChip =
					mapCollisionData[x][y].chipType == ChipType::TransparentBlock;

				// チップ生成
				if (isCreateChip)
				{
					// マップチップ生成
					MapChipCreate(mapCollisionData[x][y]);
				}
			}
		}
	}

	if (mapSwitchType == MapSwitchType::SaveData)
	{
		// キャラクター生成
		PlayerCreate(EvoLib::Convert::CellToPos(GameData::GetInstance()->GetSaveData().cell, m_mapInfoData.mapChip.chipSize));

		// プレイヤー生成フラグ
		isPlayerCreate = true;
	}


	// リスポーン
	if (mapSwitchType == MapSwitchType::Respawn)
	{
		// キャラクター生成
		PlayerCreate(EvoLib::Convert::CellToPos(GameData::GetInstance()->GetSaveData().cell, m_mapInfoData.mapChip.chipSize));

		// プレイヤー生成フラグ
		isPlayerCreate = true;
	}

	// プレイヤーが生成されていなかった場合、次のステージか前のステージに移動する
	if (!isPlayerCreate)
	{
		// ステージ変更タイプが次のステージの場合、前のステージセル座標を代入してキャラクター生成
		if (mapSwitchType == MapSwitchType::NextStage)
		{
			// キャラクター生成
			PlayerCreate(EvoLib::Convert::CellToPos(previouseStageCell, m_mapInfoData.mapChip.chipSize));
		}

		// ステージ変更タイプが前のステージの場合、次のステージセル座標を代入してキャラクター生成
		if (mapSwitchType == MapSwitchType::PreviouseStage)
		{
			// キャラクター生成
			PlayerCreate(EvoLib::Convert::CellToPos(nextStageCell, m_mapInfoData.mapChip.chipSize));
		}
	}


	// ステージのマップデータを代入する
	m_mapInfoData.mapCollisionData = mapCollisionData;


	// エラーメッセージ処理
	{
		// チップ数が多かった場合のエラーメッセージ
		const std::string overChip = "が多すぎるようです";

		// チップ数が無い場合のエラーメッセージ
		const std::string noChip = "が足りないようです";


		// チップ名
		std::string chipName = "";


		// 現在のfmfファイル名
		std::string fmfFileName = "[ " + m_mapInfoData.filePath[m_mapInfoData.mapNumber] + " ]の";


		// エラーメッセージ
		std::string errorMessage = "";

		// エラーメッセージフラグ
		bool isErrorMessage = false;


		// スポーン
		if (mapSwitchType == MapSwitchType::Spawn)
		{
			// チップ名をSpawnChipにする
			chipName = "[ SpawnChip ]";

			if (spawnChip <= 0)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + noChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
			else if (spawnChip > 1)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + overChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
		}

		// 次のステージ
		if (mapSwitchType == MapSwitchType::NextStage)
		{
			// チップ名をNextPosChipにする
			chipName = "[ NextPosChip ]";

			if (nextPosChip <= 0)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + noChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
			else if (nextPosChip > 1)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + overChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
		}

		// 前のステージ
		if (mapSwitchType == MapSwitchType::PreviouseStage)
		{
			// チップ名をPreviousePosChipにする
			chipName = "[ PreviousePosChip ]";

			if (previousePosChip <= 0)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + noChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
			else if (previousePosChip > 1)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + overChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
		}


	
		if (bossSpawnFragChip > 0)
		{
			if(bossSpawnPosChip <=0)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + noChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
			else if (bossSpawnPosChip > 1)
			{
				// エラーメッセージ生成
				errorMessage = fmfFileName + chipName + overChip;

				// エラーメッセージフラグ
				isErrorMessage = true;
			}
		}



		// エラーメッセージを出すかどうか
		if (isErrorMessage)
		{
			// エラーメッセージを出す
			EvoLib::Assert::ErrorMessageBox(errorMessage);
		}
	}
}

void ObjectManager::ObjectErase()
{	
	// すべてのオブジェクトを見て、削除する
	auto rmIt = std::remove_if
	(m_object.begin(), m_object.end(), [](const std::shared_ptr<ObjectBase>& object)
		{
			return !object->GetIsExlist();
		}
	);

	// 削除
	m_object.erase(rmIt, m_object.end());
}

void ObjectManager::StageMove(const MapSwitchType& mapSwitchType)
{
	// 次のステージに移動するかどうかのフラグからステージナンバーを増やすか決める
	if (mapSwitchType == MapSwitchType::NextStage)
	{
		// ステージナンバーの数字を増やす
		m_mapInfoData.mapNumber++;
	}
	else if(mapSwitchType == MapSwitchType::PreviouseStage)
	{
		// ステージナンバーの数字を減らす;
 		m_mapInfoData.mapNumber--;
	}
	else if (mapSwitchType == MapSwitchType::Respawn)
	{
		// セーブポイントデータのステージナンバーをステージナンバーの数字に代入
		m_mapInfoData.mapNumber = GameData::GetInstance()->GetSaveData().stageNumber;
	}


	// 要素削除
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// プラチナムデータロード
	m_pPlatinumLoader->Load(m_mapInfoData.filePath[m_mapInfoData.mapNumber].c_str());
	
	// プラチナムデータ取得
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();

	// マップのレイヤー
	const int mapLayer = 0;
	
	// マップ情報取得
	m_mapInfoData.mapChip = m_pPlatinumLoader->GetMapChip();

	// マップ生成
	MapCollisionDataCreate(mapData[mapLayer].mapData, mapSwitchType);
}


std::vector<std::vector<int>> ObjectManager::GetMapChipNumber()
{
	// 二次元の要素数を持った可変長配列
	std::vector<std::vector<int>>mapChipNumber(m_mapInfoData.mapChip.mapWidth, std::vector<int>(m_mapInfoData.mapChip.mapHeight));

	
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			mapChipNumber[x][y] = static_cast<int>(m_mapInfoData.mapCollisionData[x][y].chipType);
		}
	}

	return mapChipNumber;
}

std::tuple<bool, Vec2>  ObjectManager::GetSavePointPos()
{
	// セーブポイントデータを取得
	const GameData::SaveData savePointData = GameData::GetInstance()->GetSaveData();

	// ボスを削除する
	for (auto& object : m_object)
	{
		if (object->GetObjectID() == ObjectBase::ObjectID::BossEnemy)
		{
			object->SetIsExlist(false);
		}
	}

	// セーブポイントデータのステージナンバーと現在のステージナンバーが異なる場合、マップ生成を行う
	if (savePointData.stageNumber != m_mapInfoData.mapNumber)
	{
		// マップ移動処理
		StageMove(MapSwitchType::Respawn);

		return std::tuple<bool, Vec2>(true, Vec2());
	}


	// ギミックリセット
	GimmickReset();

	
	// セルから変換した座標を返す
	return std::tuple<bool, Vec2>(false, EvoLib::Convert::CellToPos(savePointData.cell, m_mapInfoData.mapChip.chipSize));
}

void ObjectManager::InitMap()
{
	// マップデータ初期設定
	InitMapDataFilePath();

	// マップナンバーを初期化
	m_mapInfoData.mapNumber = 0;

	// ステージ移動タイプを初期化
	MapSwitchType mapSwitchType = MapSwitchType::Spawn;

	

	// セーブデータが初期化されていない場合
	if(!GameData::GetInstance()->GetSaveData().isInit)
	{
		// ステージ移動タイプをセーブデータにする
		mapSwitchType = MapSwitchType::SaveData;

		// ステージナンバーをセーブデータにある数値にする
		m_mapInfoData.mapNumber = GameData::GetInstance()->GetSaveData().stageNumber;
	}

	// マップ生成
	StageMove(mapSwitchType);
}

void ObjectManager::InitScreenCircle()
{
	// 画面の四角形情報
	Square screenSquare = Square();

	// 画面の四角形情報を代入
	screenSquare.A = Vec2(0, 0);
	screenSquare.B = Vec2(Game::kScreenWidth, 0);
	screenSquare.C = Vec2(Game::kScreenWidth, Game::kScreenHeight);
	screenSquare.D = Vec2(0, Game::kScreenHeight);

	// 画面の中心点および、画面からの各頂点距離の最大値を半径とした円の情報を返す
	m_screenCircle =
		EvoLib::Convert::SquareToCircle(screenSquare);
}

void ObjectManager::InitMapDataFilePath()
{
	// 処理数カウント
	int processCount = 0;

	while (true)
	{
		// 処理数カウントをstring型に変換
		std::string numberStr = std::to_string(processCount);

		// ファイルパスを作成
		std::string filePath =
			kFieldDataFilePath +
			numberStr +
			kFilePathExtension;


		// そのファイルパスが存在するかどうかを調べる
		if (!std::filesystem::is_regular_file(filePath))
		{
			break;
		}

		// ファイルパス情報を追加する
		m_mapInfoData.filePath.push_back(filePath);


		// 処理カウントを増やす
		processCount++;
	}	
}

void ObjectManager::MapChipCreate(const MapCollisionData& mapCollisionData)
{
	if (mapCollisionData.chipType == ChipType::TransparentBlock)
	{
		// マップチップ生成
		m_object.push_back(std::make_shared<TransparentBlockChip>());
	}

	// 描画ランクを代入
	m_object.back()->SetDrawRank(ObjectBase::DrawRank::Rank_2);

	// ポインタを送る
	m_object.back()->SetObjectFactory(shared_from_this());

	// 座標を代入
	m_object.back()->SetSquare(mapCollisionData.square);

	// 初期化処理
	m_object.back()->Init();
}

void ObjectManager::GimmickReset()
{
	// ギミックフラグをリセット
	for (auto& object : m_object)
	{
		object->SetGimiickFlag(false);
	}
}

bool ObjectManager::IsCellCheckOutOfRange(const Cell& cell)
{
	if (cell.x < 0)return true;
	if (cell.x >= m_mapInfoData.mapChip.mapWidth)return true;
	if (cell.y < 0)return true;
	if (cell.y >= m_mapInfoData.mapChip.mapHeight)return true;

	return false;
}

void ObjectManager::ObjectUpdate(const bool isStopPlayer)
{

	for(auto& object : m_object)
	{
		// プレイヤーの更新を止める
		if (isStopPlayer && object->GetObjectID() == ObjectBase::ObjectID::Player)
		{
			continue;
		}
		object->Update();
	}
}

void ObjectManager::ObjectDraw()
{
	// 描画ランクの逆順にオブジェクトを描画するループ
	for (int i = static_cast<int>(ObjectBase::DrawRank::RankNum) - 1; i >= 0; --i) {
		// 描画ランク
		const ObjectBase::DrawRank drawRank = static_cast<ObjectBase::DrawRank>(i);

		// 条件を満たすすべてのオブジェクトを描画する
		std::for_each(m_object.begin(), m_object.end(),
			[drawRank](const std::shared_ptr<ObjectBase>& object) {
				if (object->GetDrawRank() == drawRank) {
					object->Draw();
				}
			});
	}
}

Triangle ObjectManager::ChipTypeToTriangle(const ChipType& needleDirection, const Square& square)
{
	// 左上
	const Vec2 leftTopPos = square.A;
	// 上
	const Vec2 topPos = Vec2(leftTopPos.x + (m_mapInfoData.mapChip.chipSize * 0.5f), leftTopPos.y);
	// 右上
	const Vec2 rightTopPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y);
	// 右
	const Vec2 rightPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y + (m_mapInfoData.mapChip.chipSize * 0.5f));
	// 右下
	const Vec2 rightBottomPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// 下
	const Vec2 bottomPos = Vec2(leftTopPos.x + (m_mapInfoData.mapChip.chipSize * 0.5f), leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// 左下
	const Vec2 leftBottomPos = Vec2(leftTopPos.x, leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// 左
	const Vec2 leftPos = Vec2(leftTopPos.x, leftTopPos.y + (m_mapInfoData.mapChip.chipSize * 0.5f));


	// 三角形情報
	Triangle triangle = Triangle();

	if (needleDirection == ChipType::TopNeedle)
	{
		// 上
		triangle.pos_1 = topPos;
		// 左下
		triangle.pos_2 = leftBottomPos;
		// 右下
		triangle.pos_3 = rightBottomPos;
	}

	if (needleDirection == ChipType::BottomNeedle)
	{
		// 下
		triangle.pos_1 = bottomPos;
		// 左上
		triangle.pos_2 = leftTopPos;
		// 右上
		triangle.pos_3 = rightTopPos;
	}

	if (needleDirection == ChipType::LeftNeedle)
	{
		// 左
		triangle.pos_1 = leftPos;
		// 右上
		triangle.pos_2 = rightTopPos;
		// 右下
		triangle.pos_3 = rightBottomPos;
	}

	if (needleDirection == ChipType::RightNeedle)
	{
		// 右
		triangle.pos_1 = rightPos;
		// 左上
		triangle.pos_2 = leftTopPos;
		// 左下
		triangle.pos_3 = leftBottomPos;
	}

	return triangle;
}

void ObjectManager::FieldCheck()
{
	// 判定用の円情報
	Circle collisionMapCircle = Circle();

	// プレイヤーの座標を取得
	Vec2 playerPos = GameData::GetInstance()->GetPlayerPos();
	playerPos += GameData::GetInstance()->GetCameraPos();

	// プレイヤーの判定円情報
	const Circle playerCircle = Circle(playerPos, Player::kCollisionRadius);

	// マップの範囲内であるかどうかを調べる
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{

			// 円情報をコピー
			collisionMapCircle = m_mapInfoData.mapCollisionData[x][y].circle;

			// 座標にオフセット値を加える
			collisionMapCircle.centerPos += GameData::GetInstance()->GetCameraPos();


			// スクリーン外であるかどうかを調べる
			{
				// 円の判定
				m_mapInfoData.mapCollisionData[x][y].screenFlag = EvoLib::Collision::IsCircleToCircle(collisionMapCircle, m_screenCircle);
			}

			// プレイヤーの範囲内であるかどうかを調べる
			{
				// 円の判定
				m_mapInfoData.mapCollisionData[x][y].playerRangeFlag = EvoLib::Collision::IsCircleToCircle(collisionMapCircle, playerCircle);
			}
		
		}
	}
}

void ObjectManager::TestMapDraw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,200);

	Vec2 pos1 = Vec2();
	Vec2 pos2 = Vec2();


	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// 画面内でない場合は描画しない
			if(!m_mapInfoData.mapCollisionData[x][y].screenFlag)
			{
				continue;
			}
			
			// 座標の代入
			pos1 = Vec2(x * m_mapInfoData.mapChip.chipSize, y * m_mapInfoData.mapChip.chipSize);
			pos2 = Vec2(pos1.x + m_mapInfoData.mapChip.chipSize, pos1.y + m_mapInfoData.mapChip.chipSize);

			// カメラ座標を足す
			pos1 += GameData::GetInstance()->GetCameraPos();
			pos2 += GameData::GetInstance()->GetCameraPos();


			int color = 0x00ff00;

			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::Ground)
			{
				color = 0xff0000;
			}
			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::Save)
			{
				color = 0xffffff;
			}

			
			// 数字の座標
			const Vec2 stringPos = Vec2(pos1.x + m_mapInfoData.mapChip.chipSize * 0.5f, pos1.y + m_mapInfoData.mapChip.chipSize * 0.5f);
			// 数字描画
			DrawFormatString(static_cast<int>(stringPos.x), static_cast<int>(stringPos.y), 0xffffff, "%d", static_cast<int>(m_mapInfoData.mapCollisionData[x][y].chipType));



			// 針の描画
			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::RightNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::LeftNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::TopNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::BottomNeedle)
			{
				Triangle triangle = ChipTypeToTriangle(m_mapInfoData.mapCollisionData[x][y].chipType, m_mapInfoData.mapCollisionData[x][y].square);

				triangle.pos_1 += GameData::GetInstance()->GetCameraPos();
				triangle.pos_2 += GameData::GetInstance()->GetCameraPos();	
				triangle.pos_3 += GameData::GetInstance()->GetCameraPos();

				// 三角形描画
				DrawTriangleAA(
					triangle.pos_1.x, triangle.pos_1.y,
					triangle.pos_2.x, triangle.pos_2.y,
					triangle.pos_3.x, triangle.pos_3.y, 0xff0000, true);

				continue;
			}

			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::TransparentBlock)
			{
				continue;
			}

			// マップの描画
			DrawGraphF(pos1.x, pos1.y, m_testMapGraph[static_cast<int>(m_mapInfoData.mapCollisionData[x][y].chipType)], true);

			// マップの描画
			DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);

}

void ObjectManager::DrawSaveScore()
{
	// 現在のタイムを取得
	const Time currentTime = GameData::GetInstance()->GetTime();

	// 現在の死亡回数を取得
	const int currentDeathCount = GameData::GetInstance()->GetDeathCount();
	
	const GameData::Score saveScore = GameData::GetInstance()->CalcScore(currentTime, currentDeathCount);

	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph_GameMain::kClearTextPos.x,
		ScoreGraph_GameMain::kClearTextPos.y,
		ScoreGraph_GameMain::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[0],
		TRUE);


	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph_GameMain::kRetryTextPos.x,
		ScoreGraph_GameMain::kRetryTextPos.y,
		ScoreGraph_GameMain::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[1],
		TRUE);


	// 時間スコア
	{
		// 時間スコアの描画
		const int loopNum = static_cast<int>(saveScore.time.size());

		int count = 0;

		for (int i = 0; i < loopNum; i++)
		{
			if (i != 0 && i % 2 == 0)
			{
				const float x = ScoreGraph_GameMain::kTimeScorePos.x + ((i + count) * ScoreGraph_GameMain::kGraphInterval);

				DrawRotaGraphF(
					x,
					ScoreGraph_GameMain::kTimeScorePos.y,
					ScoreGraph_GameMain::kGraphSize,
					0.0,
					m_scoreGraphData.separateGraphHandle,
					TRUE);

				count++;
			}

			const float x = ScoreGraph_GameMain::kTimeScorePos.x + ((i + count) * ScoreGraph_GameMain::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph_GameMain::kTimeScorePos.y,
				ScoreGraph_GameMain::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[saveScore.time[i]],
				TRUE);
		}
	}

	// デスカウント
	{
		// デスカウントの描画
		const int loopNum = static_cast<int>(saveScore.deathCount.size());

		for (int i = 0; i < loopNum; i++)
		{


			const float x = ScoreGraph_GameMain::kDeathCountPos.x + (i * ScoreGraph_GameMain::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph_GameMain::kDeathCountPos.y,
				ScoreGraph_GameMain::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[saveScore.deathCount[i]],
				TRUE);
		}

		float x = ScoreGraph_GameMain::kDeathCountPos.x + (loopNum * ScoreGraph_GameMain::kGraphInterval);
		x += 5.0f;

		DrawRotaGraphF(
			x,
			ScoreGraph_GameMain::kDeathCountPos.y,
			ScoreGraph_GameMain::kGraphSize,
			0.0,
			m_scoreGraphData.countGraphHandle,
			TRUE);
	}
}
