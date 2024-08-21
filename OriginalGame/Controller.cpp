#include "Controller.h"
// �V���O���g��
Controller* Controller::m_pInstance = nullptr;

namespace
{
    // ���O�̃T�C�Y
    const int kLogSize = 16;
}

Controller::Controller()
{
    // ���O�̏�����
    m_padLog.resize(kLogSize);

    // ���݂̓��̓f�o�C�X
    m_currentInputDevice = ControllerType::CONTROLLER;

    // �L�[��Ԃ̏�����
    m_keyInputState = {};
}

void Controller::Update()
{
    // �L�[�{�[�h�̓��͏�Ԃ��X�V
	KeyInputUpdate();

	// �p�b�h�̓��͏�Ԃ��X�V
	PadInputUpdate();
}

void Controller::KeyInputUpdate()
{
    // �S�ẴL�[�̓��͏�Ԃ𓾂�
    GetHitKeyStateAll(m_keyInputState.key);

    for (int i = 0; i < 256; i++)
    {
        // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
        if (m_keyInputState.key[i] != 0)
        {
            m_keyInputState.count[i]++;     // ���Z

            // 10000�ȏ�ɂȂ�����I�[�o�[�t���[���Ȃ��悤�ɂ���
            if (m_keyInputState.count[i] >= 10000)
            {
                m_keyInputState.count[i] = 10000;
            }

            // �L�[�{�[�h���͂��������ꍇ�A���̓f�o�C�X���L�[�{�[�h�ɐݒ�
            m_currentInputDevice = ControllerType::KEYBOARD;
        }
        else
        {              // ������Ă��Ȃ����
            m_keyInputState.count[i] = 0;   // 0�ɂ���
        }
    }
}

void Controller::PadInputUpdate()
{
    // ���݂̃p�b�h�̏�Ԃ��擾
    int padState = GetJoypadInputState(DX_INPUT_PAD1);

    // �p�b�h���͂��������ꍇ�A���̓f�o�C�X���L�[�{�[�h�ɐݒ�
    if (padState != 0)
    {
        m_currentInputDevice = ControllerType::CONTROLLER;
    }
    // ���O�̍X�V
    for (int i = kLogSize - 1; i >= 1; i--)
    {
        m_padLog[i] = m_padLog[i - 1];
    }

    // �ŐV�̏��
    m_padLog[0] = padState;
}

bool Controller::IsPress(const ControllerButton& button)
{
    // �{�^���̃R�[�h���擾
    const int buttonCode = GetButton(button);

    if(m_currentInputDevice == ControllerType::CONTROLLER)
	{
		// �R���g���[���[�̏ꍇ
		return (m_padLog[0] & buttonCode);
	}
    else if (m_currentInputDevice == ControllerType::KEYBOARD)
    {
        // �L�[�{�[�h�̏ꍇ
        return (m_keyInputState.key[buttonCode] != 0);
    }

    return false;
}

bool Controller::IsTrigger(const ControllerButton& button)
{
    // �{�^���̃R�[�h���擾
    const int buttonCode = GetButton(button);

	if (m_currentInputDevice == ControllerType::CONTROLLER)
	{
		// �R���g���[���[�̏ꍇ
		return ((m_padLog[0] & buttonCode) && !(m_padLog[1] & buttonCode));
	}
	else if (m_currentInputDevice == ControllerType::KEYBOARD)
	{
		// �L�[�{�[�h�̏ꍇ
		return (m_keyInputState.key[buttonCode] != 0 && m_keyInputState.count[buttonCode] == 1);
	}
    
	return false;
}

int Controller::GetButton(const ControllerButton& button)
{
    // ���̓f�o�C�X�ɂ���ă{�^����؂�ւ�
    switch (m_currentInputDevice)
    {
    case ControllerType::KEYBOARD:
        // �L�[�{�[�h�̏ꍇ
        return keyboardMapping.at(button);

    case ControllerType::CONTROLLER:

        // �R���g���[���̏ꍇ
        return controllerMapping.at(button);
    default:

        // �L�[�{�[�h�̏ꍇ
        return keyboardMapping.at(button);
    }
}
