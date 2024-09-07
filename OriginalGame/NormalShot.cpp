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
	// ˆÚ“®
	Move();

	// “–‚½‚è”»’è
	GroundCollision();

	// ’n–Ê‚É“–‚½‚Á‚Ä‚¢‚½‚çíœƒtƒ‰ƒO‚ğfalse‚É‚·‚é
	if (m_isHitGround)
	{
		m_isExlist = false;
	}
}