#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 10.0f;

	// �T�C�Y
	const Vec2 kSize(20.0f, 30.0f);
}

namespace
{
	// �W�����v��
	constexpr float kJumpPower = 15.0f;

	// �W�����v��
	constexpr int kJumpCountMax = 2;

	// �d�͑�
	constexpr float kGravity = 1.5f;
	// �d�͏�
	constexpr float kSmallGravity = 0.8f;

	// �ő�ړ���
	constexpr float kMaxDirY = 20.0f;
}


Player::Player() :
	m_pos(),
	m_vec(),
	m_moveRect(),
	m_rect(),
	m_jumpInfo()
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

	// �W�����v����
	Jump();

	// �����蔻��
	Collision();

	// ���X�|�[��
	Respawn();
}

void Player::Draw()
{

	DrawFormatString(0, 15 * 1, 0xffffff, "���WX:%f,���WY:%f,", m_pos.x, m_pos.y);

	// �v���C���[�`��
	DrawBoxAA(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
		0xff0000, true);

	// �ړ��ł���ꏊ��`��
	DrawBoxAA(m_moveRect.left, m_moveRect.top, m_moveRect.right, m_moveRect.bottom,
		0x0000ff, false);
}


void Player::Respawn()
{
	// �Z�[�u�|�C���g�Z�����擾����
	if (Pad::IsPress(PAD_INPUT_1))
	{
		m_pos = m_pObjectFactory->GetSavePointPos();
	}
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
}

void Player::Jump()
{
	// �{�^�����������Ƃ��A�W�����v�J�E���g��0�ȏ�Ȃ�΃W�����v�͂�^����
	if (Pad::IsTrigger(PAD_INPUT_10)&&
		m_jumpInfo.jumpCount > 0)
	{
		// �W�����v�t���O��true�ɂ���
		m_jumpInfo.isJump = true;

		// �W�����v�͂�^����
		m_jumpInfo.fallSpeed = -kJumpPower;

		// �W�����v�J�E���g�����炷
		m_jumpInfo.jumpCount--;
	}

	// ���ł����ꍇ�̓{�^���������Ă��邩�ǂ����ŗ����X�s�[�h���ς��
	if (m_jumpInfo.isJump)
	{
		// �{�^���������Ă��邩�ǂ����ŁA�W�����v�͂��ς��
		if (Pad::IsPress(PAD_INPUT_10))
		{
			// �������d�͂�^����
			m_jumpInfo.fallSpeed += kSmallGravity;
		}
		else
		{
			// �傫���d�͂�^����
			m_jumpInfo.fallSpeed += kGravity;
		}
	}
	else
	{
		// �W�����v���Ă��Ȃ�������ʏ�̏d�͂�^����
		m_jumpInfo.fallSpeed += kGravity;
	}

	// �ړ��ʂɗ����X�s�[�h��������
	m_vec.y = m_jumpInfo.fallSpeed;


	// ���ȏ�̑��x�ɂȂ����瑬�x��}����
	if (m_vec.y > kMaxDirY)
	{
		m_vec.y = kMaxDirY;
	}
}

void Player::Collision()
{
	// �n�ʂ̓����蔻��
	GroundCollision();

	// �}�b�v�ړ��`�b�v�̓����蔻��
	MapMoveChipCollision();
}

void Player::GroundCollision()
{
	// ���S���W�����`�����߂�
	m_rect = FunctionConclusion::RectangleCalculation(m_pos, kSize);

	// �ړ��\�͈͂̋�`���擾
	m_moveRect = FunctionConclusion::GetMoveEnableRect
	(m_rect, m_pObjectFactory->GetMapInfo(), m_pObjectFactory->GetCurrentMapData());

	// �擾������`��m_pos�̈ړ��\�͈͂ɕϊ�
	m_moveRect.left += kSize.x * 0.5f;
	m_moveRect.right -= kSize.x * 0.5f;
	m_moveRect.top += kSize.y * 0.5f;
	m_moveRect.bottom -= kSize.y * 0.5f;



	// �ړ��ʂ����W�ɑ��
	m_pos += m_vec;


	if (m_pos.x < m_moveRect.left)
	{
		m_pos.x = m_moveRect.left;
		m_vec.x = 0.0f;
	}
	if (m_pos.x > m_moveRect.right)
	{
		m_pos.x = m_moveRect.right;
		m_vec.x = 0.0f;
	}
	if (m_pos.y < m_moveRect.top)
	{
		m_pos.y = m_moveRect.top;

		// �������x��0.0f�ɂ���
		m_jumpInfo.fallSpeed = 0.0f;

		m_vec.y = 0.0f;
	}
	if (m_pos.y > m_moveRect.bottom)
	{
		// �n�ʂɒ����Ă���̂ŁA�W�����v�t���O��false�ɂ���
		m_jumpInfo.isJump = false;

		// �W�����v�J�E���g�̍ő�l��������
		m_jumpInfo.jumpCount = kJumpCountMax;

		// �������x��0.0f�ɂ���
		m_jumpInfo.fallSpeed = 0.0f;

		m_pos.y = m_moveRect.bottom;
		m_vec.y = 0.0f;
	}
}

void Player::MapMoveChipCollision()
{
	// �O�̍��W
	const Vec2 beforePos = m_pos - m_vec;


	// ���`��Ԑ����v�Z
	const int iinearInterpolationCount =
		FunctionConclusion::IinearInterpolationCountCalculation(beforePos, m_vec, kSize);

	// ���`��ԍ��W���v�Z
	std::vector<Vec2> iinearInterpolationPos =
		FunctionConclusion::IinearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

	// ���`��Ԑ���0�Ȃ�΁A���W�Ɉړ��ʂ𑫂������̂�z��ɓ����
	if (iinearInterpolationCount == 0)
	{
		// �ړ���̍��W��������
		iinearInterpolationPos.push_back(m_pos);
	}


	// ���`�␳��for������
	for (int i = 0; i < static_cast<int>(iinearInterpolationPos.size()); i++)
	{
		// �}�b�v�`�b�v�^�C�v�𒲂ׂ�
		const ObjectFactory::MapChipType mapChipType = m_pObjectFactory->GetMapChipType(iinearInterpolationPos[i]);

		// ���̃X�e�[�W�ɐi��
		if (mapChipType == ObjectFactory::MapChipType::NextStage)
		{
			m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::NextStage);

			return;
		}

		// �O�̃X�e�[�W�ɖ߂�
		if (mapChipType == ObjectFactory::MapChipType::PreviouseStage)
		{
			m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::PreviouseStage);

			return;
		}
	}
}