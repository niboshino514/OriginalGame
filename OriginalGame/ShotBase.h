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

	// ’n–Ê‚Ì“–‚½‚è”»’è
	void GroundCollision();

	// ˆÚ“®
	void Move();

protected:

	// ’n–Ê‚É“–‚½‚Á‚½‚©
	bool m_isHitGround;
};