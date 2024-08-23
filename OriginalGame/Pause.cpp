#include "Pause.h"
#include "game.h"
#include <DxLib.h>
#include "Controller.h"
#include "ObjectManager.h"
#include "EvoLib.h"
#include "ControllerOption.h"

namespace Window
{
	// �|�[�Y�E�B���h�E�̉���
	const int kWidth = 700;

	// �|�[�Y�E�B���h�E�̏c��
	const int kHeight = 500;

	// �����x
	const int kAlpha = 200;
}

namespace Graph
{
	// �|�[�Y�Z���N�g�O���t�B�b�N�t�@�C���p�X
	const char* const kPauseSelectFilePath = "Data/Pause/PauseSelect.png";
	// �|�[�Y�Z���N�g�O���t�B�b�N�̃O���t�B�b�N����
	const Vec2 kPauseSelectDistanceValue = Vec2(0, 10);
	// �|�[�Y�Z���N�g�̏c������
	const int kPauseSelectDivY = 4;
	// �|�[�Y���S���W
	const Vec2 kPauseCenterPos = Vec2(Game::kWindowCenterX, Game::kWindowCenterY);

	// �o�b�N�^�C�g���Z���N�g�O���t�B�b�N�t�@�C���p�X
	const char* const kBackTitleSelectFilePath = "Data/Selection/Selection.png";
	// �o�b�N�^�C�g���Z���N�g�O���t�B�b�N�̃O���t�B�b�N����
	const Vec2 kBackTitleSelectDistanceValue = Vec2(50, 0);
	// �o�b�N�^�C�g���Z���N�g�̏c������
	const int kBackTitleSelectDivY = 2;
	// �o�b�N�^�C�g�����S���W
	const Vec2 kBackTitleCenterPos = Vec2(kPauseCenterPos.x, kPauseCenterPos.y+200);

	// �g�嗦
	const double kScale = 0.8;

	// �����x
	const int kAlpha = 100;
}

Pause::Pause():
	m_pauseSelect(),
	m_backTitleSelect(),
	m_pauseSelectGprah(),
	m_backTitleSelectGraph(),
	m_pStateMachine(),
	m_pObjectFactory(),
	m_pControllerOption(std::make_shared<ControllerOption>())
{
}

Pause::~Pause()
{
	// �O���t�B�b�N�̃f���[�g
	for (auto& handle : m_pauseSelectGprah.handle)
	{
		DeleteGraph(handle);
	};
	for (auto& handle : m_backTitleSelectGraph.handle)
	{
		DeleteGraph(handle);
	};

}

void Pause::Init()
{

	// �R���g���[���[�I�v�V�����̏�����
	m_pControllerOption->Init();

	// �O���t�B�b�N�̃��[�h
	LoadGraph();

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
	// �|�[�Y�E�B���h�E�`��
	DrawPauseWindow();

	// �I��`��
	PauseSelectDraw();

	// �X�e�[�g�}�V���̕`��
	m_pStateMachine.Draw();
}

void Pause::StateInit()
{

	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �ʏ�X�e�[�g
	{
		auto enter = [this]() { StateNormalEnter(); };
		auto update = [this]() { StateNormalUpdate(); };
		
		m_pStateMachine.AddState(State::Normal, enter, update, dummy, dummy);
	}
	// �|�[�Y�X�e�[�g
	{
		auto enter = [this]() { StatePauseEnter(); };
		auto update = [this]() { StatePauseUpdate(); };
		auto draw = [this]() { StatePauseDraw(); };
		auto exit = [this]() { StatePauseExit(); };

		m_pStateMachine.AddState(State::Pause, enter, update, draw, dummy);
	}
	// ���͐ؑփX�e�[�g
	{
		auto enter = [this]() { StateChangeInputEnter(); };
		auto update = [this]() { StateChangeInputUpdate(); };
		auto draw = [this]() { StateChangeInputDraw(); };

		m_pStateMachine.AddState(State::ChangeInput, enter, update, draw, dummy);
	}
	// �o�b�N�^�C�g���X�e�[�g
	{
		auto enter = [this]() { StateBackTitleEnter(); };
		auto update = [this]() { StateBackTitleUpdate(); };
		auto draw = [this]() { StateBackTitleDraw(); };

		m_pStateMachine.AddState(State::BackTitle, enter, update, draw, dummy);
	}


	// �����X�e�[�g��ݒ�
	m_pStateMachine.SetState(State::Normal);
}

void Pause::StateNormalEnter()
{
	// �|�[�Y�I���̏�����
	m_pauseSelect = PauseSelect::Resume;
	// �o�b�N�^�C�g���I���̏�����
	m_backTitleSelect = BackTitleSelect::No;
}

void Pause::StateNormalUpdate()
{

	////////// test
	{
		// �I�u�W�F�N�g�t�@�N�g���[�̃X�e�[�g���|�[�Y�ɐݒ�
		m_pObjectFactory->SetState(ObjectManager::State::Pause);
		// ���͐ؑփX�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Pause);
		m_pauseSelect = PauseSelect::ChangeInput;
	}






	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// �|�[�Y�X�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Pause);
		// �I�u�W�F�N�g�t�@�N�g���[�̃X�e�[�g���|�[�Y�ɐݒ�
		m_pObjectFactory->SetState(ObjectManager::State::Pause);
	}
}

