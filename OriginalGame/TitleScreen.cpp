#include "TitleScreen.h"
#include <DxLib.h>
#include "Game.h"
#include "SceneTitle.h"
#include "Controller.h"
#include "EvoLib.h"
#include "Sound.h"
#include "GameData.h"


namespace GraphData
{

	// 選択されていないグラフィックの透明度
	constexpr int kAlpha = 100;

	// 選択されているグラフィックを左にずらす
	constexpr float kSelectMoveX = -10.0f;

	// 選択中のグラフィックデータ
	static const EvoLib::Load::GraphicInfo kSelectGraphInfo =
	{
		// ファイルパス
		"Data/Graphic/Title/Select/Select.png",

		// 中心座標
		Vec2(Game::kWindowCenterX_F + 20.0f, Game::kWindowCenterY_F+80.0f),

		// グラフィックスケール
		0.7,

		// グラフィックの分割数
		EvoLib::Load::DivNum(1,3),
	};

	// 選択中のグラフィックの距離
	const Vec2 kSelectGraphDistance = Vec2(0.0f, 20.0f);


	// タイトルグラフィックのファイル名
	const char* const kGameTitleGraphFileName = "Data/Graphic/Title/GameTitle.png";
	// タイトルグラフィックの位置
	const Vec2 kGameTitleGraphPos = Vec2(Game::kWindowCenterX_F, Game::kWindowCenterY_F - 230.0f);
	// タイトルグラフィックの拡大率
	constexpr double kGameTitleGraphScale = 1.0;

	// 背景グラフィックのファイル名
	const char* const kBackgroundGraphFileName = "Data/Graphic/Title/Background.png";
	// 背景の移動速度
	constexpr float kBackgroundMoveSpeed = 2.0f;

}

namespace SelectTriangleGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Selection/SelectTriangle.png";

	// 拡大率
	const float kScale = 0.6f;
	// フレーム速度
	const int kFrameSpeed = 1;

	// 距離
	const Vec2 kSelectDistanceValue = Vec2(-250, 0);
}

namespace
{
	// サウンドファイル名
	static std::vector<std::string> kSoundFileName =
	{
		"Bgm_0",		// BGM

		"Determination",// 決定音
		"Cancel",		// キャンセル音
		"Select",		// 選択音
	};

	// サウンド名
	enum class SoundName
	{
		BGM,			// BGM

		Decision,		// 決定音
		Cancel,			// キャンセル音
		Select,			// 選択音
	};

}

namespace ScoreGraph_Title
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
	const char* const kClearRetryTextGraphFileName = "Data/Graphic/Ending/ClearRetryText.png";


	// タイムスコアの位置
	const Vec2 kTimeScorePos = Vec2(Game::kWindowCenterX_F , Game::kWindowCenterY_F + 300.0f);
	// デスカウントの位置
	const Vec2 kDeathCountPos = Vec2(Game::kWindowCenterX_F, kTimeScorePos.y +30.0f);

	// グラフィック同士の間隔
	const float kTextGraphInterval = -100.0f;

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

TitleScreen::TitleScreen():
	m_selectGraphInfo(),
	m_selectTriangleGraph(0),
	m_select(Select()),
	m_isContinue(false),
	m_gameTitleGraph(-1),
	m_score(),
	m_isClear(false),
	m_scoreGraphData(),
	m_backgroundGraph(-1),
	m_currentMoveValue(0.0f),
	m_pStateMachine(),
	m_pSceneTitle(nullptr)
{
}

TitleScreen::~TitleScreen()
{
	// グラフィックの解放
	for(auto& handle : m_selectGraphInfo.handle)
	{
		DeleteGraph(handle);
	}

	// グラフィックの解放
	DeleteGraph(m_selectTriangleGraph);

	// グラフィックの解放
	for (auto& handle : m_scoreGraphData.numberGraphHandle)
	{
		DeleteGraph(handle);
	}

	// グラフィックの解放
	DeleteGraph(m_scoreGraphData.separateGraphHandle);

	// グラフィックの解放
	DeleteGraph(m_scoreGraphData.countGraphHandle);

	// グラフィックの解放
	for (auto& handle : m_scoreGraphData.clearRetryTextGraphHandle)
	{
		DeleteGraph(handle);
	}

	// グラフィックの解放
	DeleteGraph(m_backgroundGraph);


}

