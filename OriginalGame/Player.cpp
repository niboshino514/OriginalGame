#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 5.0f;
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
}

void Player::Update()
{
	// �ړ�����
	Move();

	// �����蔻��
	Collision();


	// �ړ��ʂ����W�ɑ��
	m_circle.centerPos += m_vec;
}

void Player::Draw()
{
	// �~�`��
	DrawCircleAA(m_circle.centerPos.x, m_circle.centerPos.y, m_circle.radius, 32, 0xffffff, true);
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

	Vec2 afterPos = (m_circle.centerPos + m_vec);

	Circle afterCircle = m_circle;
	afterCircle.centerPos = afterPos;



	for (auto& object : m_pObjectFactory->GetObjectInfo())
	{
		switch (object->GetObjectID())
		{
		case ObjectBase::Player:
			break;
		case ObjectBase::NoneMapChip:

			// �������Ă����ꍇ�A�ړ��ʂ�0�ɂ���
			if (FunctionConclusion::IsAABBCollisionDetection(afterCircle, object->GetSquare()))
			{
				m_vec = Vec2();
			}
			break;
		case ObjectBase::ObstacleMapChip:

			// �������Ă����ꍇ�A�ړ��ʂ�0�ɂ���
			if (FunctionConclusion::IsAABBCollisionDetection(afterCircle, object->GetSquare()))
			{
				m_vec = Vec2();
			}
			break;
		default:
			break;
		}
	}
}
