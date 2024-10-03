#include "Effect.h"
#include <vector>
#include "game.h"

namespace ColorData
{
	std::vector<Effect::Color>data =
	{
		Effect::Color{255, 0, 0},
		Effect::Color{0, 255, 0},
		Effect::Color{0, 0, 255},
		Effect::Color{255, 255, 0},
		Effect::Color{0, 255, 255},
		Effect::Color{255, 0, 255},
	};
}

namespace
{
	// 速度の最大値
	constexpr float kSpeedMax = 5.0f;
}


Effect::Effect(): 
	m_effectHandle(0),
	m_isExist(true)
{
}

Effect::~Effect()
{
}

void Effect::Init()
{
	// カラーデータをランダムで取得
	{
		int maxColorIndex = static_cast<int>(ColorData::data.size());
		maxColorIndex-= 1;

		int colorIndex = GetRand(maxColorIndex);
		m_effectData.color = ColorData::data[colorIndex];
	}

	// 座標をランダムで取得
	{
		m_effectData.pos.x = GetRand(Game::kScreenWidth_F);
		m_effectData.pos.y = GetRand(Game::kScreenHeight_F);
	}

	// 速度をランダムで取得
	{
		m_effectData.speed = GetRand(kSpeedMax);

		if (m_effectData.speed < 1.0f)
		{
			m_effectData.speed = 1.0f;
		}
	}

	// 移動ラジアンをランダムで取得
	{
		m_effectData.moveRad = 
			EvoLib::Convert::ConvertAngleToRadian
			(static_cast<float>(GetRand(360)));
	}

	// 画像のサイズをランダムで取得
	{
		m_effectData.graphSize = static_cast<double>(GetRand(10)*0.1f);
	}

	// 画像のラジアンをランダムで取得
	{
		m_effectData.graphRad = 
			EvoLib::Convert::ConvertAngleToRadian
			(static_cast<float>(GetRand(360)));
	}

	// 画像の透明度をランダムで取得
	{
		m_effectData.alpha = GetRand(255);
	}
}

void Effect::Update()
{
	// 座標を移動
	{
		m_effectData.pos.x += cosf(m_effectData.moveRad) * m_effectData.speed;
		m_effectData.pos.y += sinf(m_effectData.moveRad) * m_effectData.speed;
	}

	// 画像の透明度を減少
	{
		m_effectData.alpha -= 1;

		if (m_effectData.alpha < 0)
		{
			m_effectData.alpha = 0;
		}
	}

	// 透明度が0になったら存在しないフラグを立てる
	{
		if (m_effectData.alpha == 0)
		{
			m_isExist = false;
		}
	}

	// 画面外に出たら存在しないフラグを立てる
	{
		if(m_effectData.pos.y < 0.0f)
		{
			m_isExist = false;
		}

		if (m_effectData.pos.y > Game::kScreenHeight_F)
		{
			m_isExist = false;
		}

		if(m_effectData.pos.x > Game::kScreenWidth_F)
		{
			m_isExist = false;
		}

		if (m_effectData.pos.x < 0.0f)
		{
			m_isExist = false;
		}
	}
}

void Effect::Draw()
{
	// 画像の透明度を設定
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_effectData.alpha);
	}

	// 色を設定
	{
		SetDrawBright(m_effectData.color.r, m_effectData.color.g, m_effectData.color.b);
	}

	// 画像を描画
	{
		DrawRotaGraphF
		(
			m_effectData.pos.x,
			m_effectData.pos.y,
			m_effectData.graphSize,
			m_effectData.graphRad,
			m_effectHandle,
			TRUE
		);
	}

	// 色を元に戻す
	{
		SetDrawBright(255, 255, 255);
	}

	// 画像の透明度を元に戻す
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
