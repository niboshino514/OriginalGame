#pragma once
#include "ObjectBase.h"





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


public:
	Player();
	virtual ~Player();


	void Init()override;
	void Update()override;
	void Draw()override;

private:



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
	/// マップ移動チップの当たり判定
	/// </summary>
	void MapMoveChipCollision();

private:

	// 移動量
	Vec2 m_vec;

	// 座標
	Vec2 m_pos;

	// 矩形
	Rect m_rect;

	// 移動矩形
	Rect m_moveRect;


	////////////////////
	/// ジャンプ関連 ///
	////////////////////
	
	// ジャンプ情報
	JumpInfo m_jumpInfo;

};

