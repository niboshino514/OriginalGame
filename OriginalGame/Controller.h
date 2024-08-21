#pragma once
#include <unordered_map>
#include <DxLib.h>
#include <vector>

// コントローラー
class Controller
{
private:
	Controller();
	virtual ~Controller() = default;

	static Controller* m_pInstance;

public:

	// コピーインストラクタの禁止
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

	// コントローラーのボタン
	enum class ControllerButton
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

private:

	// ボタンのマッピング
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

	// ボタンのマッピング
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

	// コントローラタイプ
	enum class ControllerType
	{
		// キーボード
		KEYBOARD,
		// コントローラ
		CONTROLLER,
	};

public:



	// コントローラーの入力状態更新
	void Update();

	// キーの入力状態更新
	void KeyInputUpdate();
	// パッドの入力状態更新
	void PadInputUpdate();


	// 押され続けてるかどうか
	bool IsPress(const ControllerButton& button);
	// 押された瞬間かどうか
	bool IsTrigger(const ControllerButton& button);


private:

	// コントローラーの種類によるボタンを
	int GetButton(const ControllerButton& button);

	// キーの入力状態
	struct KeyInputState
	{
		char key[256];
		int count[256];
	};

private:

	
	// 入力ログ 0が最新の状態
	std::vector<int> m_padLog;

	// キーの状態
	KeyInputState m_keyInputState;

	// 現在の入力デバイス
	ControllerType m_currentInputDevice = ControllerType::KEYBOARD;
};