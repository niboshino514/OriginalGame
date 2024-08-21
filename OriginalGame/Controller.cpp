#include "Controller.h"
// シングルトン
Controller* Controller::m_pInstance = nullptr;

namespace
{
    // ログのサイズ
    const int kLogSize = 16;
}

Controller::Controller()
{
    // ログの初期化
    m_padLog.resize(kLogSize);

    // 現在の入力デバイス
    m_currentInputDevice = ControllerType::CONTROLLER;

    // キー状態の初期化
    m_keyInputState = {};
}

void Controller::Update()
{
    // キーボードの入力状態を更新
	KeyInputUpdate();

	// パッドの入力状態を更新
	PadInputUpdate();
}

void Controller::KeyInputUpdate()
{
    // 全てのキーの入力状態を得る
    GetHitKeyStateAll(m_keyInputState.key);

    for (int i = 0; i < 256; i++)
    {
        // i番のキーコードに対応するキーが押されていたら
        if (m_keyInputState.key[i] != 0)
        {
            m_keyInputState.count[i]++;     // 加算

            // 10000以上になったらオーバーフローしないようにする
            if (m_keyInputState.count[i] >= 10000)
            {
                m_keyInputState.count[i] = 10000;
            }

            // キーボード入力があった場合、入力デバイスをキーボードに設定
            m_currentInputDevice = ControllerType::KEYBOARD;
        }
        else
        {              // 押されていなければ
            m_keyInputState.count[i] = 0;   // 0にする
        }
    }
}

void Controller::PadInputUpdate()
{
    // 現在のパッドの状態を取得
    int padState = GetJoypadInputState(DX_INPUT_PAD1);

    // パッド入力があった場合、入力デバイスをキーボードに設定
    if (padState != 0)
    {
        m_currentInputDevice = ControllerType::CONTROLLER;
    }
    // ログの更新
    for (int i = kLogSize - 1; i >= 1; i--)
    {
        m_padLog[i] = m_padLog[i - 1];
    }

    // 最新の状態
    m_padLog[0] = padState;
}

bool Controller::IsPress(const ControllerButton& button)
{
    // ボタンのコードを取得
    const int buttonCode = GetButton(button);

    if(m_currentInputDevice == ControllerType::CONTROLLER)
	{
		// コントローラーの場合
		return (m_padLog[0] & buttonCode);
	}
    else if (m_currentInputDevice == ControllerType::KEYBOARD)
    {
        // キーボードの場合
        return (m_keyInputState.key[buttonCode] != 0);
    }

    return false;
}

bool Controller::IsTrigger(const ControllerButton& button)
{
    // ボタンのコードを取得
    const int buttonCode = GetButton(button);

	if (m_currentInputDevice == ControllerType::CONTROLLER)
	{
		// コントローラーの場合
		return ((m_padLog[0] & buttonCode) && !(m_padLog[1] & buttonCode));
	}
	else if (m_currentInputDevice == ControllerType::KEYBOARD)
	{
		// キーボードの場合
		return (m_keyInputState.key[buttonCode] != 0 && m_keyInputState.count[buttonCode] == 1);
	}
    
	return false;
}

int Controller::GetButton(const ControllerButton& button)
{
    // 入力デバイスによってボタンを切り替え
    switch (m_currentInputDevice)
    {
    case ControllerType::KEYBOARD:
        // キーボードの場合
        return keyboardMapping.at(button);

    case ControllerType::CONTROLLER:

        // コントローラの場合
        return controllerMapping.at(button);
    default:

        // キーボードの場合
        return keyboardMapping.at(button);
    }
}
