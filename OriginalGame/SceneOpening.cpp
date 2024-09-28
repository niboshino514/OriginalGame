#include "SceneOpening.h"
#include "SceneMain.h"
#include "Sound.h"
#include "OpeningScreen.h"

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 10;

	// フェードのカラー
	constexpr int kFadeColor = 0x000000;
}

SceneOpening::SceneOpening():
	m_pOpeningScreen(std::make_shared<OpeningScreen>())
{
}

SceneOpening::~SceneOpening()
{
}

void SceneOpening::Init()
{
	// フェードイン設定
	SetFadeIn(kFadeSpeed, kFadeColor);

	// オープニングスクリーンに自身のポインタを渡す
	m_pOpeningScreen->SetSceneOpeningPointer(this);

	// 初期化処理
	m_pOpeningScreen->Init();
}

SceneBase* SceneOpening::Update()
{
	// フェード処理
	UpdateFade();

	// フェードアウト時、だんだん音が小さくなるサウンド処理
	FadeOutSound();

	// フェードインが終了していたらシーン遷移する
	if (IsFadeOutEnd())
	{
		// シーン遷移
		return m_nextScene;
	}

	// 更新処理
	m_pOpeningScreen->Update();


	return this;
}

void SceneOpening::Draw()
{
	// 描画処理
	m_pOpeningScreen->Draw();

	// フェード描画
	DrawFade();
}

void SceneOpening::ChangeScene()
{
	// 次のシーンを設定
	m_nextScene = new SceneMain();


	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}

void SceneOpening::FadeOutSound()
{
	// フェードアウト中で無ければ処理をしない
	if (!GetIsFadeOut())
	{
		return;
	}

	// フェードに合わせてBGMの音量を下げる
	Sound::GetInstance()->ScreenFadeBGMStop(GetFadeAlpha());
}
