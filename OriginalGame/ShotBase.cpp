#include "ShotBase.h"
#include "EvoLib.h"

namespace
{
	// �V���b�g�̃X�s�[�h
	constexpr float kShotSpeed = 10.0f;
}

namespace Size
{
	// �V���b�g���a�T�C�Y
	constexpr float kShotRadius = 5.0f;
}


ShotBase::ShotBase():
	m_isHitGround(false),
	m_moveRect()
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Init()
{
	// �n�ʔ��菉����
	GroundCollisionInit();


	// �V���b�g�̏�����
	ShotInit();
}

void ShotBase::Update()
{
	// �V���b�g�̈ړ�
	m_pos.x += kShotSpeed;

	// �����蔻��
	GroundCollision();
}

void ShotBase::Draw()
{
	// �I�t�Z�b�g�l���擾
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// �V���b�g�̕`��
	DrawCircleAA(m_pos.x + offset.x, m_pos.y + offset.y, Size::kShotRadius, 5, GetColor(255, 255, 255), TRUE);
}

void ShotBase::GroundCollisionInit()
{



	// �}�b�v�`�b�v�̃T�C�Y���擾
	float mapChipSize = m_pObjectManager->GetMapInfoData().mapChip.chipSize;

	// �}�b�v�`�b�v�̍ő�Z�����擾
	Cell maxCell = Cell(m_pObjectManager->GetMapInfoData().mapChip.mapWidth, m_pObjectManager->GetMapInfoData().mapChip.mapHeight);


	// �n�ʃZ���ԍ�
	std::vector<int>groundCellNumber;
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::Ground));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::IceBlock));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TopConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::BottomConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::LeftConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::RigthConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TransparentBlock));


	// �~�̒��a
	const Vec2 diameter = Vec2(Size::kShotRadius * 2.0f, Size::kShotRadius * 2.0f);

	// �V���b�g��`
	const Rect shotRect = EvoLib::Convert::PosToRect(m_pos, diameter);

	// �ړ��\�͈͂̋�`���擾
	m_moveRect = EvoLib::Calculation::CalculateRectangleMovementRange
	(shotRect, maxCell, mapChipSize, m_pObjectManager->GetMapChipNumber(), groundCellNumber);

}

void ShotBase::GroundCollision()
{

#if true
	// �I�t�Z�b�g�l���擾
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// �ړ��\�͈͂̋�`���擾
	Rect drawMoveRect = Rect();

	drawMoveRect.left = m_moveRect.left + offset.x;
	drawMoveRect.right = m_moveRect.right + offset.x;
	drawMoveRect.bottom = m_moveRect.bottom + offset.y;
	drawMoveRect.top = m_moveRect.top + offset.y;

	// �ړ��ł���ꏊ��`��
	DrawBoxAA(drawMoveRect.left, drawMoveRect.top, drawMoveRect.right, drawMoveRect.bottom,
		0x0000ff, true);
#endif


	// �@���x�N�g��
	Vec2 normal = Vec2();


	const Vec2 leftTop = Vec2(m_moveRect.left, m_moveRect.top);
	const Vec2 rightTop = Vec2(m_moveRect.right, m_moveRect.top);
	const Vec2 leftBottom = Vec2(m_moveRect.left, m_moveRect.bottom);
	const Vec2 rightBottom = Vec2(m_moveRect.right, m_moveRect.bottom);

	Vec2 temp[2] = { Vec2(), Vec2() };

	// �ړ���̍��W
	Vec2 afterPos = m_pos + m_vec;


	// ���[�ɓ��������ꍇ
	if (afterPos.x <= m_moveRect.left)
	{
		temp[0] = leftTop;
		temp[1] = leftBottom;

	}
	else if(afterPos.x >= m_moveRect.right)
	{
		temp[0] = rightTop;
		temp[1] = rightBottom;
	}
	else if(afterPos.y <= m_moveRect.top)
	{
		temp[0] = leftTop;
		temp[1] = rightTop;
	}
	else if(afterPos.y >= m_moveRect.bottom)
	{
		temp[0] = leftBottom;
		temp[1] = rightBottom;
	}
	else
	{
		return;
	}


	// �ǂɓ�������
	//m_isHitGround = true;

	// ���˃x�N�g�����v�Z
	const Vec2 reflection = EvoLib::Calculation::ReflectVector(m_vec, temp[0], temp[1]);

	// ���˃x�N�g�������W�A���ϊ�
	float rad = atan2(reflection.y, reflection.x);

	// ���W�A�����p�x�ɕϊ�
	m_shotData.angle = EvoLib::Convert::ConvertRadianToAngle(rad);




}

void ShotBase::Move()
{
	
	// ���W�A�������߂�
	const float rad = EvoLib::Convert::ConvertAngleToRadian(m_shotData.angle);

	// ���W�A������x�N�g�������߂�
	m_vec.x = cos(rad);
	m_vec.y = sin(rad);

	// �x�N�g���̐��K��
	m_vec = m_vec.normalize();

	// �x�N�g���ɃX�s�[�h���|����
	m_vec *= m_shotData.speed;

	// �x�[�X���W�Ɉړ��ʂ����Z
	m_basePos += m_vec;

	// ���W���X�V
	m_pos = m_basePos;
}
