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

	// �n�ʔ��菉����
	void GroundCollisionInit();

	// �V���b�g�̏�����
	virtual void ShotInit() = 0;



	// �n�ʂ̓����蔻��
	void GroundCollision();

	// �ړ�
	void Move();

protected:

	// �x�[�X���W
	Vec2 m_basePos;

	// �ړ���`
	Rect m_moveRect;


	// �n�ʂɓ���������
	bool m_isHitGround;


};