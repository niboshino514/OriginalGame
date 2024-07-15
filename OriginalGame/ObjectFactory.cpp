#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"

#include "NoramalMapChip.h"
#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
#include <cassert>

namespace
{

	// 0:何もなし(侵入不可)
	// 1:通常セル
	// 2:障害物セル(侵入不可)
	// 3:次のステージに進む
	// 4:前のステージに戻る
	// 5:次のステージに進んだ際のプレイヤーの初期セル
	// 6:前のステージに戻った際のプレイヤーの初期セル
	// 7:スポーンセル(ゲームを始めた際の初期セル等)
	
	
	// 盤面情報
	const std::vector<std::vector<int>> kMap_1 =
	{
		{0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 6, 3},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	};

	// 盤面情報
	const std::vector<std::vector<int>> kMap_2 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{4, 5, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
	};


	// チップサイズ
	constexpr float kChipSize = 64.0f;


	// マップ情報
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

void ObjectFactory::MapChipCreate(const Map& mapData, const MapSwitchType& mapSwitchType)
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
			case 4:
				// 前のステージに戻るマップチップ生成
				m_object.push_back(std::make_shared<PreviousStageMapChip>());

				break;

			default:

				if (mapSwitchType == MapSwitchType::Spawn && mapData.mapInfo[y][x] == 7)
				{
					// キャラクター生成
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// 通常マップチップ生成
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}
				else if ((mapSwitchType == MapSwitchType::NextStage && mapData.mapInfo[y][x] == 5))
				{
					// キャラクター生成
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// 通常マップチップ生成
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}
				else if ((mapSwitchType == MapSwitchType::PreviousStage && mapData.mapInfo[y][x] == 6))
				{
					// キャラクター生成
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// 通常マップチップ生成
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}

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

void ObjectFactory::StageMove(const MapSwitchType& mapSwitchType)
{
	// 次のステージに移動するかどうかのフラグからステージナンバーを増やすか決める
	if (mapSwitchType == MapSwitchType::NextStage)
	{
		// ステージナンバーの数字を増やす
		m_stageNumber++;


		int test = static_cast<int>(kMapInfo.size());


		assert((test > m_stageNumber) &&
			"ステージナンバーに上限以上の数字が入っているようです。");

	}
	else if(mapSwitchType == MapSwitchType::PreviousStage)
	{
		// ステージナンバーの数字を減らす;
		m_stageNumber--;

		assert((-1 < m_stageNumber) &&
			"ステージナンバーに0以下の数字が入っているようです。");
	}

	// 要素削除
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}


	// 次のステージのマップ
	MapChipCreate(kMapInfo[m_stageNumber], mapSwitchType);
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