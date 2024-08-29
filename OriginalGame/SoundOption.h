#pragma once
#include <vector>
#include <Vec2.h>



class SoundOption
{
public:
	SoundOption();
	virtual ~SoundOption();


	void Init();
	void Update();
	void Draw();


	// グラフィック情報
	struct GraphInfo
	{
		// ハンドル
		std::vector<int> handle;

		// 座標
		std::vector<Vec2> pos;
	};

	void InitSettingItem();// 設定項目の初期化


	// ウィンドウを閉じるかどうか
	bool GetIsCloseWindow() { return m_isCloseWindow; }
private:

	// グラフィックのロード
	void Load();


	// 選択項目の更新
	void UpdateSettingItem();
	// サウンドBGMボリュームの更新
	void UpdateSoundBgmVolume();
	// サウンドSEボリュームの更新
	void UpdateSoundSeVolume();

	// ウィンドウの描画
	void DrawWindow();

	// 設定項目の描画
	void DrawSettingItem();
	// サウンドBGMボリュームの描画
	void DrawSoundBgmVolume();
	// サウンドSEボリュームの描画
	void DrawSoundSeVolume();




private:

	// 設定項目
	enum class SettingItemSenect
	{
		// BGM調整
		BGM,
		// SE調整
		SE,
		// セレクト数
		SELECT_NUM,
	};





	struct SoundVolume
	{
		// BGMのボリューム
		int bgmVolume;
		// SEのボリューム
		int seVolume;
	};

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// ウィンドウ名
	GraphInfo m_windowNameGraph;

	// 設定項目グラフィック
	GraphInfo m_settingItemGraph;

	// サウンドボリュームグラフィック
	GraphInfo m_soundVolumeGraph;

	// 戻るグラフィック
	GraphInfo m_backGraph;

	// 選択三角形グラフィック
	int m_selectTriangleGraph;

	//////////////
	// 選択関連 //
	//////////////

	// 選択項目
	SettingItemSenect m_settingItemSelect;

	// サウンドボリューム
	SoundVolume m_soundVolume;

	// ウィンドウを閉じるかどうか
	bool m_isCloseWindow;
};