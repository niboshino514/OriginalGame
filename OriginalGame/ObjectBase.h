#pragma once
#include "Vec2.h"
#include <DxLib.h>
#include "EvoLib.h"
#include "ObjectManager.h"
#include <memory>
#include "GameData.h"
#include <vector>

class ObjectManager;

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

		TransparentBlockChip,	// 透明ブロックチップ
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
	
	// アニメーションの種類
	enum class AnimationType
	{
		// 停止状態
		Idle,
		// 移動状態
		Move,
	};


	/// <summary>
	/// アニメーションの詳細を表す構造体
	/// </summary>
	struct AnimationDetails
	{
		// アニメーション番号
		int number = 0;
		// アニメーションの速度
		int frameSpeed = 0;
		// キャラクターが動いているかどうかの処理
		AnimationType type = AnimationType();
		// 向き
		Direction direction[2];
	};

	/// <summary>
	/// アニメーションの範囲を表す構造体
	/// </summary>
	struct AnimationRange
	{
		// 最小インデックス
		int minIndex = 0;
		// 最大インデックス
		int maxIndex = 0;
		// アニメーションを開始する配列番号
		int dirNo = 0;
		// 停止中の配列番号
		int stopNo = 0;
	};



public:

	/// <summary>
	/// オブジェクトファクトリークラスをコピー
	/// </summary>
	/// <param name="objectFactory">オブジェクトファクトリー</param>
	void SetObjectFactory(std::shared_ptr<ObjectManager>objectFactory) { m_pObjectFactory = objectFactory; }

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
	/// 座標を代入
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Vec2& pos) { m_pos = pos; }

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

	/// <summary>
	/// ギミックフラグを取得
	/// </summary>
	/// <param name="isGimmick">ギミックフラグ</param>
	void SetGimiickFlag(const bool& isGimmick) { m_isGimmick = isGimmick; }

	/// <summary>
	/// グラフィックハンドルをセット
	/// </summary>
	/// <param name="graphicHandle">グラフィックハンドル</param>
	void SetGraphicHandle(const std::vector<int>& graphicHandle) { m_graphicHandle = graphicHandle; }

protected:

	/// <summary>
	///  アニメーション番号更新
	/// </summary>
	/// <param name="animationDetails">アニメーション詳細</param>
	/// <param name="animationRange">アニメーション範囲</param>
	/// <returns>アニメーション番号</returns>
	int AnimationNamberUpdate(const AnimationDetails& animationDetails, const AnimationRange& animationRange);


protected:


	// オブジェクトの種類
	ObjectID m_objectID;

	// ギミックフラグ
	bool m_isGimmick;

	// 存在フラグ
	bool m_isExlist;

	// 円情報
	Circle m_circle;

	// 座標
	Vec2 m_pos;

	// 四角形情報
	Square m_square;

	// 描画の優先順位
	DrawRank m_drawRank;

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// 画像ハンドル
	std::vector<int>m_graphicHandle;

	////////////////////////
	// アニメーション関連 //
	////////////////////////

	// アニメーション詳細
	AnimationDetails m_animationDetails;

	// アニメーション範囲
	AnimationRange m_animationRange;
	
	////////////////////
	// クラスポインタ //
	////////////////////

	std::shared_ptr<ObjectManager>m_pObjectFactory;// オブジェクトファクトリー
};