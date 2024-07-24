#include "ObjectFactory.h"
#include "Player.h"
#include "GameData.h"

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
	m_mapInfo(),
	m_stageNumber(0),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>()),
	m_pGameData(std::make_shared<GameData>())
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	// マップデータ初期設定
	InitMapDataFilePath();

	// マップ生成
	StageMove(MapSwitchType::Spawn);
}

void ObjectFactory::Update()
{
	// 更新処理(ラムダ式使用)
	std::for_each(m_object.begin(), m_object.end(),
		[](std::shared_ptr<ObjectBase> object) {object->Update(); });


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
	// すべてのセルを見る
	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// マップチップタイプに変換
			const MapChipType mapChipType = MapChipType(mapData[x][y]);

			// スポーン
			if (mapChipType == MapChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// セーブ情報を送る
				m_pGameData->SetSavePointData(GameData::SavePointData(m_stageNumber, Cell(x, y)));

				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y),m_mapInfo.chipSize));
			}

			// 次のステージ
			if (mapChipType == MapChipType::NextPos &&
				mapSwitchType == MapSwitchType::NextStage)
			{
				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize));
			}

			// 前のステージ
			if (mapChipType == MapChipType::PreviousePos &&
				mapSwitchType == MapSwitchType::PreviouseStage)
			{
				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize));
			}
		}
	}

	// リスポーン
	if (mapSwitchType == MapSwitchType::Respawn)
	{
		// キャラクター生成
		CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(m_pGameData->GetSavePointData().cell, m_mapInfo.chipSize));
	}
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
		m_stageNumber++;
	}
	else if(mapSwitchType == MapSwitchType::PreviouseStage)
	{
		// ステージナンバーの数字を減らす;
		m_stageNumber--;
	}
	else if (mapSwitchType == MapSwitchType::Respawn)
	{
		// セーブポイントデータのステージナンバーをステージナンバーの数字に代入
		m_stageNumber = m_pGameData->GetSavePointData().stageNumber;
	}


	// 要素削除
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// プラチナムデータロード
	m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	

	// プラチナムデータ取得
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// マップのレイヤー
	const int mapLayer = 0;

	
	// マップ情報取得
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();

	// 次のステージのマップデータを代入する
	m_currentMapData = mapData[mapLayer].mapData;

	// 次のステージのマップ
	MapChipCreate(m_currentMapData, mapSwitchType);
}


ObjectFactory::MapChipType ObjectFactory::GetMapChipType(const Vec2& pos)
{
	// 座標からセルを求める
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	// セルが範囲外ならば、ここで処理を終了する
	if (!FunctionConclusion::IsCellRange(cell, Cell(m_mapInfo.mapWidth, m_mapInfo.mapHeight), Cell(0, 0)))
	{
		return MapChipType::NotExists;
	}

	// マップチップタイプを格納
	const MapChipType mapChipType = MapChipType(m_currentMapData[cell.x][cell.y]);


	// マップチップがセーブだった場合、ゲームデータクラスにセーブ情報を送る
	if (mapChipType == MapChipType::Save)
	{
		// セーブ情報を送る
		m_pGameData->SetSavePointData(GameData::SavePointData(m_stageNumber, cell));
	}


	// マップチップタイプを返す
	return mapChipType;
}

Vec2 ObjectFactory::GetSavePointPos()
{
	// セーブポイントデータを取得
	const GameData::SavePointData savePointData = m_pGameData->GetSavePointData();

	// セーブポイントデータのステージナンバーと現在のステージナンバーが異なる場合、マップ生成を行う
	if (savePointData.stageNumber != m_stageNumber)
	{
		// マップ移動処理
		StageMove(MapSwitchType::Respawn);

		return Vec2();
	}

	// セルから変換した座標を返す
	return FunctionConclusion::CellWithCoordinateToConversion(savePointData.cell, m_mapInfo.chipSize);
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

bool ObjectFactory::IsCellCheckOutOfRange(const Cell& cell)
{
	if (cell.x < 0)return true;
	if (cell.x >= m_mapInfo.mapWidth)return true;
	if (cell.y < 0)return true;
	if (cell.y >= m_mapInfo.mapHeight)return true;

	return false;
}

void ObjectFactory::TestMapDraw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);

	Vec2 pos1;
	Vec2 pos2;


	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// チップ番号の代入
			const int chipNum = m_currentMapData[x][y];

			// 座標の代入
			pos1 = Vec2(x * m_mapInfo.chipSize, y * m_mapInfo.chipSize);
			pos2 = Vec2(pos1.x + m_mapInfo.chipSize, pos1.y + m_mapInfo.chipSize);

			int color = 0x00ff00;

			if (chipNum == 1)
			{
				color = 0xff0000;
			}

			Vec2 stringPos = Vec2(pos1.x + m_mapInfo.chipSize * 0.5f, pos1.y + m_mapInfo.chipSize * 0.5f);

			DrawFormatString(static_cast<int>(stringPos.x), static_cast<int>(stringPos.y), 0xffffff, "%d", chipNum);


			// マップの描画
			DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}