void Pause::StatePauseEnter()
{
	
}

void Pause::StatePauseUpdate()
{
	// �I���X�V
	PauseSelectUpdate();

	// ���菈��
	PauseSelectDecision();


	// �|�[�Y�{�^���������ꂽ��ʏ�X�e�[�g�ɑJ��
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// �ʏ�X�e�[�g�ɑJ��
		ResumeProcess();
	}
}

void Pause::StatePauseDraw()
{
}

void Pause::StatePauseExit()
{

}

void Pause::StateChangeInputEnter()
{
}

void Pause::StateChangeInputUpdate()
{
	// �ݒ荀�ڍX�V
	m_pControllerOption->Update();
}

void Pause::StateChangeInputDraw()
{
	// �ݒ荀�ڕ`��
	m_pControllerOption->Draw();
}

void Pause::StateBackTitleEnter()
{

}

void Pause::StateBackTitleUpdate()
{
	// �o�b�N�^�C�g���I���X�V
	BackTitleSelectUpdate();
	// �o�b�N�^�C�g�����菈��
	BackTitleSelectDecision();
}

void Pause::StateBackTitleDraw()
{
	// �o�b�N�^�C�g���I��`��
	BackTitleSelectDraw();
}

void Pause::LoadGraph()
{
	// �|�[�Y�I���O���t�B�b�N
	{
		// �|�[�Y�I���O���t�B�b�N
		m_pauseSelectGprah.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(Graph::kPauseSelectFilePath, EvoLib::Load::DivNum(1, 4));

		// �O���t�B�b�N�̍��W���ϓ��ɔz�u
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_pauseSelectGprah.handle, Graph::kScale);

		m_pauseSelectGprah.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, Graph::kPauseCenterPos, static_cast<int>(m_pauseSelectGprah.handle.size()), Graph::kPauseSelectDistanceValue, false);
	}

	// �o�b�N�^�C�g���I���O���t�B�b�N
	{
		// �o�b�N�^�C�g���I���O���t�B�b�N
		m_backTitleSelectGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(Graph::kBackTitleSelectFilePath, EvoLib::Load::DivNum(1, 2));

		// �O���t�B�b�N�̍��W���ϓ��ɔz�u
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_backTitleSelectGraph.handle, Graph::kScale);

		m_backTitleSelectGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, Graph::kBackTitleCenterPos, static_cast<int>(m_backTitleSelectGraph.handle.size()), Graph::kBackTitleSelectDistanceValue, true);
	}
}

void Pause::PauseSelectUpdate()
{
	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) - 1 + static_cast<int>(PauseSelect::SelectNum)) % static_cast<int>(PauseSelect::SelectNum));
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) + 1) % static_cast<int>(PauseSelect::SelectNum));
	}
}

