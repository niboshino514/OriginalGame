#pragma once
#include <vector>
#include <Vec2.h>
#include "Controller.h"

class ControllerOption
{
public:
	ControllerOption();
	virtual ~ControllerOption();

	void Init();
	void Update();
	void Draw();


private:

	// �ݒ荀��
	enum class SettingItemSenect
	{
		// �R���g���[���[�����؊���
		AUTO_SWITCH,
		// �R���g���[���[�^�C�v�I��
		CTRL_TYPE,
		// ���̓f�o�C�X�ؑ�
		INPUT_DEVICE,
		// �߂�
		BACK,
		// �Z���N�g��
		SELECT_NUM,
	};





	// �O���t�B�b�N���
	struct GraphInfo
	{
		// �n���h��
		std::vector<int> handle;

		// ���W
		std::vector<Vec2> pos;
	};

	



	// �O���t�B�b�N�̃��[�h
	void LoadGraph();

	// �ݒ荀�ڂ̕`��
	void DrawSettingItem();
	// �R���g���[���[�ݒ�̕`��
	void DrawControllerSetting();


private:

	//////////////
	// �I���֘A //
	//////////////

	// �I������
	SettingItemSenect m_settingItemSenect;

	// �R���g���[���[�ݒ�
	Controller::ControllerSetting m_controllerSetting;


	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////


	// �ݒ荀�ڃO���t�B�b�N
	GraphInfo m_settingItemGraph;

	// �R���g���[���ݒ�O���t�B�b�N
	std::vector<GraphInfo> m_controllerSettingGraph;


};

