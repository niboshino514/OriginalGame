#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"

#include "NoramalMapChip.h"
#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"

namespace
{

	
	// 盤面情報
	const std::vector<std::vector<int>> kMap_1 =
	{
		{0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	};

	// 盤面情報
	const std::vector<std::vector<int>> kMap_2 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2},
	};

	// 盤面情報1
	const Map kMapTest_1 =
	{
		{
			{0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		}
	};


	// チップサイズ
	constexpr float kChipSize = 64.0f;

	std::vector<Map>kMapInfo
	{
		Map(kMap_1),
		Map(kMap_2)
	};


}

namespace
{
	// 円の半径
	constexpr float kCircleRadius = 10.0f;
}


ObjectFactory::ObjectFactory() :
	m_stageNumber(0)
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	m_mapInfo_test.push_back(kMapTest_1);

	



	// マップ生成
	MapChipCreate(m_mapInfo_test[m_stageNumber]);


	for (auto& character : m_characterPos)
	{
		// キャラクター生成
		CharacterCreate(character);
	}

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

void ObjectFactory::MapChipCreate(const Map& mapData)
{
	// マップ縦横幅代入
	const int cellWidth = static_cast<int>(mapData.mapInfo[0].size());
	const int cellHeight = static_cast<int>(mapData.mapInfo.size());


	// 四角形情報
	Square square = Square();

	// マップチップの左上座標
	Vec2 topLeftmapChipPos = Vec2();


	// すべてのセルを見る
	for (int y = 0; y < cellHeight; y++) 
	{
		for (int x = 0; x < cellWidth; x++)
		{
			// マップチップの左上座標代入
			topLeftmapChipPos = Vec2(x * kChipSize, y * kChipSize);

			// 四角形情報代入
			square.A = topLeftmapChipPos;
			square.B = Vec2(square.A.x + kChipSize, square.A.y);
			square.C = Vec2(square.A.x + kChipSize, square.A.y + kChipSize);
			square.D = Vec2(square.A.x, square.A.y + kChipSize);

			switch (mapData.mapInfo[y][x])
			{
			case 0:
				// 侵入不可マップチップ生成
				m_object.push_back(std::make_shared<NoneMapChip>());

				break;
			case 1:
				// 通常マップチップ生成
				m_object.push_back(std::make_shared<NoramalMapChip>());

				break;

			case 2:
				// 障害物マップチップ生成
				m_object.push_back(std::make_shared<ObstacleMapChip>());

				break;
			case 3:
				// 次のステージに進むマップチップ生成
				m_object.push_back(std::make_shared<NextStageMapChip>());

				break;
			default:

				// キャラクターの座標を入れる
				m_characterPos.push_back(MapChipCenterPos(topLeftmapChipPos));

				// 通常マップチップ生成
				m_object.push_back(std::make_shared<NoramalMapChip>());

				break;
			}
		

			// ポインタを送る
			m_object.back()->SetObjectFactory(shared_from_this());

			// 座標を入れる
			m_object.back()->SetSquare(square);

			// 初期設定
			m_object.back()->Init();
		}
	}
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

void ObjectFactory::NextStageToMigration()
{
	for (auto& object : m_object)
	{
		if (object->GetObjectID() == ObjectBase::ObjectID::Player)
		{
			continue;
		}
		object->SetIsExlist(false);
	}

	// ステージナンバーの数字を増やす
	m_stageNumber++;

	// 次のステージのマップ
	MapChipCreate(kMapInfo[m_stageNumber]);
}

Vec2 ObjectFactory::MapChipCenterPos(const Vec2& topLeftmapChipPos)
{
	// 中心座標にマップチップの左上座標を代入
	Vec2 centerPos = topLeftmapChipPos;

	// マップチップの半分サイズを足す
	centerPos.x += (kChipSize * 0.5f);
	centerPos.y += (kChipSize * 0.5f);

	// マップチップの中心座標を返す
	return centerPos;
}

void ObjectFactory::MapSwitch()
{
}