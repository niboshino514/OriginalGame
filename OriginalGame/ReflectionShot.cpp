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
	// ˆÚ“®
	Move();

	// ”½ŽË‰ñ”‚ªÅ‘å‰ñ”‚É’B‚µ‚Ä‚¢‚½‚çíœƒtƒ‰ƒO‚ðfalse‚É‚·‚é
	if (m_shotData.reflect.count > m_shotData.reflect.maxCount)
	{
		m_isExlist = false;
	}
}