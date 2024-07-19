#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 20.0f;

	// �c��
	constexpr float kHurtboxHeght = 15.0f;

	// �c��
	constexpr float kHurtboxWidth = 10.0f;

	// ���蔼�a
	constexpr float kHurtboxRadius = 2.0f;

}


Player::Player() :
	m_vec()
{
}

Player::~Player()
{
}

void Player::Init()
{
	// �I�u�W�F�N�gID�ݒ�
	m_objectID = ObjectID::Player;


	m_pos = m_circle.centerPos;
}

void Player::Update()
{
	// �ړ�����
	Move();

	// �n�[�g�{�b�N�X�X�V����
	m_hurtbox = HurtboxSetting(m_pos);

	// �����蔻��
	Collision();

	// �ړ��ʂ����W�ɑ��
	m_pos += m_vec;
}

void Player::Draw()
{
	

	// �~�`��
	DrawCircleAA(m_pos.x, m_pos.y, m_circle.radius, 32, 0xffffff, true);

	// ���S���W
	DrawCircleAA(m_hurtbox.centerPos.x, m_hurtbox.centerPos.y, kHurtboxRadius, 32, 0xffffff, true);

	// ����W(��)
	DrawCircleAA(m_hurtbox.topPos.x, m_hurtbox.topPos.y, kHurtboxRadius, 32, 0xff0000, true);

	// �����W(��)
	DrawCircleAA(m_hurtbox.bottomPos.x, m_hurtbox.bottomPos.y, kHurtboxRadius, 32, 0x00ff00, true);

	// �����W(��)
	DrawCircleAA(m_hurtbox.leftPos.x, m_hurtbox.leftPos.y, kHurtboxRadius, 32, 0x0000ff, true);

	// �E���W(��)
	DrawCircleAA(m_hurtbox.rightPos.x, m_hurtbox.rightPos.y, kHurtboxRadius, 32, 0xff00ff, true);



	Vec2 pos1;
	Vec2 pos2;

	// ���W�̑��
	pos1 = Vec2(m_pos.x - (kHurtboxHeght * 0.5), m_pos.y - (kHurtboxHeght * 0.5));
	pos2 = Vec2((pos1.x + kHurtboxHeght), (pos1.y + kHurtboxHeght));

	DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, 0x00ff00, false);


}

Hurtbox Player::HurtboxSetting(const Vec2& pos)
{
	// �n�[�g�{�b�N�X
	Hurtbox hurtbox = Hurtbox();

	// ���S���W
	hurtbox.centerPos = pos;

	// ����W
	hurtbox.topPos = Vec2(hurtbox.centerPos.x, hurtbox.centerPos.y - kHurtboxHeght);

	// �����W
	hurtbox.bottomPos = Vec2(hurtbox.centerPos.x, hurtbox.centerPos.y + kHurtboxHeght);

	// �����W
	hurtbox.leftPos = Vec2(hurtbox.centerPos.x - kHurtboxWidth, hurtbox.centerPos.y);

	// �E���W
	hurtbox.rightPos = Vec2(hurtbox.centerPos.x + kHurtboxWidth, hurtbox.centerPos.y);

	return hurtbox;
}



void Player::Move()
{
	// �ړ��ʏ�����
	m_vec = Vec2();

	// �p�b�h���g�p�����ړ�
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_vec.x += kMoveSpeed;
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_vec.x -= kMoveSpeed;

	}
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		m_vec.y -= kMoveSpeed;

	}
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		m_vec.y += kMoveSpeed;
	}
}

void Player::Collision()
{

	const Vec2 afterPos = (m_pos + m_vec);


	const Hurtbox afterHurtbox = HurtboxSetting(m_pos);


	Vec2 add = Vec2();


	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.topPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.topPos, ObjectFactory::HurtboxDrection::Top);

		if (FunctionConclusion::IsValueNegativeCount(m_vec.y))
		{
			m_vec.y = 0.0f;
		}
	}

	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.bottomPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.bottomPos, ObjectFactory::HurtboxDrection::Bottom);

		if (!FunctionConclusion::IsValueNegativeCount(m_vec.y))
		{
			m_vec.y = 0.0f;
		}
	}
	
	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.leftPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.leftPos, ObjectFactory::HurtboxDrection::Left);

		if (FunctionConclusion::IsValueNegativeCount(m_vec.x))
		{
			m_vec.x = 0.0f;
		}
	}


	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.rightPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.rightPos, ObjectFactory::HurtboxDrection::Right);

		if (!FunctionConclusion::IsValueNegativeCount(m_vec.x))
		{
			m_vec.x = 0.0f;
		}
	}

	m_pos += add;
}
