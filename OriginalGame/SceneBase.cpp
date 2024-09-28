#include "SceneBase.h"
#include <DxLib.h>
#include "game.h"
#include "Sound.h"

void SceneBase::SetFadeIn(const int& fadeSpeed, const int& fadeColor)
{
	// フェードステートをフェードインに設定
	m_fade.state = Fade::In;

	// フェードスピードを設定
	m_fade.speed = fadeSpeed;

	// フェードカラーを設定
	m_fade.color = fadeColor;

	// フェードアルファ値を設定
	m_fade.alpha = 255;
}

void SceneBase::SetFadeOut(const int& fadeSpeed, const int& fadeColor)
{
	// フェードがフェードイン中だったら処理をしない
	if (m_fade.state != Fade::InEnd)
	{
		return;
	}

	// フェードステートをフェードアウトに設定
	m_fade.state = Fade::Out;

	// フェードスピードを設定
	m_fade.speed = fadeSpeed;

	// フェードカラーを設定
	m_fade.color = fadeColor;

	// フェードアルファ値を設定
	m_fade.alpha = 0;
}


bool SceneBase::IsFadeOutEnd()
{
	// フェードアウトが完了していたらtrueを返す
	return m_fade.state == Fade::OutEnd;
}

void SceneBase::UpdateFade()
{
	// フェードが完了していたら処理をしない
	if (m_fade.state == Fade::State::InEnd ||
		m_fade.state == Fade::State::OutEnd)
	{
		return;
	}


	if (m_fade.state == Fade::State::In)
	{
		// フェードイン処理
		m_fade.alpha -= m_fade.speed;
		if (m_fade.alpha <= 0)
		{
			m_fade.alpha = 0;
			m_fade.state = Fade::State::InEnd;
		}
	}
	else if (m_fade.state == Fade::State::Out)
	{
		// フェードアウト処理
		m_fade.alpha += m_fade.speed;
		if (m_fade.alpha >= 255)
		{
			m_fade.alpha = 255;
			m_fade.state = Fade::State::OutEnd;
		}
	}
}

void SceneBase::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade.alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fade.color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::FadeOutSound()
{
	// フェードアウト中で無ければ処理をしない
	if (m_fade.state != Fade::Out)
	{
		return;
	}

	// フェードに合わせてBGMの音量を下げる
	Sound::GetInstance()->ScreenFadeBGMStop(GetFadeAlpha());
}