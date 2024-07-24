#include "SceneManager.h"
#include <cassert>

#include "SceneMain.h"
#include "Pad.h"
#include <DxLib.h>
#include "game.h"


SceneManager::SceneManager()
{
	m_pScene = nullptr;
}
SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
	m_pScene = new SceneMain;
	m_pScene->init();
}

void SceneManager::end()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	m_pScene->end();
	delete m_pScene;
}

void SceneManager::update()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	// 更新処理速度をはかる
	LONGLONG start = GetNowHiPerformanceCount();

	// パッドのアップデート処理
	Pad::Update();

	SceneBase* pScene = m_pScene->update();
	if (pScene != m_pScene)
	{
		// 前のシーンの終了処理
		m_pScene->end();
		delete m_pScene;

		m_pScene = pScene;
		m_pScene->init();
	}

	// 更新処理速度計算
	m_updateTime = static_cast<int>(GetNowHiPerformanceCount() - start);
}

void SceneManager::draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	// 描画処理速度をはかる
	LONGLONG start = GetNowHiPerformanceCount();

	m_pScene->draw();

	// 描画処理速度計算
	m_drawTime = static_cast<int>(GetNowHiPerformanceCount() - start);

	DrawString(0, Game::kScreenHeight - 48, "描画", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 48 + 2, 48 + 16 - 2, Game::kScreenHeight - 32 - 2, 0x0000ff, true);

	DrawString(0, Game::kScreenHeight - 32, "処理", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 32 + 2, 48 + 16 - 2, Game::kScreenHeight - 16 - 2, 0xff0000, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	int width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0x0000ff, true);

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0xff0000, true);

}