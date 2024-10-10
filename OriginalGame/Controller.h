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

	// コントローラー自動切換え
	enum class AutoSwitch
	{
		// オン
		ON,
		// オフ
		OFF,
		// セレクト数
		AUTO_SWITCH_NUM,
	};

	// コントローラタイプ
	enum class ControllerType
	{
		// キーボード
		KEYBOARD,
		// コントローラ
		CONTROLLER,
		// 項目数
		CONTROLLER_NUM,
	};

	// パッドのタイプ
	enum class PadType
	{
		// XBOXコントローラ
		XBOX,
		// デュアルショック
		DUALSHOCK,
		// SWITCHPRO
		SWITCH_PRO,
		// 項目数
		PAD_TYPE_NUM,
	};

	// 設定
	struct ControllerSetting
	{
		// 自動切換え
		AutoSwitch autoSwitch = AutoSwitch::ON;

		// パッドタイプ
		PadType padType = PadType::XBOX;

		// コントローラータイプ
		ControllerType controllerType = ControllerType::KEYBOARD;
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

	// xboxコントローラーのマッピング
	const std::unordered_map<ControllerButton, int> xboxMapping =
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

	// dualshockコントローラーマッピング
	const std::unordered_map<ControllerButton, int> dualShockMapping =
	{
		{ControllerButton::DECIDE, PAD_INPUT_2},
		{ControllerButton::CANCEL, PAD_INPUT_3},

		{ControllerButton::JUMP, PAD_INPUT_2},
		{ControllerButton::RESPAWN, PAD_INPUT_4},

		{ControllerButton::PAUSE, PAD_INPUT_10},

		{ControllerButton::UP, PAD_INPUT_UP},
		{ControllerButton::DOWN, PAD_INPUT_DOWN},
		{ControllerButton::LEFT, PAD_INPUT_LEFT},
		{ControllerButton::RIGHT, PAD_INPUT_RIGHT},
	};

	// switchProコントローラーマッピング
	const std::unordered_map<ControllerButton, int> switchProMapping =
	{
		{ControllerButton::DECIDE, PAD_INPUT_2},
		{ControllerButton::CANCEL, PAD_INPUT_1},

		{ControllerButton::JUMP, PAD_INPUT_2},
		{ControllerButton::RESPAWN, PAD_INPUT_3},

		{ControllerButton::PAUSE, PAD_INPUT_10},

		{ControllerButton::UP, PAD_INPUT_UP},
		{ControllerButton::DOWN, PAD_INPUT_DOWN},
		{ControllerButton::LEFT, PAD_INPUT_LEFT},
		{ControllerButton::RIGHT, PAD_INPUT_RIGHT},
	};



public:

	// コントローラー設定をロード
	void LoadControllerSetting();

	// コントローラー設定の書き込み
	void WriteControllerSetting();


	// コントローラーの入力状態更新
	void Update();

	// キーの入力状態更新
	void KeyInputUpdate();
	// パッドの入力状態更新
	void PadInputUpdate();


	// 押され続けてるかどうか
	bool IsPress(const ControllerButton& button);
	// 押された瞬間かどうか
	bool IsTrigger(const ControllerButton& button, bool isAllController = false);

	/// <summary>
	/// 操作を受け付けるかどうかを設定
	/// </summary>
	/// <param name="isAcceptInput">受付フラグ</param>
	void SetAcceptInput(const bool& isAcceptInput) { m_isAcceptInput = isAcceptInput; }

	/// <summary>
	/// コントローラー設定を返す
	/// </summary>
	/// <returns>コントローラー設定</returns>
	ControllerSetting GetControllerSetting() { return m_controllerSetting; }

	/// <summary>
	/// コントローラー設定をセット
	/// </summary>
	/// <param name="controllerSetting">コントローラー設定</param>
	void SetControllerSetting(const ControllerSetting& controllerSetting) { m_controllerSetting = controllerSetting; }

	/// <summary>
	/// キーが押された瞬間かどうか
	/// </summary>
	/// <param name="key">キー</param>
	bool IsTriggerKey(const int& key);

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

	// コントローラーの設定
	ControllerSetting m_controllerSetting;

	// 操作を受け付けるかどうか
	bool m_isAcceptInput;
};