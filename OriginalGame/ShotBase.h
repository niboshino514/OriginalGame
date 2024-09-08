#pragma once
#include "ObjectBase.h"


class ShotBase : public ObjectBase
{
public:
	ShotBase();
	virtual ~ShotBase();

	void Init() override;
	void Update() override;
	void Draw() override;

protected:

	// 地面判定初期化
	void GroundCollisionInit();

	// ショットの初期化
	virtual void ShotInit() = 0;

	// ショットの更新
	virtual void ShotUpdate();

	// 移動
	void Move();

	// 地面の当たり判定
	void GroundCollision();


protected:

	// ベース座標
	Vec2 m_basePos;

	// 移動矩形
	Rect m_moveRect;


	// 地面に当たったか
	bool m_isHitGround;


};