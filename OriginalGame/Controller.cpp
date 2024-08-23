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

    // 設定の初期化
    {
        // 自動切換え
        m_controllerSetting.autoSwitch = Controller::AutoSwitch::ON;

        // パッドタイプ
        m_controllerSetting.padType = Controller::PadType::XBOX;

        // コントローラータイプ
        m_controllerSetting.controllerType = Controller::ControllerType::CONTROLLER;
    }

    // キー状態の初期化
    m_keyInputState = {};

    // 入力受付フラグ
    m_isAcceptInput = true;
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

            // オートスイッチがONの場合
            if(m_controllerSetting.autoSwitch == AutoSwitch::ON)
			{
				// キーボード入力があった場合、入力デバイスをキーボードに設定
				m_controllerSetting.controllerType = ControllerType::KEYBOARD;
			}

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
        // オートスイッチがONの場合
        if (m_controllerSetting.autoSwitch == AutoSwitch::ON)
        {
            m_controllerSetting.controllerType = ControllerType::CONTROLLER;
        }
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
    // 入力受付フラグがfalseの場合、入力を受け付けない
    if (!m_isAcceptInput)
	{
		return false;
	}



    // ボタンのコードを取得
    const int buttonCode = GetButton(button);

    if(m_controllerSetting.controllerType == ControllerType::CONTROLLER)
	{
		// コントローラーの場合
		return (m_padLog[0] & buttonCode);
	}
    else if (m_controllerSetting.controllerType == ControllerType::KEYBOARD)
    {
        // キーボードの場合
        return (m_keyInputState.key[buttonCode] != 0);
    }

    return false;
}

bool Controller::IsTrigger(const ControllerButton& button)
{
    // 入力受付フラグがfalseの場合、入力を受け付けない
    if (!m_isAcceptInput)
    {
        return false;
    }

    // ボタンのコードを取得
    const int buttonCode = GetButton(button);

	if (m_controllerSetting.controllerType == ControllerType::CONTROLLER)
	{
		// コントローラーの場合
		return ((m_padLog[0] & buttonCode) && !(m_padLog[1] & buttonCode));
	}
	else if (m_controllerSetting.controllerType == ControllerType::KEYBOARD)
	{
		// キーボードの場合
		return (m_keyInputState.key[buttonCode] != 0 && m_keyInputState.count[buttonCode] == 1);
	}
    
	return false;
}

int Controller::GetButton(const ControllerButton& button)
{

    
    if (m_controllerSetting.controllerType == ControllerType::KEYBOARD)
    {
        // キーボードの場合
		return keyboardMapping.at(button);
    }
    else
    {

        switch (m_controllerSetting.padType)
        {
        case Controller::PadType::XBOX:
            // XBOXの場合
            return xboxMapping.at(button);

            break;
        case Controller::PadType::DUALSHOCK:
            // デュアルショックの場合
            return dualShockMapping.at(button);

            break;
        case Controller::PadType::SWITCH_PRO:
            // SWITCHPROの場合
            return switchProMapping.at(button);
            break;
        default:
            // XBOXの場合
            return xboxMapping.at(button);

            break;
        }
    }


    // キーボードの場合
    return keyboardMapping.at(button);
}
