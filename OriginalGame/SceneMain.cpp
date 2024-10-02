#include <DxLib.h>
#include "SceneMain.h"
#include "Vec2.h"
#include "game.h"
#include "Pad.h"
#include "MainScreen.h"
#include "SceneTitle.h"
#include "Sound.h"
#include "SceneEnding.h"
#include "Controller.h"

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 5;

	// フェードのカラー
	constexpr int kFadeColor = 0x000000;
}

SceneMain::SceneMain():
	m_pMainScreen(std::make_shared<MainScreen>())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// 操作受付を有効にする
	Controller::GetInstance()->SetAcceptInput(true);

	// フェードイン設定
	SetFadeIn(kFadeSpeed, kFadeColor);

	// メインスクリーンに自身のポインタを渡す
	m_pMainScreen->SetSceneMain(this);

	// 初期化処理
	m_pMainScreen->Init();
}

SceneBase* SceneMain::Update()
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
	m_pMainScreen->Update();
	

	return this;
}

void SceneMain::Draw()
{
	// 描画処理
	m_pMainScreen->Draw();

	// フェード描画(一番下に来るようにする)
	DrawFade();
}

void SceneMain::ChangeScene(const Scene& nextScene)
{
	// 次のシーンを設定
	switch (nextScene)
	{
	case SceneMain::Scene::Title:

		// タイトルシーン
		m_nextScene = new SceneTitle();
		break;

	case SceneMain::Scene::Ending:

		// エンディングシーン
		m_nextScene = new SceneEnding();
		break;

	default:
		break;
	}

	// 操作受付を無効にする
	Controller::GetInstance()->SetAcceptInput(false);

	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}
