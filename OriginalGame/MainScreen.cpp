#include "MainScreen.h"
#include "ObjectManager.h"


MainScreen::MainScreen():
	m_pObjectFactory(std::make_shared<ObjectManager>())
{
}

MainScreen::~MainScreen()
{
}

void MainScreen::Init()
{
	// ‰Šú‰»ˆ—
	m_pObjectFactory->Init();
}

void MainScreen::Update()
{
	// XVˆ—
	m_pObjectFactory->Update();
}

void MainScreen::Draw()
{
	// •`‰æˆ—
	m_pObjectFactory->Draw();
}
