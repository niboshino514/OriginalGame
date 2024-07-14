#pragma once
#include <Vec2.h>
#include <DxLib.h>
#include "FunctionConclusion.h"
#include "ObjectFactory.h"
#include <memory>


class ObjectFactory;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// オブジェクトID
	enum ObjectID
	{
		Player,					// プレイヤー

		NoneMapChip,			// 侵入不可マップチップ
		NoramalMapChip,			// 通常マップチップ
		ObstacleMapChip,		// 障害物マップチップ
		NextStageMapChip,		// 次のステージに進むマップチップ
		PreviousStageMapChip,	// 前のステージに戻るマップチップ

	};


	/// <summary>
	/// オブジェクトファクトリークラスをコピー
	/// </summary>
	/// <param name="objectFactory">オブジェクトファクトリー</param>
	void SetObjectFactory(std::shared_ptr<ObjectFactory>objectFactory) { m_pObjectFactory = objectFactory; }

	/// <summary>
	/// 存在フラグ代入
	/// </summary>
	/// <param name="isExlist">存在フラグ</param>
	void SetIsExlist(const bool& isExlist) { m_isExlist = isExlist; }

	/// <summary>
	/// 存在フラグを返す
	/// </summary>
	/// <returns>存在フラグ</returns>
	bool GetIsExlist() { return m_isExlist; }

	


	/// <summary>
	/// 円情報代入
	/// </summary>
	/// <param name="circle">円情報</param>
	void SetCircle(const Circle& circle) { m_circle = circle; }

	/// <summary>
	/// 円情報を返す
	/// </summary>
	/// <returns>円情報</returns>
	Circle GetCircle() { return m_circle; }


	/// <summary>
	/// 四角形情報を代入
	/// </summary>
	/// <param name="square">四角形情報</param>
	void SetSquare(const Square& square) { m_square = square; }

	/// <summary>
	/// 四角形情報を返す
	/// </summary>
	/// <returns>四角形情報</returns>
	Square GetSquare() { return m_square; }


	/// <summary>
	/// オブジェクトIDを返す
	/// </summary>
	/// <returns>オブジェクトID</returns>
	ObjectID GetObjectID() { return m_objectID; }


	/// <summary>
	/// 描画優先順位取得
	/// </summary>
	/// <returns>描画優先順位</returns>
	int GetDrawRank() { return m_drawRank; }


protected:


	// オブジェクトの種類
	ObjectID m_objectID;


	// 存在フラグ
	bool m_isExlist;

	// 円情報
	Circle m_circle;

	// 四角形情報
	Square m_square;

	// 描画の優先順位
	int m_drawRank;

	////////////////////
	// クラスポインタ //
	////////////////////

	std::shared_ptr<ObjectFactory>m_pObjectFactory;// オブジェクトファクトリーs

};