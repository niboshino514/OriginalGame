#include "OpeningScreen.h"
#include "MessageWindow.h"
#include "SceneOpening.h"
#include <string>

namespace
{
	const std::string kTalkDataFilePath = "Data/Csv/Talk/TalkData_1.csv";
}

OpeningScreen::OpeningScreen():
	m_pSceneOpening(nullptr),
	m_pMessageWindow(std::make_shared<MessageWindow>())
{
}

OpeningScreen::~OpeningScreen()
{
}

void OpeningScreen::Init()
{

	// ���[�h
	m_pMessageWindow->LoadData(kTalkDataFilePath);
}

void OpeningScreen::Update()
{
	// �X�V
	m_pMessageWindow->Update();

	if(m_pMessageWindow->IsAllTextEnd())
	{
		// �V�[���ύX
		m_pSceneOpening->ChangeScene();
	}
}

void OpeningScreen::Draw()
{
	// �`��
	m_pMessageWindow->Draw();
}