void Pause::PauseSelectDecision()
{
	// ����{�^����������Ă��Ȃ����return
	if(!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}



	switch (m_pauseSelect)
	{
	case PauseSelect::Resume:

		// �ʏ�X�e�[�g�ɑJ��
		ResumeProcess();

		break;
	case PauseSelect::Volume:
		break;
	case PauseSelect::ChangeInput:

		// ���͐ؑփX�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::ChangeInput);

		break;
	case PauseSelect::Title:

		// �o�b�N�^�C�g���I���ɑJ��
		m_pStateMachine.SetState(State::BackTitle);

		break;
	
	default:

		// �ʏ�X�e�[�g�ɑJ��
		ResumeProcess();

		break;
	}

}

void Pause::BackTitleSelectUpdate()
{


	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{
		m_backTitleSelect = static_cast<BackTitleSelect>((static_cast<int>(m_backTitleSelect) - 1 + static_cast<int>(BackTitleSelect::SelectNum)) % static_cast<int>(BackTitleSelect::SelectNum));
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		m_backTitleSelect = static_cast<BackTitleSelect>((static_cast<int>(m_backTitleSelect) + 1) % static_cast<int>(BackTitleSelect::SelectNum));
	}
}

void Pause::BackTitleSelectDecision()
{


	// ����{�^����������Ă��Ȃ����return
	if (!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}

	switch (m_backTitleSelect)
	{
	case BackTitleSelect::Yes:

		// �^�C�g���ɑJ��
		m_pObjectFactory->ChangeScene(SceneMain::Scene::Title);

		// �����t�𖳌��ɂ���
		Controller::GetInstance()->SetAcceptInput(false);

		break;
	case BackTitleSelect::No:

		// �|�[�Y�X�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Pause);

		break;
	default:

		// �|�[�Y�X�e�[�g�ɑJ��
		m_pStateMachine.SetState(State::Pause);

		break;
	}
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

void Pause::PauseSelectDraw()
{

	// �|�[�Y�I��`��
	{
		// �����x�ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Graph::kAlpha);

		// �I�΂�Ă���I�����͐ԐF�ŕ`�悵�A����ȊO�͔������ŕ`��
		for (int i = 0; i < static_cast<int>(PauseSelect::SelectNum); i++)
		{
			if (m_pauseSelect != static_cast<PauseSelect>(i))
			{
				DrawRotaGraphF(m_pauseSelectGprah.pos[i].x, m_pauseSelectGprah.pos[i].y, Graph::kScale, 0.0, m_pauseSelectGprah.handle[i], true);
			}
		}

		// �����x����
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �I��`��
	{
		// �I�΂�Ă���I�����͐ԐF�ŕ`��
		SetDrawBright(255, 0, 0);

		DrawRotaGraphF(m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].x, m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].y, Graph::kScale, 0.0, m_pauseSelectGprah.handle[static_cast<int>(m_pauseSelect)], true);

		// �F�����ɖ߂�
		SetDrawBright(255, 255, 255);
	}
}

void Pause::BackTitleSelectDraw()
{

	// �o�b�N�^�C�g���I��`��
	{
		// �����x�ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Graph::kAlpha);

		// �I�΂�Ă���I�����͐ԐF�ŕ`�悵�A����ȊO�͔������ŕ`��
		for (int i = 0; i < static_cast<int>(BackTitleSelect::SelectNum); i++)
		{
			if (m_backTitleSelect != static_cast<BackTitleSelect>(i))
			{
				DrawRotaGraphF(m_backTitleSelectGraph.pos[i].x, m_backTitleSelectGraph.pos[i].y, Graph::kScale, 0.0, m_backTitleSelectGraph.handle[i], true);
			}
		}

		// �����x����
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �I��`��
	{
		// �I�΂�Ă���I�����͐ԐF�ŕ`��
		SetDrawBright(255, 0, 0);

		DrawRotaGraphF(m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].x, m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].y, Graph::kScale, 0.0, m_backTitleSelectGraph.handle[static_cast<int>(m_backTitleSelect)], true);

		// �F�����ɖ߂�
		SetDrawBright(255, 255, 255);
	}

}

void Pause::ResumeProcess()
{
	// �ʏ�X�e�[�g�ɑJ��
	m_pStateMachine.SetState(State::Normal);
	// �I�u�W�F�N�g�t�@�N�g���[�̃X�e�[�g��ʏ�ɐݒ�
	m_pObjectFactory->SetState(ObjectManager::State::Normal);
}
