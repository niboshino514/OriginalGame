#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"

namespace
{
	// 移動スピード
	constexpr float kMoveSpeed = 10.0f;

	// サイズ
	const Vec2 kSize(20.0f, 30.0f);
}

namespace
{
	// ジャンプ力
	constexpr float kJumpPower = 15.0f;

	// ジャンプ回数
	constexpr int kJumpCountMax = 2;

	// 重力大
	constexpr float kGravity = 1.5f;
	// 重力小
	constexpr float kSmallGravity = 0.8f;

	// 最大移動量
	constexpr float kMaxDirY = 20.0f;
}


Player::Player() :
	m_pos(),
	m_vec(),
	m_moveRect(),
	m_rect(),
	m_jumpInfo()
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

	// ジャンプ処理
	Jump();

	// 当たり判定
	Collision();

	// リスポーン
	Respawn();
}

void Player::Draw()
{

	DrawFormatString(0, 15 * 1, 0xffffff, "座標X:%f,座標Y:%f,", m_pos.x, m_pos.y);

	// プレイヤー描画
	DrawBoxAA(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
		0xff0000, true);

	// 移動できる場所を描画
	DrawBoxAA(m_moveRect.left, m_moveRect.top, m_moveRect.right, m_moveRect.bottom,
		0x0000ff, false);
}


void Player::Respawn()
{
	// セーブポイントセルを取得する
	if (Pad::IsPress(PAD_INPUT_1))
	{
		m_pos = m_pObjectFactory->GetSavePointPos();
	}
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
}

void Player::Jump()
{
	// ボタンを押したとき、ジャンプカウントが0以上ならばジャンプ力を与える
	if (Pad::IsTrigger(PAD_INPUT_10)&&
		m_jumpInfo.jumpCount > 0)
	{
		// ジャンプフラグをtrueにする
		m_jumpInfo.isJump = true;

		// ジャンプ力を与える
		m_jumpInfo.fallSpeed = -kJumpPower;

		// ジャンプカウントを減らす
		m_jumpInfo.jumpCount--;
	}

	// 飛んでいた場合はボタンを押しているかどうかで落下スピードが変わる
	if (m_jumpInfo.isJump)
	{
		// ボタンを押しているかどうかで、ジャンプ力が変わる
		if (Pad::IsPress(PAD_INPUT_10))
		{
			// 小さい重力を与える
			m_jumpInfo.fallSpeed += kSmallGravity;
		}
		else
		{
			// 大きい重力を与える
			m_jumpInfo.fallSpeed += kGravity;
		}
	}
	else
	{
		// ジャンプしていなかったら通常の重力を与える
		m_jumpInfo.fallSpeed += kGravity;
	}

	// 移動量に落下スピードを代入する
	m_vec.y = m_jumpInfo.fallSpeed;


	// 一定以上の速度になったら速度を抑える
	if (m_vec.y > kMaxDirY)
	{
		m_vec.y = kMaxDirY;
	}
}

void Player::Collision()
{
	// 地面の当たり判定
	GroundCollision();

	// マップ移動チップの当たり判定
	MapMoveChipCollision();
}

void Player::GroundCollision()
{
	// 中心座標から矩形を求める
	m_rect = FunctionConclusion::RectangleCalculation(m_pos, kSize);

	// 移動可能範囲の矩形を取得
	m_moveRect = FunctionConclusion::GetMoveEnableRect
	(m_rect, m_pObjectFactory->GetMapInfo(), m_pObjectFactory->GetCurrentMapData());

	// 取得した矩形をm_posの移動可能範囲に変換
	m_moveRect.left += kSize.x * 0.5f;
	m_moveRect.right -= kSize.x * 0.5f;
	m_moveRect.top += kSize.y * 0.5f;
	m_moveRect.bottom -= kSize.y * 0.5f;



	// 移動量を座標に代入
	m_pos += m_vec;


	if (m_pos.x < m_moveRect.left)
	{
		m_pos.x = m_moveRect.left;
		m_vec.x = 0.0f;
	}
	if (m_pos.x > m_moveRect.right)
	{
		m_pos.x = m_moveRect.right;
		m_vec.x = 0.0f;
	}
	if (m_pos.y < m_moveRect.top)
	{
		m_pos.y = m_moveRect.top;

		// 落下速度を0.0fにする
		m_jumpInfo.fallSpeed = 0.0f;

		m_vec.y = 0.0f;
	}
	if (m_pos.y > m_moveRect.bottom)
	{
		// 地面に着いているので、ジャンプフラグをfalseにする
		m_jumpInfo.isJump = false;

		// ジャンプカウントの最大値を代入する
		m_jumpInfo.jumpCount = kJumpCountMax;

		// 落下速度を0.0fにする
		m_jumpInfo.fallSpeed = 0.0f;

		m_pos.y = m_moveRect.bottom;
		m_vec.y = 0.0f;
	}
}

void Player::MapMoveChipCollision()
{
	// 前の座標
	const Vec2 beforePos = m_pos - m_vec;


	// 線形補間数を計算
	const int iinearInterpolationCount =
		FunctionConclusion::IinearInterpolationCountCalculation(beforePos, m_vec, kSize);

	// 線形補間座標を計算
	std::vector<Vec2> iinearInterpolationPos =
		FunctionConclusion::IinearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

	// 線形補間数が0ならば、座標に移動量を足したものを配列に入れる
	if (iinearInterpolationCount == 0)
	{
		// 移動後の座標を代入する
		iinearInterpolationPos.push_back(m_pos);
	}


	// 線形補正分for分を回す
	for (int i = 0; i < static_cast<int>(iinearInterpolationPos.size()); i++)
	{
		// マップチップタイプを調べる
		const ObjectFactory::MapChipType mapChipType = m_pObjectFactory->GetMapChipType(iinearInterpolationPos[i]);

		// 次のステージに進む
		if (mapChipType == ObjectFactory::MapChipType::NextStage)
		{
			m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::NextStage);

			return;
		}

		// 前のステージに戻る
		if (mapChipType == ObjectFactory::MapChipType::PreviouseStage)
		{
			m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::PreviouseStage);

			return;
		}
	}
}