#include "SaveDataSelectScreen.h"
#include <DxLib.h>
#include "Game.h"
#include "SceneSaveDataSelect.h"
#include "Controller.h"


SaveDataSelectScreen::SaveDataSelectScreen():
	m_pSceneSaveDataSelect(nullptr)
{
}

SaveDataSelectScreen::~SaveDataSelectScreen()
{
}

void SaveDataSelectScreen::Init()
{
}

void SaveDataSelectScreen::Update()
{
	// ボタンが押されたらフェードアウトを設定する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		// ゲームメインシーンに遷移
		m_pSceneSaveDataSelect->ChangeScene(SceneSaveDataSelect::NextScene::GameMain);
	}

	// ボタンが押されたらフェードアウトを設定する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::CANCEL))
	{
		// タイトルシーンに遷移
		m_pSceneSaveDataSelect->ChangeScene(SceneSaveDataSelect::NextScene::Title);
	}
}

void SaveDataSelectScreen::Draw()
{
	// 背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x4682b4, true);
}