void TitleScreen::Init()
{
	// グラフィックの読み込み
	Load();

	// セーブデータ確認
	CheckSaveData();

	// スコアデータ確認
	CheckScoreData();

	// ステートの初期化
	StateInit();

	// BGM再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::BGM)]);
}

void TitleScreen::Update()
{
	// ステートマシンの更新
	m_pStateMachine.Update();
}

void TitleScreen::Draw()
{
	// 背景
	DrawBackground();

	// タイトルグラフィックの描画
	DrawRotaGraphF(
		GraphData::kGameTitleGraphPos.x,
		GraphData::kGameTitleGraphPos.y,
		GraphData::kGameTitleGraphScale,
		0.0,
		m_gameTitleGraph,
		TRUE);


	// ステートマシンの描画
	m_pStateMachine.Draw();
	
	// スコアの描画
	DrawScore();
}

void TitleScreen::Load()
{
	// サウンドの読み込み
	{
		Sound::GetInstance()->Load(kSoundFileName);	
	}


	// 選択中のグラフィック読み込み
	{
		// グラフィックの読み込み
		m_selectGraphInfo.handle = 
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(
				GraphData::kSelectGraphInfo.filePath,
				GraphData::kSelectGraphInfo.div);

		// グラフィックのサイズを取得
		const Vec2 size = 
			EvoLib::Calculation::GetGraphSize_EvoLib(
				m_selectGraphInfo.handle,
				GraphData::kSelectGraphInfo.scale);

		// サイズを代入
		for(int i = 0; i < m_selectGraphInfo.handle.size(); i++)
		{
			m_selectGraphInfo.scale.push_back(GraphData::kSelectGraphInfo.scale);
		}

		// グラフィックを均等に配置する
		m_selectGraphInfo.pos =
			EvoLib::Calculation::GraphEqualization(
				size,
				GraphData::kSelectGraphInfo.pos,
				GraphData::kSelectGraphInfo.div.y,
				GraphData::kSelectGraphDistance,
				false);
	}


	// セレクト三角形グラフィック
	{
		// グラフィックロード
		m_selectTriangleGraph = LoadGraph(SelectTriangleGraph::kFilePath);
	}

	// スコアグラフィックロード
	{
		m_scoreGraphData.numberGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph_Title::kNumberGraphFileName, ScoreGraph_Title::kNumberGraphDivNum);

		m_scoreGraphData.separateGraphHandle = LoadGraph(ScoreGraph_Title::kSeparateGraphFileName);
		m_scoreGraphData.countGraphHandle = LoadGraph(ScoreGraph_Title::kCountGraphFileName);

		// クリアリトライ回数テキストグラフィックロード
		m_scoreGraphData.clearRetryTextGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph_Title::kClearRetryTextGraphFileName, EvoLib::Load::DivNum(1, 2));

		{
			// グラフィックのサイズを取得
			const Vec2 size = EvoLib::Calculation::GetGraphSize_EvoLib(m_scoreGraphData.numberGraphHandle, ScoreGraph_Title::kGraphSize);

			// グラフィックの間隔を設定
			ScoreGraph_Title::kGraphInterval = size.x + ScoreGraph_Title::kGraphIntervalSetting;
		}
	}

	// ゲームタイトルグラフィック
	{
		// グラフィックの読み込み
		m_gameTitleGraph = LoadGraph(GraphData::kGameTitleGraphFileName);
	}

	// 背景グラフィック
	{
		// グラフィックの読み込み
		m_backgroundGraph = LoadGraph(GraphData::kBackgroundGraphFileName);
	}
}

