#include "SceneSaveDataSelect.h"

#include "SaveDataSelectScreen.h"

#include "SceneTitle.h"
#include "SceneMain.h"

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 5;

	// フェードのカラー
	constexpr int kFadeColor = 0x000000;
}

SceneSaveDataSelect::SceneSaveDataSelect():
	m_pSaveDataSelectScreen(std::make_shared<SaveDataSelectScreen>())
{
}

SceneSaveDataSelect::~SceneSaveDataSelect()
{
}

void SceneSaveDataSelect::Init()
{
	// フェードイン設定
	SetFadeIn(kFadeSpeed, kFadeColor);

	// セーブデータ選択スクリーンに自身のポインタを渡す
	m_pSaveDataSelectScreen->SetSceneSaveDataSelect(this);
	// 初期化処理
	m_pSaveDataSelectScreen->Init();
}

SceneBase* SceneSaveDataSelect::Update()
{
	// フェード処理
	UpdateFade();


	// フェードインが終了していたらシーン遷移する
	if (IsFadeOutEnd())
	{
		// シーン遷移
		return m_nextScene;
	}

	// 更新処理
	m_pSaveDataSelectScreen->Update();

	return this;
}

void SceneSaveDataSelect::Draw()
{
	// 描画処理
	m_pSaveDataSelectScreen->Draw();

	// フェード描画(一番下に来るようにする)
	DrawFade();
}

void SceneSaveDataSelect::ChangeScene(const NextScene& nextScene)
{
	// 次のシーンを設定
	switch (nextScene)
	{
	case SceneSaveDataSelect::NextScene::Title:
		// タイトルシーンに遷移
		m_nextScene = new SceneTitle();

		break;
	case SceneSaveDataSelect::NextScene::GameMain:
		// ゲームメインシーンに遷移
		m_nextScene = new SceneMain();

		break;
	default:
		break;
	}

	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}
