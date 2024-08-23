#include "ControllerOption.h"
#include <DxLib.h>
#include "game.h"
#include "EvoLib.h"

namespace SettingItem
{
	// �O���t�B�b�N�t�@�C���p�X
	const char* const kFilePath = "Data/ControllerOption/SettingItem.png";
	// �O���t�B�b�N�̃O���t�B�b�N����
	const Vec2 kDistanceValue = Vec2(0, 40);
	// ������
	const EvoLib::Load::DivNum kDivNum = { 1, 4 };
	// ���S���W
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX-250, Game::kWindowCenterY);
	// �g�嗦
	const double kScale = 0.5;
}

namespace
{
	struct ControllerSettingGraphInfo
	{
		// �O���t�B�b�N�t�@�C���p�X
		const char* const kFilePath = "";
		// ������
		const EvoLib::Load::DivNum kDivNum = { 1, 1 };
		// �ݒ荀�ڂ���̋���
		const Vec2 kDistanceValue = Vec2(500, 0);
		// �g�嗦
		const double kScale = 0.5;
	};



	// �ݒ荀��
	const std::vector< ControllerSettingGraphInfo> m_controllerSettingGraphInfo =
	{
		// �R���g���[���[�����؊���
		{
			"Data/ControllerOption/Switch.png",
			{ 1, 2 },
			{ 500, 0 },
			0.5
		},
		// �R���g���[���[�^�C�v�I��
		{
			"Data/ControllerOption/ControllerType.png",
			{ 1, 3 },
			{ 500, 0 },
			0.5
		},
		// ���̓f�o�C�X�ؑ�
		{
			"Data/ControllerOption/InputDevice.png",
			{ 1, 2 },
			{ 500, 0 },
			0.5
		},
	};

}




ControllerOption::ControllerOption():
	m_settingItemSenect(),
	m_controllerSetting(),
	m_settingItemGraph(),
	m_controllerSettingGraph()
{
}

ControllerOption::~ControllerOption()
{
	// �n���h���̉��
	for(auto& handle : m_settingItemGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& graph : m_controllerSettingGraph)
	{
		for(auto& handle : graph.handle)
		{
			DeleteGraph(handle);
		}
	}

}

void ControllerOption::Init()
{
	// �I�����ڂ̏�����
	m_settingItemSenect = SettingItemSenect::AUTO_SWITCH;
	
	// �R���g���[���[�ݒ�擾
	m_controllerSetting = Controller::GetInstance()->GetControllerSetting();
	


	// �O���t�B�b�N�̃��[�h
	LoadGraph();
}

void ControllerOption::Update()
{
}

void ControllerOption::Draw()
{
	// �ݒ荀�ڂ̕`��
	DrawSettingItem();

	// �R���g���[���[�ݒ�̕`��
	DrawControllerSetting();
}

void ControllerOption::LoadGraph()
{
	// �ݒ荀�ڂ̃O���t�B�b�N�̃��[�h
	{
		// �O���t�B�b�N���[�h
		m_settingItemGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(SettingItem::kFilePath, SettingItem::kDivNum);

		// ���W�̐ݒ�
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_settingItemGraph.handle, SettingItem::kScale);

		// ���W�̐ݒ�
		m_settingItemGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, SettingItem::kCenterPos, static_cast<int>(m_settingItemGraph.handle.size()), SettingItem::kDistanceValue, false);
	}
	
	{
		// �Z���N�g��
		const int selectNum = static_cast<int>(SettingItemSenect::SELECT_NUM) - 1;

		// ���T�C�Y
		m_controllerSettingGraph.resize(selectNum);

		for (int i = 0; i < selectNum; i++)
		{
			// �ݒ�i���o�[
			const int settingNum = static_cast<int>(SettingItemSenect(i));


			// �O���t�B�b�N���[�h
			m_controllerSettingGraph[i].handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
			(m_controllerSettingGraphInfo[settingNum].kFilePath, m_controllerSettingGraphInfo[settingNum].kDivNum);

			// ���W�̐ݒ�
			const Vec2 pos = m_settingItemGraph.pos[i] + m_controllerSettingGraphInfo[settingNum].kDistanceValue;

			// ���W�̐ݒ�
			m_controllerSettingGraph[i].pos.push_back(pos);
		}


		

	}
}

void ControllerOption::DrawSettingItem()
{
	// �ݒ荀�ڂ̕`��
	for (size_t i = 0; i < m_settingItemGraph.handle.size(); i++)
	{
		DrawRotaGraphF(m_settingItemGraph.pos[i].x, m_settingItemGraph.pos[i].y, SettingItem::kScale, 0.0, m_settingItemGraph.handle[i], TRUE);
	}
}

void ControllerOption::DrawControllerSetting()
{
	// �O���t�B�b�N��
	const int selectNum = static_cast<int>(m_controllerSettingGraph.size());

	// �ݒ荀�ڂ̕`��
	for (int i = 0; i < selectNum; i++)
	{
		// �ݒ�i���o�[
		const int settingNum = static_cast<int>(SettingItemSenect(i));
		// �摜�i���o�[
		const int graphNum = 0;






		// �O���t�B�b�N�̕`��
		DrawRotaGraphF(
			m_controllerSettingGraph[i].pos[0].x, 
			m_controllerSettingGraph[i].pos[0].y, 
			m_controllerSettingGraphInfo[settingNum].kScale,
			0.0, m_controllerSettingGraph[i].handle[graphNum], TRUE);
	}


}
