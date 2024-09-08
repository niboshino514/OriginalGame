#include "SineCurveShot.h"

namespace
{
	// �ő�t���[��
	constexpr int kMaxFrame = 20;

	constexpr int kSineCurveSpeed = 2;

	constexpr float kMaxSineCurveValue = 50.0f;
}


SineCurveShot::SineCurveShot():
	m_sineCurveData()
{
}

SineCurveShot::~SineCurveShot()
{
}

void SineCurveShot::ShotInit()
{
	// �T�C���J�[�u�̃f�[�^�̏�����
	m_sineCurveData.sineCurrentFrame = 0;
	m_sineCurveData.sineMaxFrame = kMaxFrame;
	m_sineCurveData.sineMaxValue = kMaxSineCurveValue;
}

void SineCurveShot::ShotUpdate()
{
	// �T�C���J�[�u�̃f�[�^�̍X�V
	SineCurveMove();

	// �n�ʂɓ������Ă�����폜�t���O��false�ɂ���
	if (m_isHitGround)
	{
		m_isExlist = false;
	}
}

void SineCurveShot::SineCurveMove()
{
	// �ړ��ʂ����߂�
	{
		// ���W�A�������߂�
		const float rad = EvoLib::Convert::ConvertAngleToRadian(m_shotData.angle);

		// �i�s�����x�N�g�����v�Z
		m_vec.x = cos(rad);
		m_vec.y = sin(rad);

		// �x�N�g���̐��K��
		m_vec = m_vec.normalize();

		// �x�N�g���ɃX�s�[�h���|����
		m_vec *= m_shotData.speed;

		// �x�[�X���W�Ɉړ��ʂ����Z
		m_basePos += m_vec;
	}

	// �T�C���J�[�u�̒l�����߂�
	const Vec2 sineCurve = EvoLib::Calculation::SineCurveAngle<float>(m_sineCurveData, m_shotData.angle);

	// ���W���X�V
	m_pos = m_basePos + sineCurve;
}
