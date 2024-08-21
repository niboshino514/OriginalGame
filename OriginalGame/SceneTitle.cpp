#include "SceneTitle.h"

#include "TitleScreen.h"

#include "SceneSaveDataSelect.h"
#include "SceneMain.h"

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
}

SceneBase* SceneTitle::Update()
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

void SceneTitle::ChangeScene(const NextScene& nextScene)
{
	// 次のシーンを設定
	switch (nextScene)
	{
	case SceneTitle::NextScene::SaveDataSelect:

		// セーブデータ選択シーン
		m_nextScene = new SceneSaveDataSelect();

		break;
	default:
		break;
	}



	// フェードアウト設定
	SetFadeOut(kFadeSpeed, kFadeColor);
}

