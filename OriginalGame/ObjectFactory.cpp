#include "ObjectFactory.h"
#include "Player.h"
#include "GameData.h"
#include "Camera.h"
#include "game.h"

#include <cassert>
#include <string>
#include <filesystem>
#include <DxLib.h>


namespace
{
	// マップデータファイルパス
	const std::string kFieldDataFilePath = "Data/mapData_";

	// ファイルパス拡張子
	const std::string kFilePathExtension = ".fmf";

	// 判定半径
	constexpr float kHurtboxRadius = 2.0f;
}

namespace
{
	// 円の半径
	constexpr float kCircleRadius = 10.0f;
}


ObjectFactory::ObjectFactory() :
	m_object(),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>()),
	m_pCamera(std::make_shared<Camera>())
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	// マップ関連初期設定
	{
		// マップデータ初期設定
		InitMapDataFilePath();

		// マップ生成
		StageMove(MapSwitchType::Spawn);
	}

	// カメラクラスにオブジェクトファクトリークラスポインタを送る
	m_pCamera->SetObjectFactoryPointer(shared_from_this());

	// スクリーンサークル初期化
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
			FunctionConclusion::CalculateQuadrangularCenter(screenSquare);
	}
}

void ObjectFactory::Update()
{
	// 更新処理(ラムダ式使用)
	std::for_each(m_object.begin(), m_object.end(),
		[](std::shared_ptr<ObjectBase> object)
		{
			object->Update(); 
		});

	// スクリーンチェック
	ScreenCheck();

	// カメラ更新
	m_pCamera->Update();

	// オブジェクト削除
	ObjectErase();
}

void ObjectFactory::Draw()
{
	// 描画ランクの逆順にオブジェクトを描画するループ
	for (int i = static_cast<int>(ObjectBase::DrawRank::RankNum) - 1; i >= 0; --i) {
		// 描画ランク
		ObjectBase::DrawRank drawRank = static_cast<ObjectBase::DrawRank>(i);

		// 条件を満たすすべてのオブジェクトを描画する
		std::for_each(m_object.begin(), m_object.end(),
			[drawRank](const std::shared_ptr<ObjectBase>& object) {
				if (object->GetDrawRank() == drawRank) {
					object->Draw();
				}
			});
	}
	
	// マップ描画
	TestMapDraw();
}

void ObjectFactory::CharacterCreate(const Vec2& pos)
{
	// 円情報代入
	Circle circle = Circle(pos, kCircleRadius);

	// キャラクター生成
	m_object.push_back(std::make_shared<Player>());

	// ポインタを送る
	m_object.back()->SetObjectFactory(shared_from_this());

	// 描画ランクを代入
	m_object.back()->SetDrawRank(ObjectBase::DrawRank::Rank_1);

	// 円情報を入れる
	m_object.back()->SetCircle(circle);

	// 初期設定
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType)
{
	// マップデータ
	std::vector<std::vector<MapCollisionData>>mapCollisionData(
		m_mapInfoData.mapChip.mapWidth, 
		std::vector<MapCollisionData>(m_mapInfoData.mapChip.mapHeight));

	// すべてのセルを見る
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// マップチップタイプに変換
			const ChipType mapChipType = ChipType(mapData[x][y]);

			// スポーン
			if (mapChipType == ChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// セーブ情報を送る
				GameData::GetInstance()->SetSavePointData(GameData::SavePointData(m_mapInfoData.stageNumber, Cell(x, y)));

				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y), m_mapInfoData.mapChip.chipSize));
			}

			// 次のステージ
			if (mapChipType == ChipType::NextPos &&
				mapSwitchType == MapSwitchType::NextStage)
			{
				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfoData.mapChip.chipSize));
			}

			// 前のステージ
			if (mapChipType == ChipType::PreviousePos &&
				mapSwitchType == MapSwitchType::PreviouseStage)
			{
				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfoData.mapChip.chipSize));
			}

			// マップ判定データ
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
					FunctionConclusion::CalculateQuadrangularCenter(mapCollisionData[x][y].square);
			}
		}
	}

	// リスポーン
	if (mapSwitchType == MapSwitchType::Respawn)
	{
		// キャラクター生成
		CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(GameData::GetInstance()->GetSavePointData().cell, m_mapInfoData.mapChip.chipSize));
	}

	// 次のステージのマップデータを代入する
	m_mapInfoData.mapCollisionData = mapCollisionData;
}

void ObjectFactory::ObjectErase()
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

