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
	// �{�^���������ꂽ��t�F�[�h�A�E�g��ݒ肷��
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::SaveDataSelect);
	}
}

void TitleScreen::Draw()
{
	// �w�i
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xa9a9a9, true);

}
