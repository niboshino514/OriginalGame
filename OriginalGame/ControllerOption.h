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


public:
	void InitSettingItem();// 設定項目の初期化


	// ウィンドウを閉じるかどうか
	bool GetIsCloseWindow() { return m_isCloseWindow; }
private:

	// 設定項目
	enum class SettingItemSenect
	{
		// コントローラー自動切換え
		AUTO_SWITCH,
		// コントローラータイプ選択
		CTRL_TYPE,
		// 入力デバイス切替
		INPUT_DEVICE,
		// セレクト数
		SELECT_NUM,
	};





	// グラフィック情報
	struct GraphInfo
	{
		// ハンドル
		std::vector<int> handle;

		// 座標
		std::vector<Vec2> pos;
	};

	



	// グラフィックのロード
	void Load();

	// 選択項目の更新
	void UpdateSettingItem();
	// コントローラー設定の更新
	void UpdateControllerSetting();

	void UpdateAutoSwitch();// コントローラー自動切換え
	void UpdateCtrlType();// コントローラータイプ選択
	void UpdateInputDevice();// 入力デバイス切替



	// ウィンドウの描画
	void DrawWindow();
	// 設定項目の描画
	void DrawSettingItem();
	// コントローラー設定の描画
	void DrawControllerSetting();
	// コントローラー説明の描画
	void DrawControllerExplanation();

private:

	//////////////
	// 選択関連 //
	//////////////

	// 選択項目
	SettingItemSenect m_settingItemSelect;

	// コントローラー設定
	Controller::ControllerSetting m_controllerSetting;

	// ウィンドウを閉じるかどうか
	bool m_isCloseWindow;

	//////////////////////
	// グラフィック関連 //
	//////////////////////
	
	// ウィンドウ名
	GraphInfo m_windowNameGraph;

	// 設定項目グラフィック
	GraphInfo m_settingItemGraph;

	// コントローラ設定グラフィック
	std::vector<GraphInfo> m_controllerSettingGraph;

	// 戻るグラフィック
	GraphInfo m_backGraph;

	// セレクト三角形グラフィック
	int m_selectTriangleGraph;

	// コントローラー説明グラフィック
	std::vector<int> m_controllerExplanationGraph;
};