void TitleScreen::StateInit()
{
	// ステートマシンの初期化
	auto dummy = []() {};

	// 入力待ちステート
	{
		auto enter = [this]() {this->StateWaitEnter(); };
		auto update = [this]() {this->StateWaitUpdate(); };
		auto draw = [this]() {this->StateWaitDraw(); };
		auto exit = [this]() {this->StateWaitExit(); };

		m_pStateMachine.AddState(State::WaitInput, enter, update, draw, exit);
	}

	// 選択中ステート
	{
		auto enter = [this]() {this->StateSelectEnter(); };
		auto update = [this]() {this->StateSelectUpdate(); };
		auto draw = [this]() {this->StateSelectDraw(); };
		auto exit = [this]() {this->StateSelectExit(); };

		m_pStateMachine.AddState(State::Selecting, enter, update, draw, exit);
	}

	// 初期ステート設定
	m_pStateMachine.SetState(State::Selecting);
}

void TitleScreen::StateWaitEnter()
{
}

void TitleScreen::StateWaitUpdate()
{
	// 決定ボタンが押されたら、選択中ステートに遷移する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		// 選択中ステートに遷移
		m_pStateMachine.SetState(State::Selecting);
	}
}

void TitleScreen::StateWaitDraw()
{
}

void TitleScreen::StateWaitExit()
{
}

void TitleScreen::StateSelectEnter()
{
}

void TitleScreen::StateSelectUpdate()
{
	// 選択処理
	SelectProcess();

	// 決定処理
	DecideProcess();
}

void TitleScreen::StateSelectDraw()
{
	// 選択されていないグラフィック描画
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, GraphData::kAlpha);

		// グラフィック描画
		for (int i = 0; i < m_selectGraphInfo.handle.size(); i++)
		{
			// 選択中ならば、continueする
			if (i == static_cast<int>(m_select))
			{
				continue;
			}

			if (!m_isContinue &&
				i == static_cast<int>(Select::Continue))
			{
				continue;
			}


			DrawRotaGraphF(
				m_selectGraphInfo.pos[i].x,
				m_selectGraphInfo.pos[i].y,
				m_selectGraphInfo.scale[i],
				0.0f,
				m_selectGraphInfo.handle[i],
				true);
		}
		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 選ばれている選択肢は赤色で描画
	SetDrawBright(255, 0, 0);

	// 選択中のグラフィック描画
	{
		// グラフィック描画
		DrawRotaGraphF(
			m_selectGraphInfo.pos[static_cast<int>(m_select)].x + GraphData::kSelectMoveX,
			m_selectGraphInfo.pos[static_cast<int>(m_select)].y,
			m_selectGraphInfo.scale[static_cast<int>(m_select)],
			0.0f,
			m_selectGraphInfo.handle[static_cast<int>(m_select)],
			true);
	}


	// 選択されている三角形描画
	{
		Vec2 pos = Vec2();
		pos.x = (m_selectGraphInfo.pos[static_cast<int>(m_select)].x + GraphData::kSelectMoveX);
		pos.y = m_selectGraphInfo.pos[static_cast<int>(m_select)].y;

		pos += SelectTriangleGraph::kSelectDistanceValue;

		// 回転描画
		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			pos,
			SelectTriangleGraph::kFrameSpeed,
			false,
			SelectTriangleGraph::kScale);
	}

	// 色を元に戻す
	SetDrawBright(255, 255, 255);
}

void TitleScreen::StateSelectExit()
{
}

void TitleScreen::CheckSaveData()
{
	// セーブデータが初期化されているかどうかを取得
	const bool isSaveDataInit = GameData::GetInstance()->IsSaveDataInit();

	// セーブデータが初期化されているならば、続きからプレイできない
	if(isSaveDataInit)
	{
		m_isContinue = false;

		// セレクトの初期をNewGameに設定
		m_select = Select::NewGame;
	}
	else
	{
		m_isContinue = true;
	}

}

void TitleScreen::CheckScoreData()
{
	// スコアデータの読み込み
	const GameData::ScoreData scoreData = 
		GameData::GetInstance()->GetScoreData();


	m_score = GameData::GetInstance()->CalcScore
	(scoreData.clearTime, scoreData.deathCount);

	// クリアしているかどうか
	m_isClear = scoreData.isClear;

}

