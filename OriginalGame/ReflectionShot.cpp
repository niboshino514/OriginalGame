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
	// 移動
	Move();

	// 反射回数が最大回数に達していたら削除フラグをfalseにする
	if (m_shotData.reflect.count > m_shotData.reflect.maxCount)
	{
		m_isExlist = false;
	}
}