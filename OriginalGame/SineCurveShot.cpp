#include "SineCurveShot.h"

namespace
{
	// 最大フレーム
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
	// サインカーブのデータの初期化
	m_sineCurveData.sineCurrentFrame = 0;
	m_sineCurveData.sineMaxFrame = kMaxFrame;
	m_sineCurveData.sineMaxValue = kMaxSineCurveValue;
}

void SineCurveShot::ShotUpdate()
{
	// サインカーブのデータの更新
	SineCurveMove();

	// 地面に当たっていたら削除フラグをfalseにする
	if (m_isHitGround)
	{
		m_isExlist = false;
	}
}

void SineCurveShot::SineCurveMove()
{
	// 移動量を求める
	{
		// ラジアンを求める
		const float rad = EvoLib::Convert::ConvertAngleToRadian(m_shotData.angle);

		// 進行方向ベクトルを計算
		m_vec.x = cos(rad);
		m_vec.y = sin(rad);

		// ベクトルの正規化
		m_vec = m_vec.normalize();

		// ベクトルにスピードを掛ける
		m_vec *= m_shotData.speed;

		// ベース座標に移動量を加算
		m_basePos += m_vec;
	}

	// サインカーブの値を求める
	const Vec2 sineCurve = EvoLib::Calculation::SineCurveAngle<float>(m_sineCurveData, m_shotData.angle);

	// 座標を更新
	m_pos = m_basePos + sineCurve;
}
