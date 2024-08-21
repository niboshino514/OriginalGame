#pragma once
#include <unordered_map>
#include <DxLib.h>
#include <vector>

// �R���g���[���[
class Controller
{
private:
	Controller();
	virtual ~Controller() = default;

	static Controller* m_pInstance;

public:

	// �R�s�[�C���X�g���N�^�̋֎~
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller(Controller&&) = delete;
	Controller& operator=(Controller&&) = delete;

	static Controller* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new Controller();
		}

		return m_pInstance;
	}

	static void DeleteInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:

	// �R���g���[���[�̃{�^��
	enum class ControllerButton
	{
		DECIDE, // ����
		CANCEL, // �L�����Z��

		JUMP,   // �W�����v
		RESPAWN,// ���X�|�[��

		PAUSE,  // �|�[�Y

		UP,     // ��
		DOWN,   // ��
		LEFT,   // ��
		RIGHT,  // �E
	};

private:

	// �{�^���̃}�b�s���O
	const std::unordered_map<ControllerButton, int> keyboardMapping =
	{
		{ControllerButton::DECIDE, KEY_INPUT_Z},
		{ControllerButton::CANCEL, KEY_INPUT_X},

		{ControllerButton::JUMP, KEY_INPUT_SPACE},
		{ControllerButton::RESPAWN, KEY_INPUT_R},

		{ControllerButton::PAUSE, KEY_INPUT_Q},

		{ControllerButton::UP, KEY_INPUT_UP},
		{ControllerButton::DOWN, KEY_INPUT_DOWN},
		{ControllerButton::LEFT, KEY_INPUT_LEFT},
		{ControllerButton::RIGHT, KEY_INPUT_RIGHT},
	};

	// �{�^���̃}�b�s���O
	const std::unordered_map<ControllerButton, int> controllerMapping =
	{
		{ControllerButton::DECIDE, PAD_INPUT_1},
		{ControllerButton::CANCEL, PAD_INPUT_2},

		{ControllerButton::JUMP, PAD_INPUT_1},
		{ControllerButton::RESPAWN, PAD_INPUT_4},

		{ControllerButton::PAUSE, PAD_INPUT_8},

		{ControllerButton::UP, PAD_INPUT_UP},
		{ControllerButton::DOWN, PAD_INPUT_DOWN},
		{ControllerButton::LEFT, PAD_INPUT_LEFT},
		{ControllerButton::RIGHT, PAD_INPUT_RIGHT},
	};

	// �R���g���[���^�C�v
	enum class ControllerType
	{
		// �L�[�{�[�h
		KEYBOARD,
		// �R���g���[��
		CONTROLLER,
	};

public:



	// �R���g���[���[�̓��͏�ԍX�V
	void Update();

	// �L�[�̓��͏�ԍX�V
	void KeyInputUpdate();
	// �p�b�h�̓��͏�ԍX�V
	void PadInputUpdate();


	// �����ꑱ���Ă邩�ǂ���
	bool IsPress(const ControllerButton& button);
	// �����ꂽ�u�Ԃ��ǂ���
	bool IsTrigger(const ControllerButton& button);


private:

	// �R���g���[���[�̎�ނɂ��{�^����
	int GetButton(const ControllerButton& button);

	// �L�[�̓��͏��
	struct KeyInputState
	{
		char key[256];
		int count[256];
	};

private:

	
	// ���̓��O 0���ŐV�̏��
	std::vector<int> m_padLog;

	// �L�[�̏��
	KeyInputState m_keyInputState;

	// ���݂̓��̓f�o�C�X
	ControllerType m_currentInputDevice = ControllerType::KEYBOARD;
};