#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"


// マップ
struct Map
{
	// マップ情報
	std::vector<std::vector<int>> mapInfo;
};

class ObjectBase;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:



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
	void MapChipCreate(const Map& mapData);


	/// <summary>
	/// オブジェクト削除
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// 次のステージへ移行
	/// </summary>
	void NextStageToMigration();


	/// <summary>
	/// マップチップの中心座標を計算
	/// </summary>
	/// <param name="topLeftmapChipPos">マップチップの左上座標</param>
	/// <returns>マップチップの中心座標</returns>
	Vec2 MapChipCenterPos(const Vec2& topLeftmapChipPos);


	/// <summary>
	/// マップ切り替え
	/// </summary>
	void MapSwitch();



	/// <summary>
	/// オブジェクト情報を返す
	/// </summary>
	/// <returns>オブジェクト情報</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


private:

	// オブジェクト
	std::list<std::shared_ptr<ObjectBase>>m_object;

	// キャラクター生成座標情報
	std::vector<Vec2>m_characterPos;

	
	// ステージナンバー
	int m_stageNumber;


	std::vector<Map>m_mapInfo_test;



};