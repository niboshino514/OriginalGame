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
	// 移動
	Move();

	// 当たり判定
	GroundCollision();

	// 地面に当たっていたら削除フラグをfalseにする
	if (m_isHitGround)
	{
		m_isExlist = false;
	}
}