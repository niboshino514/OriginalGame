#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// 移動スピード
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
	// オブジェクトID設定
	m_objectID = ObjectID::Player;
}

void Player::Update()
{
	// 移動処理
	Move();

	// 当たり判定
	Collision();


	// 移動量を座標に代入
	m_circle.centerPos += m_vec;
}

void Player::Draw()
{
	// 円描画
	DrawCircleAA(m_circle.centerPos.x, m_circle.centerPos.y, m_circle.radius, 32, 0xffffff, true);
}

void Player::Move()
{
	// 移動量初期化
	m_vec = Vec2();

	// パッドを使用した移動
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






	for (auto& object : m_pObjectFactory->GetObjectInfo())
	{
		switch (object->GetObjectID())
		{
		case ObjectBase::Player:

			// 自身のIDと調べているIDが同じなら、コンティニューする
			continue;

			break;
		case ObjectBase::NoramalMapChip:

			// 当たっていない場合、移動量を0にする
			if (FunctionConclusion::IsAABBCollisionDetection(m_circle, object->GetSquare()))
			{
				m_vec += Vec2(-kMoveSpeed, -kMoveSpeed);
			}

			break;
		default:
			break;
		}
	}
}
