#include <unordered_map>
#include <DxLib.h>

namespace
{
    // コントローラタイプ
    enum class ControllerType
    {
        // キーボード
        KEYBOARD,
        // コントローラ
        CONTROLLER,
    };

    constexpr int kLogNum = 16;
    // 入力ログ 0が最新の状態
    int padLog[kLogNum];

    // 現在の入力デバイス
    ControllerType currentInputDevice = ControllerType::KEYBOARD;

    // ボタンの種類
    enum class Button
    {
        DECIDE, // 決定
        CANCEL, // キャンセル

        JUMP,   // ジャンプ
        RESPAWN,// リスポーン

        PAUSE,  // ポーズ

        UP,     // 上
        DOWN,   // 下
        LEFT,   // 左
        RIGHT,  // 右
    };

    // ボタンのマッピング
    const std::unordered_map<Button, int> keyboardMapping =
    {
        {Button::DECIDE, KEY_INPUT_Z},
        {Button::CANCEL, KEY_INPUT_X},

        {Button::JUMP, KEY_INPUT_SPACE},
        {Button::RESPAWN, KEY_INPUT_R},

        {Button::PAUSE, KEY_INPUT_Q},

        {Button::UP, KEY_INPUT_UP},
        {Button::DOWN, KEY_INPUT_DOWN},
        {Button::LEFT, KEY_INPUT_LEFT},
        {Button::RIGHT, KEY_INPUT_RIGHT},
    };

    const std::unordered_map<Button, int> controllerMapping =
    {
        {Button::DECIDE, PAD_INPUT_1},
        {Button::CANCEL, PAD_INPUT_2},

        {Button::JUMP, PAD_INPUT_3},
        {Button::RESPAWN, PAD_INPUT_4},

        {Button::PAUSE, PAD_INPUT_4},

        {Button::UP, PAD_INPUT_UP},
        {Button::DOWN, PAD_INPUT_DOWN},
        {Button::LEFT, PAD_INPUT_LEFT},
        {Button::RIGHT, PAD_INPUT_RIGHT},
    };
}

namespace Pad
{
    // パッドの入力状態取得
    void Update()
    {
        // 現在のパッドの状態を取得
        int padState = GetJoypadInputState(DX_INPUT_KEY_PAD1);
        // 現在のキーボードの状態を取得
        char keyState[256];
        GetHitKeyStateAll(keyState);

        // ログの更新
        for (int i = kLogNum - 1; i >= 1; i--)
        {
            padLog[i] = padLog[i - 1];
        }
        // 最新の状態
        padLog[0] = padState;

        // 入力デバイスの判定
        bool isKeyboardInput = false;
        for (const auto& key : keyboardMapping)
        {
            if (keyState[key.second])
            {
                isKeyboardInput = true;
                break;
            }
        }

        if (isKeyboardInput)
        {
            currentInputDevice = ControllerType::KEYBOARD;
        }
        else if (padState != 0)
        {
            currentInputDevice = ControllerType::CONTROLLER;
        }
    }

    // 押し下げ判定
    bool IsPress(int button)
    {
        return (padLog[0] & button);
    }

    // トリガー判定
    bool IsTrigger(int button)
    {
        bool isNow = (padLog[0] & button);   // 現在の状態
        bool isLast = (padLog[1] & button);  // １フレーム前の状態
        return (isNow && !isLast);
    }

    // 離した判定
    bool IsRelase(int button)
    {
        bool isNow = (padLog[0] & button);   // 現在の状態
        bool isLast = (padLog[1] & button);  // １フレーム前の状態
        return (!isNow && isLast);
    }

    // コントローラの種類に対するボタンを返す
    int GetButton(const Button& type)
    {
        switch (currentInputDevice)
        {
        case ControllerType::KEYBOARD:

            return keyboardMapping.at(type);

            break;
        case ControllerType::CONTROLLER:

            return controllerMapping.at(type);

            break;
        default:

            return keyboardMapping.at(type);

            break;
        }

        return 0; // デフォルトの戻り値
    }

    // 現在の入力デバイスを取得する
    ControllerType GetCurrentInputDevice()
    {
        return currentInputDevice;
    }
}