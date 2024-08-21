#include "Pause.h"
#include "game.h"
#include <DxLib.h>
#include "Controller.h"
#include "ObjectManager.h"

namespace Window
{
	// �|�[�Y�E�B���h�E�̉���
	const int kWidth = 400;

	// �|�[�Y�E�B���h�E�̏c��
	const int kHeight = 300;

	// �����x
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
	// �X�e�[�g�}�V���̏�����
	StateInit();
}

void Pause::Update()
{
	// �X�e�[�g�}�V���̍X�V
	m_pStateMachine.Update();
}

void Pause::Draw()
{
	// �X�e�[�g�}�V���̕`��
	m_pStateMachine.Draw();
}

void Pause::StateInit()
{

	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �ʏ�X�e�[�g
	{
		
		auto update = [this]() { StateNormalUpdate(); };
		
		

		m_pStateMachine.AddState(State::Normal, dummy, update, dummy, dummy);
	}
	// �|�[�Y�X�e�[�g
	{
		auto update = [this]() { StatePauseUpdate(); };
		auto draw = [this]() { StatePauseDraw(); };

		m_pStateMachine.AddState(State::Pause, dummy, update, draw, dummy);
	}

	// �����X�e�[�g��ݒ�
	m_pStateMachine.SetState(State::Normal);
}

void Pause::StateNormalUpdate()
{
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// �I�u�W�F�N�g�t�@�N�g���[�̃X�e�[�g���|�[�Y�ɐݒ�
		m_pObjectFactory->SetState(ObjectManager::State::Pause);

		// �|�[�Y�X�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Pause);
	}
}

void Pause::StatePauseUpdate()
{
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// �I�u�W�F�N�g�t�@�N�g���[�̃X�e�[�g��ʏ�ɐݒ�
		m_pObjectFactory->SetState(ObjectManager::State::Normal);

		// �ʏ�X�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Normal);
	}
}

void Pause::StatePauseDraw()
{
	// �|�[�Y�E�B���h�E�`��
	DrawPauseWindow();
}

void Pause::DrawPauseWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - Window::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - Window::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + Window::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + Window::kHeight / 2;


	// �����x�ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);

	// �|�[�Y�E�B���h�E
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// �����x����
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �|�[�Y�E�B���h�E�g
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);
}
