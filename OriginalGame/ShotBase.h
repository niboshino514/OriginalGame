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

	// �n�ʂ̓����蔻��
	void GroundCollision();

	// �ړ�
	void Move();

protected:

	// �n�ʂɓ���������
	bool m_isHitGround;
};