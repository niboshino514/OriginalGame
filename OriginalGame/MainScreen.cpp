#include "MainScreen.h"
#include "ObjectFactory.h"


MainScreen::MainScreen():
	m_pObjectFactory(std::make_shared<ObjectFactory>())
{
}

MainScreen::~MainScreen()
{
}

void MainScreen::Init()
{
	// 初期化処理
	m_pObjectFactory->Init();
}

void MainScreen::Update()
{
	// 更新処理
	m_pObjectFactory->Update();
}

void MainScreen::Draw()
{
	// 描画処理
	m_pObjectFactory->Draw();
}
