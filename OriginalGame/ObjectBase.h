#pragma once
#include "Vec2.h"
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
	enum class ObjectID
	{
		Player,					// プレイヤー

		NoneMapChip,			// 侵入不可マップチップ
		NoramalMapChip,			// 通常マップチップ
		ObstacleMapChip,		// 障害物マップチップ
		NextStageMapChip,		// 次のステージに進むマップチップ
		PreviousStageMapChip,	// 前のステージに戻るマップチップ
	};


	// 描画ランク(ランクの数値が小さい程手前に描画される)
	enum class DrawRank
	{
		Rank_1 = 1,
		Rank_2,
		Rank_3,
		Rank_4,
		Rank_5,
		Rank_6,
		RankNum,
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
	/// 描画優先順位を返す
	/// </summary>
	/// <returns>描画優先順位</returns>
	DrawRank GetDrawRank() { return m_drawRank; }

	/// <summary>
	/// 描画優先順位取得
	/// </summary>
	/// <param name="drawRank">描画ランク</param>
	void SetDrawRank(const DrawRank& drawRank) { m_drawRank = drawRank; }


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
	DrawRank m_drawRank;

	////////////////////
	// クラスポインタ //
	////////////////////

	std::shared_ptr<ObjectFactory>m_pObjectFactory;// オブジェクトファクトリー
};