#include "SceneEnding.h"
#include "Sound.h"
#include "SceneTitle.h"
#include "EndingScreen.h"
#include "Controller.h"

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 5;

	// フェードのカラー
	constexpr int kFadeColor = 0x000000;
}

SceneEnding::SceneEnding() :
	m_pEndingScreen(std::make_shared<EndingScreen>())
{
}

SceneEnding::~SceneEnding()
{
}

void SceneEnding::Init()
{
	// フェードイン設定
	SetFadeIn(kFadeSpeed, kFadeColor);

	// 操作受付を有効にする
	Controller::GetInstance()->SetAcceptInput(true);

	// エンディングスクリーンに自身のポインタを渡す
	m_pEndingScreen->SetSceneEndingPointer(this);

	// 初期化処理
	m_pEndingScreen->Init();
}

SceneBase* SceneEnding::Update()
{
	// フェード処理
	UpdateFade();

	// フェードアウト時、だんだん音が小さくなるサウンド処理
	FadeOutSound();


	// フェードアウトが終了していたらシーン遷移する
	if (IsFadeOutEnd())
	{
		// シーン遷移
		return m_nextScene;
	}

	// 更新処理
	m_pEndingScreen->Update();

	return this;
}

void SceneEnding::Draw()
{
	// 描画処理
	m_pEndingScreen->Draw();

	// フェード描画(一番下に来るようにする)
	DrawFade();
}

void SceneEnding::ChangeScene(const Scene& nextScene)
{
	// 次のシーンを設定
	switch (nextScene)
	{
	case SceneEnding::Scene::Title:

		// タイトルシーン
		m_nextScene = new SceneTitle();

		break;
	default:
		break;
	}

	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}

