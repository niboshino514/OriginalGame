#pragma once
#include "ObjectBase.h"





class Player : public ObjectBase
{
public:
	Player();
	virtual ~Player();


	void Init()override;
	void Update()override;
	void Draw()override;

private:

	/// <summary>
	/// ハートボックス更新処理
	/// </summary>
	Hurtbox HurtboxSetting(const Vec2& pos);

	


	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 衝突
	/// </summary>
	void Collision();

private:

	// 移動量
	Vec2 m_vec;

	// 座標
	Vec2 m_pos;


};

