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

	// 設定項目
	enum class SettingItemSenect
	{
		// コントローラー自動切換え
		AUTO_SWITCH,
		// コントローラータイプ選択
		CTRL_TYPE,
		// 入力デバイス切替
		INPUT_DEVICE,
		// 戻る
		BACK,
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
	void LoadGraph();

	// 設定項目の描画
	void DrawSettingItem();
	// コントローラー設定の描画
	void DrawControllerSetting();


private:

	//////////////
	// 選択関連 //
	//////////////

	// 選択項目
	SettingItemSenect m_settingItemSenect;

	// コントローラー設定
	Controller::ControllerSetting m_controllerSetting;


	//////////////////////
	// グラフィック関連 //
	//////////////////////


	// 設定項目グラフィック
	GraphInfo m_settingItemGraph;

	// コントローラ設定グラフィック
	std::vector<GraphInfo> m_controllerSettingGraph;


};

