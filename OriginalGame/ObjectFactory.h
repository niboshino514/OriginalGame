#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"
#include "PlatinumLoader.h"
#include <string>

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
		PreviousStage	// 前のステージ
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
	/// マップチップの中心座標を計算
	/// </summary>
	/// <param name="topLeftmapChipPos">マップチップの左上座標</param>
	/// <returns>マップチップの中心座標</returns>
	Vec2 MapChipCenterPos(const Vec2& topLeftmapChipPos);


	/// <summary>
	/// オブジェクト情報を返す
	/// </summary>
	/// <returns>オブジェクト情報</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }

private:


	/// <summary>
	/// マップデータファイルパスの初期設定
	/// </summary>
	void InitMapDataFilePath();


	void TestMapDraw();

private:

	// オブジェクト
	std::list<std::shared_ptr<ObjectBase>>m_object;

	// マップ情報
	PlatinumLoader::MapInfo m_mapInfo;

	// ステージナンバー
	int m_stageNumber;

	// マップデータのファイルパス
	std::vector<std::string> m_mapDataFilePath;


	std::vector<std::vector<int>> testMapData;

	////////////////////
	// クラスポインタ //
	////////////////////

	// プラチナムローダー
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

};