#include "ReflectionShot.h"





ReflectionShot::ReflectionShot()
{
}

ReflectionShot::~ReflectionShot()
{
}

void ReflectionShot::ShotInit()
{

}

void ReflectionShot::ShotUpdate()
{
	// �ړ�
	Move();

	// ���ˉ񐔂��ő�񐔂ɒB���Ă�����폜�t���O��false�ɂ���
	if (m_shotData.reflect.count > m_shotData.reflect.maxCount)
	{
		m_isExlist = false;
	}
}