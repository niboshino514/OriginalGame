#include "PlayerShot.h"

PlayerShot::PlayerShot()
{
}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Init()
{
	m_objectID = ObjectID::PlayerShot;
}

void PlayerShot::Update()
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