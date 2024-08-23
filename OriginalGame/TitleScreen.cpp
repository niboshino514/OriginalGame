#include "TitleScreen.h"
#include <DxLib.h>
#include "Game.h"
#include "SceneTitle.h"
#include "Controller.h"


TitleScreen::TitleScreen():
	m_pSceneTitle(nullptr)
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Init()
{
}

void TitleScreen::Update()
{
	// ボタンが押されたらフェードアウトを設定する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::SaveDataSelect);
	}
}

void TitleScreen::Draw()
{
	// 背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xa9a9a9, true);

}
