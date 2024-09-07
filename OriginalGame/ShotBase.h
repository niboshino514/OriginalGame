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

	// 地面の当たり判定
	void GroundCollision();

	// 移動
	void Move();

protected:

	// 地面に当たったか
	bool m_isHitGround;
};