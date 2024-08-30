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

	// ロード
	m_pMessageWindow->LoadData(kTalkDataFilePath);
}

void OpeningScreen::Update()
{
	// 更新
	m_pMessageWindow->Update();

	if(m_pMessageWindow->IsAllTextEnd())
	{
		// シーン変更
		m_pSceneOpening->ChangeScene();
	}
}

void OpeningScreen::Draw()
{
	// 描画
	m_pMessageWindow->Draw();
}