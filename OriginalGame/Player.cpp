#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// 移動スピード
	constexpr float kMoveSpeed = 20.0f;

	// 縦幅
	constexpr float kHurtboxHeght = 15.0f;

	// 縦幅
	constexpr float kHurtboxWidth = 10.0f;

	// 判定半径
	constexpr float kHurtboxRadius = 2.0f;

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


	m_pos = m_circle.centerPos;
}

void Player::Update()
{
	// 移動処理
	Move();

	// ハートボックス更新処理
	m_hurtbox = HurtboxSetting(m_pos);

	// 当たり判定
	Collision();

	// 移動量を座標に代入
	m_pos += m_vec;
}

void Player::Draw()
{
	

	// 円描画
	DrawCircleAA(m_pos.x, m_pos.y, m_circle.radius, 32, 0xffffff, true);

	// 中心座標
	DrawCircleAA(m_hurtbox.centerPos.x, m_hurtbox.centerPos.y, kHurtboxRadius, 32, 0xffffff, true);

	// 上座標(赤)
	DrawCircleAA(m_hurtbox.topPos.x, m_hurtbox.topPos.y, kHurtboxRadius, 32, 0xff0000, true);

	// 下座標(緑)
	DrawCircleAA(m_hurtbox.bottomPos.x, m_hurtbox.bottomPos.y, kHurtboxRadius, 32, 0x00ff00, true);

	// 左座標(青)
	DrawCircleAA(m_hurtbox.leftPos.x, m_hurtbox.leftPos.y, kHurtboxRadius, 32, 0x0000ff, true);

	// 右座標(紫)
	DrawCircleAA(m_hurtbox.rightPos.x, m_hurtbox.rightPos.y, kHurtboxRadius, 32, 0xff00ff, true);



	Vec2 pos1;
	Vec2 pos2;

	// 座標の代入
	pos1 = Vec2(m_pos.x - (kHurtboxHeght * 0.5), m_pos.y - (kHurtboxHeght * 0.5));
	pos2 = Vec2((pos1.x + kHurtboxHeght), (pos1.y + kHurtboxHeght));

	DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, 0x00ff00, false);


}

Hurtbox Player::HurtboxSetting(const Vec2& pos)
{
	// ハートボックス
	Hurtbox hurtbox = Hurtbox();

	// 中心座標
	hurtbox.centerPos = pos;

	// 上座標
	hurtbox.topPos = Vec2(hurtbox.centerPos.x, hurtbox.centerPos.y - kHurtboxHeght);

	// 下座標
	hurtbox.bottomPos = Vec2(hurtbox.centerPos.x, hurtbox.centerPos.y + kHurtboxHeght);

	// 左座標
	hurtbox.leftPos = Vec2(hurtbox.centerPos.x - kHurtboxWidth, hurtbox.centerPos.y);

	// 右座標
	hurtbox.rightPos = Vec2(hurtbox.centerPos.x + kHurtboxWidth, hurtbox.centerPos.y);

	return hurtbox;
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

	const Vec2 afterPos = (m_pos + m_vec);


	const Hurtbox afterHurtbox = HurtboxSetting(m_pos);


	Vec2 add = Vec2();


	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.topPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.topPos, ObjectFactory::HurtboxDrection::Top);

		if (FunctionConclusion::IsValueNegativeCount(m_vec.y))
		{
			m_vec.y = 0.0f;
		}
	}

	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.bottomPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.bottomPos, ObjectFactory::HurtboxDrection::Bottom);

		if (!FunctionConclusion::IsValueNegativeCount(m_vec.y))
		{
			m_vec.y = 0.0f;
		}
	}
	
	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.leftPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.leftPos, ObjectFactory::HurtboxDrection::Left);

		if (FunctionConclusion::IsValueNegativeCount(m_vec.x))
		{
			m_vec.x = 0.0f;
		}
	}


	if (ObjectFactory::MapChipType::Ground ==
		m_pObjectFactory->MapChipTypeFromCoordinate(afterHurtbox.rightPos))
	{
		add +=
			m_pObjectFactory->CorrectionCoordinateValue(afterHurtbox.rightPos, ObjectFactory::HurtboxDrection::Right);

		if (!FunctionConclusion::IsValueNegativeCount(m_vec.x))
		{
			m_vec.x = 0.0f;
		}
	}

	m_pos += add;
}
