#include "EndingScreen.h"
#include "SceneEnding.h"
#include "Controller.h"
#include "game.h"

EndingScreen::EndingScreen() :
	m_pSceneEnding(nullptr)

{
}

EndingScreen::~EndingScreen()
{
}

void EndingScreen::Init()
{
}

void EndingScreen::Update()
{
	// ボタンが押されたらフェードアウトを設定する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		m_pSceneEnding->ChangeScene(SceneEnding::Scene::Title);
	}
}

void EndingScreen::Draw()
{	// 背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00ff00, true);
}
