#include "NormalShot.h"

NormalShot::NormalShot()
{
}

NormalShot::~NormalShot()
{
}

void NormalShot::ShotInit()
{
}

void NormalShot::Update()
{
	// �ړ�
	Move();

	// �����蔻��
	GroundCollision();

	// �n�ʂɓ������Ă�����폜�t���O��false�ɂ���
	if (m_isHitGround)
	{
		m_isExlist = false;
	}
}