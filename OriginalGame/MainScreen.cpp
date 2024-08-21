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
	// ����������
	m_pObjectFactory->Init();
}

void MainScreen::Update()
{
	// �X�V����
	m_pObjectFactory->Update();
}

void MainScreen::Draw()
{
	// �`�揈��
	m_pObjectFactory->Draw();
}
