#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"
#include "PlatinumLoader.h"
#include <string>
#include "FunctionConclusion.h"


class ObjectBase;
class PlatinumLoader;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:

	// ステージ移動タイプ
	enum class MapSwitchType
	{
		Spawn,			// スポーン
		NextStage,		// 次のステージ
		PreviouseStage	// 前のステージ
	};


	// マップチップタイプ
	enum class MapChipType
	{
		None,			// 何もなし
		Ground,			// 地面
		NextStage,		// 次のステージ
		PreviouseStage,	// 前のステージ
		NextPos,		// 次のステージ座標
		PreviousePos,	// 前のステージ座標
		SpawnPos,		// スポーン座標
		NotExists		// 存在しない
	};

	// ハートボックスの向き
	enum class HurtboxDrection
	{
		Top,	// 上
		Bottom,	// 下
		Left,	// 左
		Right,	// 右
	};

public:
	ObjectFactory();
	virtual ~ObjectFactory();

	void Init();
	void Update();
	void Draw();


	/// <summary>
	/// キャラクター生成
	/// </summary>
	void CharacterCreate(const Vec2& pos);

	/// <summary>
	/// マップ生成
	/// </summary>
	/// <param name="マップ情報"></param>
	void MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType);


	/// <summary>
	/// オブジェクト削除
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// ステージ移動
	/// </summary>
	void StageMove(const MapSwitchType& mapSwitchType);


	/// <summary>
	/// オブジェクト情報を返す
	/// </summary>
	/// <returns>オブジェクト情報</returns>
	std::vector<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


	/// <summary>
	/// 現在のマップデータを返す
	/// </summary>
	/// <returns></returns>
	PlatinumLoader::MapData GetCurrentMapData() { return PlatinumLoader::MapData(m_currentMapData); }


	/// <summary>
	/// マップ情報を返す
	/// </summary>
	/// <returns>マップ情報</returns>
	PlatinumLoader::MapInfo GetMapInfo() { return m_mapInfo; }
	

	/// <summary>
	/// マップチップ情報を返す
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>マップチップ情報</returns>
	MapChipType GetMapChipType(const Vec2& pos);

	
private:


	/// <summary>
	/// マップデータファイルパスの初期設定
	/// </summary>
	void InitMapDataFilePath();


	/// <summary>
	/// セルが範囲外かどうかを確認する
	/// </summary>
	/// <param name="cell">セル</param>
	/// <returns>セルが範囲外かどうかのフラグ</returns>
	bool IsCellCheckOutOfRange(const Cell& cell);


	/// <summary>
	/// マップ描画
	/// </summary>
	void TestMapDraw();

private:



	// オブジェクト
	std::vector<std::shared_ptr<ObjectBase>>m_object;

	// マップ情報
	PlatinumLoader::MapInfo m_mapInfo;

	// ステージナンバー
	int m_stageNumber;

	// マップデータのファイルパス
	std::vector<std::string> m_mapDataFilePath;

	// 現在のマップデータ
	std::vector<std::vector<int>> m_currentMapData;



	////////////////////
	// クラスポインタ //
	////////////////////

	// プラチナムローダー
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

};