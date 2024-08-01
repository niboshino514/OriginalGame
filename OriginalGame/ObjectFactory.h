#pragma once

#include "StateMachine.h"
#include <list>
#include <memory>
#include <vector>
#include <string>
#include "Vec2.h"
#include "PlatinumLoader.h"


#include "EvoLib.h"

#include <tuple>

class ObjectBase;
class PlatinumLoader;
class Camera;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:

	// ステージ移動タイプ
	enum class MapSwitchType
	{
		Spawn,			// スポーン
		Respawn,		// リスポーン
		NextStage,		// 次のステージ
		PreviouseStage	// 前のステージ
	};


	// チップタイプ
	enum class ChipType
	{
		None,			// 何もなし
		Ground,			// 地面
		NextStage,		// 次のステージ
		PreviouseStage,	// 前のステージ
		NextPos,		// 次のステージ座標
		PreviousePos,	// 前のステージ座標
		SpawnPos,		// スポーン座標
		Save,			// セーブ

		TopNeedle,		// 上針
		BottomNeedle,	// 下針
		LeftNeedle,		// 左針
		RightNeedle,	// 右針
		DiedBlock,		// 死亡ブロック
		
		BottomGravity,	// 下重力
		TopGravity,		// 上重力
		LeftGravity,	// 左重力
		RightGravity,	// 右重力

		NotExists		// 存在しない
		
	};



	// マップ判定データ
	struct MapCollisionData
	{
		// チップタイプ
		ChipType chipType = ChipType::None;

		// マップ四角形情報
		Square square = Square();

		// マップ円情報
		Circle circle = Circle();

		// 画面内フラグ
		bool screenFlag = false;
	};

	// マップ情報データ
	struct MapInfoData
	{
		// マップデータのファイルパス
		std::vector<std::string> filePath;

		// マップ判定データ
		std::vector<std::vector<MapCollisionData>> mapCollisionData;

		// マップ情報
		PlatinumLoader::MapChip mapChip;
		
		// マップナンバー
		int mapNumber = 0;
	};



public:
	ObjectFactory();
	virtual ~ObjectFactory();

	void Init();
	void Update();
	void Draw();

public:

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
	/// マップ情報データを返す
	/// </summary>
	/// <returns>マップ情報データ</returns>
	MapInfoData GetMapInfoData() { return m_mapInfoData; }
	
	/// <summary>
	/// マップチップ番号を返す
	/// </summary>
	/// <returns></returns>
	std::vector<std::vector<int>>GetMapChipNumber();

	/// <summary>
	/// セーブポイントを設定する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetSavePoint(const Vec2& pos);

	/// <summary>
	/// セーブポイントの座標を返す
	/// </summary>
	/// <returns>bool : ステージを変更するかどうか　Vec2 : 座標</returns>
	std::tuple<bool, Vec2> GetSavePointPos();

	/// <summary>
	/// ニードル情報を返す
	/// </summary>
	/// <param name="needleDirection">ニードルの向き</param>
	/// <param name="square">四角形情報</param>
	/// <returns>ニードル情報</returns>
	Triangle ChipTypeToTriangle(const ChipType& needleDirection, const Square& square);

	/// <summary>
	/// スクリーン内かどうかを調べる
	/// </summary>
	void ScreenCheck();

	/// <summary>
	/// マップ当たり判定データを返す
	/// </summary>
	std::vector<std::vector<MapCollisionData>> GetMapCollisionData() { return m_mapInfoData.mapCollisionData; }


private:

	/// <summary>
	/// マップ関連初期設定
	/// </summary>
	void InitMap();
	
	/// <summary>
	/// スクリーンサークル初期化
	/// </summary>
	void InitScreenCircle();



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
	std::list<std::shared_ptr<ObjectBase>>m_object;

	// マップ情報データ
	MapInfoData m_mapInfoData;

	// スクリーンサークル
	Circle m_screenCircle;

	// マップグラフィック
	std::vector<int>m_testMapGraph;

	////////////////////
	// クラスポインタ //
	////////////////////

	// プラチナムローダー
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

	// カメラ
	std::shared_ptr<Camera>m_pCamera;
};