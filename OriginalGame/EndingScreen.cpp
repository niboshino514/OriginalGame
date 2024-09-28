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
	// �{�^���������ꂽ��t�F�[�h�A�E�g��ݒ肷��
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		m_pSceneEnding->ChangeScene(SceneEnding::Scene::Title);
	}
}

void EndingScreen::Draw()
{	// �w�i
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00ff00, true);
}