void ObjectFactory::StageMove(const MapSwitchType& mapSwitchType)
{
	// 次のステージに移動するかどうかのフラグからステージナンバーを増やすか決める
	if (mapSwitchType == MapSwitchType::NextStage)
	{
		// ステージナンバーの数字を増やす
		m_mapInfoData.stageNumber++;
	}
	else if(mapSwitchType == MapSwitchType::PreviouseStage)
	{
		// ステージナンバーの数字を減らす;
		m_mapInfoData.stageNumber--;
	}
	else if (mapSwitchType == MapSwitchType::Respawn)
	{
		// セーブポイントデータのステージナンバーをステージナンバーの数字に代入
		m_mapInfoData.stageNumber = GameData::GetInstance()->GetSavePointData().stageNumber;
	}


	// 要素削除
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// プラチナムデータロード
	m_pPlatinumLoader->Load(m_mapInfoData.filePath[m_mapInfoData.stageNumber].c_str());
	
	// プラチナムデータ取得
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();

	// マップのレイヤー
	const int mapLayer = 0;
	
	// マップ情報取得
	m_mapInfoData.mapChip = m_pPlatinumLoader->GetMapChip();

	// マップ生成
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


std::vector<std::vector<int>> ObjectFactory::GetMapChipNumber()
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

ObjectFactory::ChipType ObjectFactory::GetMapChipType(const Vec2& pos)
{
	// 座標からセルを求める
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfoData.mapChip.chipSize);

	// セルが範囲外ならば、ここで処理を終了する
	if (!FunctionConclusion::IsCellRange(cell, Cell(m_mapInfoData.mapChip.mapWidth, m_mapInfoData.mapChip.mapHeight), Cell(0, 0)))
	{
		return ChipType::NotExists;
	}

	// マップチップがセーブだった場合、ゲームデータクラスにセーブ情報を送る
	if (m_mapInfoData.mapCollisionData[cell.x][cell.y].chipType == ChipType::Save)
	{
		// セーブ情報を送る
		GameData::GetInstance()->SetSavePointData(GameData::SavePointData(m_mapInfoData.stageNumber, cell));
	}

	// マップチップタイプを返す
	return m_mapInfoData.mapCollisionData[cell.x][cell.y].chipType;
}

void ObjectFactory::SetSavePoint(const Vec2& pos)
{
	// 座標からセルを求める
	const Cell saveCell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfoData.mapChip.chipSize);

	// セーブポイントのセルを設定
	GameData::GetInstance()->SetSavePointData(
		GameData::SavePointData(m_mapInfoData.stageNumber, saveCell));

	return;
}

std::tuple<bool, Vec2>  ObjectFactory::GetSavePointPos()
{
	// セーブポイントデータを取得
	const GameData::SavePointData savePointData = GameData::GetInstance()->GetSavePointData();

	// セーブポイントデータのステージナンバーと現在のステージナンバーが異なる場合、マップ生成を行う
	if (savePointData.stageNumber != m_mapInfoData.stageNumber)
	{
		// マップ移動処理
		StageMove(MapSwitchType::Respawn);

		return std::tuple<bool, Vec2>(true, Vec2());
	}

	// セルから変換した座標を返す
	return std::tuple<bool, Vec2>(false, FunctionConclusion::CellWithCoordinateToConversion(savePointData.cell, m_mapInfoData.mapChip.chipSize));
}

void ObjectFactory::InitMapDataFilePath()
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

bool ObjectFactory::IsCellCheckOutOfRange(const Cell& cell)
{
	if (cell.x < 0)return true;
	if (cell.x >= m_mapInfoData.mapChip.mapWidth)return true;
	if (cell.y < 0)return true;
	if (cell.y >= m_mapInfoData.mapChip.mapHeight)return true;

	return false;
}

Triangle ObjectFactory::ChipTypeToTriangle(const ChipType& needleDirection, const Square& square)
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

void ObjectFactory::ScreenCheck()
{
	// 判定用の円情報
	Circle collisionMapCircle = Circle();


	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// 円情報をコピー
			collisionMapCircle = m_mapInfoData.mapCollisionData[x][y].circle;

			// 座標にオフセット値を加える
			collisionMapCircle.centerPos += GameData::GetInstance()->GetCameraPos();

			// 円の判定
			m_mapInfoData.mapCollisionData[x][y].screenFlag = FunctionConclusion::CircleCollision(collisionMapCircle, m_screenCircle);
		}
	}
}

void ObjectFactory::TestMapDraw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,200);

	Vec2 pos1;
	Vec2 pos2;


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

			// マップの描画
			DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}