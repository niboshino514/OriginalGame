#pragma once
#include <list>
#include <memory>
#include <vector>



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
	void CharacterCreate();

	/// <summary>
	/// マップ生成
	/// </summary>
	/// <param name="マップ情報"></param>
	void MapChipCreate(const std::vector<std::vector<int>>& mapData);



	/// <summary>
	/// オブジェクト削除
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// 次のステージへ移行
	/// </summary>
	void NextStageToMigration();


	/// <summary>
	/// オブジェクト情報を返す
	/// </summary>
	/// <returns>オブジェクト情報</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


private:

	// オブジェクト
	std::list<std::shared_ptr<ObjectBase>>m_object;

};