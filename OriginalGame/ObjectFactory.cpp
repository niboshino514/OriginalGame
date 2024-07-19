#include "ObjectFactory.h"
#include "Player.h"


#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
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
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>())
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
	// マップデータコピー
	m_currentMapData = mapData;

	// マップチップのハートボックス格納変数
	std::vector<std::vector<Hurtbox>> mapChipHurtbox(m_mapInfo.mapWidth, std::vector<Hurtbox>(m_mapInfo.mapHeight));

	// マップチップの左上座標
	Vec2 topLeftmapChipPos = Vec2();

	// すべてのセルを見る
	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{

			// マップチップタイプに変換
			const MapChipType mapChipType = MapChipType(mapData[x][y]);


			// ハートボックス
			{
				mapChipHurtbox[x][y].centerPos = FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize);
				mapChipHurtbox[x][y].topPos = Vec2(mapChipHurtbox[x][y].centerPos.x, mapChipHurtbox[x][y].centerPos.y - (m_mapInfo.chipSize * 0.5f));
				mapChipHurtbox[x][y].bottomPos = Vec2(mapChipHurtbox[x][y].centerPos.x, mapChipHurtbox[x][y].centerPos.y + (m_mapInfo.chipSize * 0.5f));
				mapChipHurtbox[x][y].leftPos = Vec2(mapChipHurtbox[x][y].centerPos.x - (m_mapInfo.chipSize * 0.5f), mapChipHurtbox[x][y].centerPos.y);
				mapChipHurtbox[x][y].rightPos = Vec2(mapChipHurtbox[x][y].centerPos.x + (m_mapInfo.chipSize * 0.5f), mapChipHurtbox[x][y].centerPos.y);
			}


			if (mapChipType == MapChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// キャラクター生成
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y),m_mapInfo.chipSize));
			}
		}
	}


	// マップチップハートボックスコビー
	m_mapChipHurtbox = mapChipHurtbox;
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
	m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	

	// プラチナムデータ取得
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// マップのレイヤー
	const int mapLayer = 0;

	
	// マップ情報取得
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();


	// 次のステージのマップ
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


ObjectFactory::MapChipType ObjectFactory::MapChipTypeFromCoordinate(const Vec2& pos)
{
	// マップチップ情報からチップタイプを返す
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	// セルが範囲外かどうかを調べ、範囲外ならば処理を終了する
	if (IsCellCheckOutOfRange(cell))
	{
		// 存在しない
		return MapChipType::NotExists;
	}

	// チップタイプを返す
	return MapChipType(m_currentMapData[cell.x][cell.y]);
}

Vec2 ObjectFactory::CorrectionCoordinateValue(const Vec2& pos, const HurtboxDrection& hurtboxDrection)
{
	// 補正座標
	Vec2 correctionPos;


	// マップチップ情報からチップタイプを返す
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	if (hurtboxDrection == HurtboxDrection::Top)
	{
		correctionPos.y = ((m_mapChipHurtbox[cell.x][cell.y].bottomPos.y - pos.y) - 1.0f);
	}
	else if (hurtboxDrection == HurtboxDrection::Bottom)
	{
		correctionPos.y = (m_mapChipHurtbox[cell.x][cell.y].topPos.y - pos.y);
	}
	else if (hurtboxDrection == HurtboxDrection::Left)
	{
		correctionPos.x = ((m_mapChipHurtbox[cell.x][cell.y].rightPos.x - pos.x) - 1.0f);
	}
	else if (hurtboxDrection == HurtboxDrection::Right)
	{
		correctionPos.x = (m_mapChipHurtbox[cell.x][cell.y].leftPos.x - pos.x);
	}


	return correctionPos;
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

			Vec2 stringPos = Vec2(pos1.x + m_mapInfo.chipSize * 0.5, pos1.y + m_mapInfo.chipSize * 0.5);

			DrawFormatString(stringPos.x, stringPos.y, 0xffffff, "%d", chipNum);


			// マップの描画
			DrawBox(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}