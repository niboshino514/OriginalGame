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
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
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


	


	// チップサイズ
	constexpr float kChipSize = 64.0f;

	std::vector<Map>kTest
	{
		Map(kMap_1),
		Map(kMap_2)
	};

	
	

	Map test = Map(kMap_1);

}

namespace
{

	// プレイヤー円情報
	const Circle kPlayerCircle = Circle(Vec2(100, 100), 10.0f);


}


ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	// キャラクター生成
	CharacterCreate();

	// マップ生成
	MapChipCreate(kMap_1);
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

void ObjectFactory::CharacterCreate()
{
	// キャラクター生成
	m_object.push_back(std::make_shared<Player>());

	// ポインタを送る
	m_object.back()->SetObjectFactory(shared_from_this());

	// 座標を入れる
	m_object.back()->SetCircle(kPlayerCircle);

	// 初期設定
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData)
{
	// マップ縦横幅代入
	const int cellWidth = static_cast<int>(mapData[0].size());
	const int cellHeight = static_cast<int>(mapData.size());


	// 四角形情報
	Square square = Square();


	// すべてのセルを見る
	for (int y = 0; y < cellHeight; y++) 
	{
		for (int x = 0; x < cellWidth; x++)
		{
			// 四角形情報代入
			square.A = Vec2(x * kChipSize, y * kChipSize);
			square.B = Vec2(square.A.x + kChipSize, square.A.y);
			square.C = Vec2(square.A.x + kChipSize, square.A.y + kChipSize);
			square.D = Vec2(square.A.x, square.A.y + kChipSize);

			switch (mapData[y][x])
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


	// 次のステージのマップ
	MapChipCreate(kMap_2);
}