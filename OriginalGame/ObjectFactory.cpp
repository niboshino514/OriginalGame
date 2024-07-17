#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"


#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
#include <cassert>
#include <string>
#include <filesystem>

namespace
{



	// マップデータファイルパス
	const std::string kFieldDataFilePath = "Data/mapData_";

	// ファイルパス拡張子
	const std::string kFilePathExtension = ".fmf";


	const std::string kTestFilePath = "Data/mapData_1.fmf";
}

namespace
{
	// 円の半径
	constexpr float kCircleRadius = 10.0f;
}


ObjectFactory::ObjectFactory() :
	m_object(),
	m_mapInfo(),
	m_stageNumber(0),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>())
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	InitMapDataFilePath();


	// マップ生成
	StageMove(MapSwitchType::Spawn);
}

void ObjectFactory::Update()
{
	// 更新処理
	for (auto& object : m_object)
	{
		object->Update();
	}

	// オブジェクト削除
	ObjectErase();
}

void ObjectFactory::Draw()
{
	// 描画処理
	for (auto& object : m_object)
	{
		object->Draw();
	}

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

	// 円情報を入れる
	m_object.back()->SetCircle(circle);

	// 初期設定
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType)
{

	testMapData = mapData;

	// 四角形情報
	Square square = Square();

	// マップチップの左上座標
	Vec2 topLeftmapChipPos = Vec2();


	//// すべてのセルを見る
	//for (int y = 0; y < m_mapInfo.mapHeight; y++)
	//{
	//	for (int x = 0; x < m_mapInfo.mapWidth; x++)
	//	{
	//		// マップチップの左上座標代入
	//		topLeftmapChipPos = Vec2(x * m_mapInfo.chipSize, y * m_mapInfo.chipSize);

	//		// 四角形情報代入
	//		square.A = topLeftmapChipPos;
	//		square.B = Vec2(square.A.x + m_mapInfo.chipSize, square.A.y);
	//		square.C = Vec2(square.A.x + m_mapInfo.chipSize, square.A.y + m_mapInfo.chipSize);
	//		square.D = Vec2(square.A.x, square.A.y + m_mapInfo.chipSize);




	//		switch (mapData[x][y])
	//		{
	//		case 0:
	//			// 侵入不可マップチップ生成
	//			//m_object.push_back(std::make_shared<NoneMapChip>());

	//			break;
	//
	//		case 1:
	//			// 障害物マップチップ生成
	//			m_object.push_back(std::make_shared<ObstacleMapChip>());

	//			break;
	//		case 2:
	//			// 次のステージに進むマップチップ生成
	//			m_object.push_back(std::make_shared<NextStageMapChip>());

	//			break;
	//		case 3:
	//			// 前のステージに戻るマップチップ生成
	//			m_object.push_back(std::make_shared<PreviousStageMapChip>());

	//			break;

	//		default:

	//			if (mapSwitchType == MapSwitchType::Spawn && mapData[y][x] == 6)
	//			{
	//				// キャラクター生成
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//				
	//			}
	//			else if ((mapSwitchType == MapSwitchType::NextStage && mapData[y][x] == 4))
	//			{
	//				// キャラクター生成
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//			
	//			}
	//			else if ((mapSwitchType == MapSwitchType::PreviousStage && mapData[y][x] == 5))
	//			{
	//				// キャラクター生成
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//			}

	//			break;
	//		}
	//	

	//		// ポインタを送る
	//		m_object.back()->SetObjectFactory(shared_from_this());

	//		// 座標を入れる
	//		m_object.back()->SetSquare(square);

	//		// 初期設定
	//		m_object.back()->Init();
	//	}
	//}
}

void ObjectFactory::ObjectErase()
{	// すべてのオブジェクトを見て、削除する
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
		m_stageNumber++;
	}
	else if(mapSwitchType == MapSwitchType::PreviousStage)
	{
		// ステージナンバーの数字を減らす;
		m_stageNumber--;
	}

	// 要素削除
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// プラチナムデータロード
	//m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	m_pPlatinumLoader->Load(kTestFilePath.c_str());

	// プラチナムデータ取得
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// マップのレイヤー
	const int mapLayer = 0;

	
	// マップ情報取得
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();


	// 次のステージのマップ
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


Vec2 ObjectFactory::MapChipCenterPos(const Vec2& topLeftmapChipPos)
{
	// 中心座標にマップチップの左上座標を代入
	Vec2 centerPos = topLeftmapChipPos;

	// マップチップの半分サイズを足す
	centerPos.x += (m_mapInfo.chipSize * 0.5f);
	centerPos.y += (m_mapInfo.chipSize * 0.5f);

	// マップチップの中心座標を返す
	return centerPos;
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
		m_mapDataFilePath.push_back(filePath);


		// 処理カウントを増やす
		processCount++;
	}



}

void ObjectFactory::TestMapDraw()
{
	Vec2 pos1;
	Vec2 pos2;


	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// チップ番号の代入
			int chipNum = testMapData[x][y];

			// 座標の代入
			pos1 = Vec2(x * m_mapInfo.chipSize, y * m_mapInfo.chipSize);


			pos2 = Vec2(pos1.x + m_mapInfo.chipSize, pos1.y + m_mapInfo.chipSize);




			int color = 0x00ff00;

			if (chipNum == 1)
			{
				color = 0xff0000;
			}

			Vec2 stringPos = Vec2(pos1.x + m_mapInfo.chipSize * 0.5, pos1.y + m_mapInfo.chipSize * 0.5);

			DrawFormatString(stringPos.x, stringPos.y, 0xffffff, "%d", chipNum);

			// マップの描画
			DrawBox(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}
}