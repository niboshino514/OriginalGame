#pragma once

class SceneSaveDataSelect;

class SaveDataSelectScreen
{
public:
	SaveDataSelectScreen();
	~SaveDataSelectScreen();

	/// <summary>
	/// シーンセーブデータ選択のポインタを取得
	/// </summary>
	/// <param name="sceneSaveDataSelect">シーンセーブデータ選択ポインタ</param>
	void SetSceneSaveDataSelect(SceneSaveDataSelect* sceneSaveDataSelect) { m_pSceneSaveDataSelect = sceneSaveDataSelect; }

	void Init();
	void Update();
	void Draw();

private:

	// シーンセーブデータ選択ポインタ
	SceneSaveDataSelect* m_pSceneSaveDataSelect;

};