void TitleScreen::SelectProcess()
{
	// セレクト数
	const int selectNum = static_cast<int>(Select::SelectNum);




	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		m_select = static_cast<Select>(((static_cast<int>(m_select) - 1 + selectNum) % selectNum));

		if (!m_isContinue &&
			m_select == Select::Continue)
		{
			m_select = static_cast<Select>((static_cast<int>(m_select) - 1 + selectNum));
		}

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		m_select = static_cast<Select>(((static_cast<int>(m_select) + 1) % selectNum));

		if(!m_isContinue&&
			m_select == Select::Continue)
		{
			m_select = static_cast<Select>(((static_cast<int>(m_select) + 1)));
		}

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void TitleScreen::DecideProcess()
{
	// フェードインが終了していなければ、return
	if (!m_pSceneTitle->IsFadeInEnd())
	{
		return;
	}

	// 決定ボタンが押されていなければ、return
	if (!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}



	// 選択されたものによって処理を分岐
	switch (m_select)
	{
	case TitleScreen::Select::Continue:
		// シーンを変更する
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::GameMain);

		break;
	case TitleScreen::Select::NewGame:
		// セーブデータを初期化
		GameData::GetInstance()->InitSaveData();
		// シーンを変更する
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::GameMain);

	
		break;
	case TitleScreen::Select::GameEnd:
		
		GameData::GetInstance()->WriteSaveData();	// セーブデータを書き込む
		GameData::GetInstance()->WriteScoreData();	// スコアデータの書き込み

		// ゲーム終了
		DxLib_End();

		break;
	default:
		break;
	}

	// 決定音を再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Decision)]);
}

void TitleScreen::DrawScore()
{
	// クリアしていないならば、return
	if (!m_isClear)
	{
		return;
	}


	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph_Title::kClearTextPos.x,
		ScoreGraph_Title::kClearTextPos.y,
		ScoreGraph_Title::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[0],
		TRUE);


	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph_Title::kRetryTextPos.x,
		ScoreGraph_Title::kRetryTextPos.y,
		ScoreGraph_Title::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[1],
		TRUE);


	// 時間スコア
	{
		// 時間スコアの描画
		const int loopNum = static_cast<int>(m_score.time.size());

		int count = 0;

		for (int i = 0; i < loopNum; i++)
		{
			if (i != 0 && i % 2 == 0)
			{
				const float x = ScoreGraph_Title::kTimeScorePos.x + ((i + count) * ScoreGraph_Title::kGraphInterval);

				DrawRotaGraphF(
					x,
					ScoreGraph_Title::kTimeScorePos.y,
					ScoreGraph_Title::kGraphSize,
					0.0,
					m_scoreGraphData.separateGraphHandle,
					TRUE);

				count++;
			}

			const float x = ScoreGraph_Title::kTimeScorePos.x + ((i + count) * ScoreGraph_Title::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph_Title::kTimeScorePos.y,
				ScoreGraph_Title::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[m_score.time[i]],
				TRUE);
		}
	}

	// デスカウント
	{
		// デスカウントの描画
		const int loopNum = static_cast<int>(m_score.deathCount.size());

		for (int i = 0; i < loopNum; i++)
		{


			const float x = ScoreGraph_Title::kDeathCountPos.x + (i * ScoreGraph_Title::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph_Title::kDeathCountPos.y,
				ScoreGraph_Title::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[m_score.deathCount[i]],
				TRUE);
		}

		float x = ScoreGraph_Title::kDeathCountPos.x + (loopNum * ScoreGraph_Title::kGraphInterval);
		x += 5.0f;

		DrawRotaGraphF(
			x,
			ScoreGraph_Title::kDeathCountPos.y,
			ScoreGraph_Title::kGraphSize,
			0.0,
			m_scoreGraphData.countGraphHandle,
			TRUE);
	}
}

void TitleScreen::DrawBackground()
{
	m_currentMoveValue += GraphData::kBackgroundMoveSpeed;

	if (m_currentMoveValue >= Game::kScreenWidth)
	{
		m_currentMoveValue = 0.0f;
	}

	EvoLib::Draw::SimpleLoopBackground(
		m_backgroundGraph,
		m_currentMoveValue,
		GraphData::kBackgroundMoveSpeed,
		Vec2(Game::kScreenWidth, Game::kScreenHeight),
		true,
		EvoLib::Draw::LoopDirection::Right);
}
