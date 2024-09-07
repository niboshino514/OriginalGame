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