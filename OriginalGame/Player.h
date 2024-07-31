#pragma once
#include "ObjectBase.h"



template <class TState> class StateMachine;

class Player : public ObjectBase
{
public:

	// ジャンプ情報
	struct JumpInfo
	{
		// ジャンプしているかどうか
		bool isJump = false;

		// 落下速度
		float fallSpeed = 0.0f;

		// ジャンプ回数
		int jumpCount = 0;
	};

	// 状態
	enum State
	{
		// 通常状態
		Normal,
		// 死亡
		Dead,
	};

public:
	Player();
	virtual ~Player();


	void Init()override;
	void Update()override;
	void Draw()override;

private:

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	
	// ノーマルステート処理
	void StateNormalEnter();
	void StateNormalUpdate();
	void StateNormalDraw();
	void StateNormalExit();

	



	/// <summary>
	/// リスポーン
	/// </summary>
	void Respawn();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void Jump();

	/// <summary>
	/// 衝突
	/// </summary>
	void Collision();

private:


	/// <summary>
	/// 地面の衝突判定
	/// </summary>
	void GroundCollision();

	/// <summary>
	/// 座標線形補間
	/// </summary>
	void PosLinearInterpolation();

	/// <summary>
	/// マップチップの当たり判定
	/// </summary>
	void MapChipCollision(const Vec2& pos);

	/// <summary>
	/// 障害物の当たり判定
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	/// <param name="pos">プレイヤーの座標</param>
	void ObstacleCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos);

	/// <summary>
	/// マップ移動
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	/// <param name="pos">プレイヤーの座標</param>
	void MapMove(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos);


private:

	// 移動量
	Vec2 m_vec;

	// 矩形
	Rect m_rect;

	// 移動矩形
	Rect m_moveRect;



	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

	////////////////////
	/// ジャンプ関連 ///
	////////////////////
	
	// ジャンプ情報
	JumpInfo m_jumpInfo;

};

