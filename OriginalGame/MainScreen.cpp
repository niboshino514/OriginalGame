#include "MainScreen.h"
#include "ObjectManager.h"





MainScreen::MainScreen():
	m_pSceneMain(nullptr),
	m_pObjectFactory(std::make_shared<ObjectManager>())
{
}

MainScreen::~MainScreen()
{
}

void MainScreen::Init()
{
	// オブジェクトファクトリークラスに自身のポインタを渡す
	m_pObjectFactory->SetMainScreenPointer(shared_from_this());

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

void MainScreen::ChangeScene(const SceneMain::Scene& nextScene)
{
	// シーン変更
	m_pSceneMain->ChangeScene(nextScene);
}
