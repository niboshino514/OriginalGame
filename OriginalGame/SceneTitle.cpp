#include "SceneTitle.h"

#include "TitleScreen.h"

#include "SceneMain.h"
#include "Controller.h"
namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 5;

	// フェードのカラー
	constexpr int kFadeColor = 0x000000;
}

SceneTitle::SceneTitle():
	m_pTitleScreen(std::make_shared<TitleScreen>())
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// フェードイン設定
	SetFadeIn(kFadeSpeed, kFadeColor);

	// タイトルスクリーンに自身のポインタを渡す
	m_pTitleScreen->SetSceneTitle(this);
	// 初期化処理
	m_pTitleScreen->Init();

	// 操作受付を有効にする
	Controller::GetInstance()->SetAcceptInput(true);
}

SceneBase* SceneTitle::Update()
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
	m_pTitleScreen->Update();	



	return this;
}

void SceneTitle::Draw()
{
	// 描画処理
	m_pTitleScreen->Draw();

	// フェード描画(一番下に来るようにする)
	DrawFade();
}

void SceneTitle::ChangeScene(const Scene& nextScene)
{
	// 次のシーンを設定
	switch (nextScene)
	{
	case SceneTitle::Scene::GameMain:

		// セーブデータ選択シーン
		m_nextScene = new SceneMain();

		break;
	default:
		break;
	}

	// 操作受付を無効にする
	Controller::GetInstance()->SetAcceptInput(false);

	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}

