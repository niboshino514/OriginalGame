#include "Pause.h"
#include "game.h"
#include <DxLib.h>
#include "Controller.h"
#include "ObjectManager.h"

namespace Window
{
	// ポーズウィンドウの横幅
	const int kWidth = 400;

	// ポーズウィンドウの縦幅
	const int kHeight = 300;

	// 透明度
	const int kAlpha = 200;
}


Pause::Pause():
	m_pStateMachine(),
	m_pObjectFactory()
{
}

Pause::~Pause()
{
}

void Pause::Init()
{
	// ステートマシンの初期化
	StateInit();
}

void Pause::Update()
{
	// ステートマシンの更新
	m_pStateMachine.Update();
}

void Pause::Draw()
{
	// ステートマシンの描画
	m_pStateMachine.Draw();
}

void Pause::StateInit()
{

	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 通常ステート
	{
		
		auto update = [this]() { StateNormalUpdate(); };
		
		

		m_pStateMachine.AddState(State::Normal, dummy, update, dummy, dummy);
	}
	// ポーズステート
	{
		auto update = [this]() { StatePauseUpdate(); };
		auto draw = [this]() { StatePauseDraw(); };

		m_pStateMachine.AddState(State::Pause, dummy, update, draw, dummy);
	}

	// 初期ステートを設定
	m_pStateMachine.SetState(State::Normal);
}

void Pause::StateNormalUpdate()
{
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// オブジェクトファクトリーのステートをポーズに設定
		m_pObjectFactory->SetState(ObjectManager::State::Pause);

		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);
	}
}

void Pause::StatePauseUpdate()
{
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// オブジェクトファクトリーのステートを通常に設定
		m_pObjectFactory->SetState(ObjectManager::State::Normal);

		// 通常ステートに遷移
		m_pStateMachine.SetState(State::Normal);
	}
}

void Pause::StatePauseDraw()
{
	// ポーズウィンドウ描画
	DrawPauseWindow();
}

void Pause::DrawPauseWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - Window::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - Window::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + Window::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + Window::kHeight / 2;


	// 透明度設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);

	// ポーズウィンドウ
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズウィンドウ枠
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);
}
