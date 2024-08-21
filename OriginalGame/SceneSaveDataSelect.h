#pragma once
#include "SceneBase.h"
#include <memory>

class SaveDataSelectScreen;

class SceneSaveDataSelect : public SceneBase
{
public:
	SceneSaveDataSelect();
	virtual ~SceneSaveDataSelect();

	virtual void Init()override;
	virtual SceneBase* Update()override;
	virtual void Draw()override;

public:

	// 次のシーン
	enum class NextScene
	{
		// タイトルシーン
		Title,

		// ゲームメインシーン
		GameMain,
	};

	/// <summary>
	/// シーン変更
	/// </summary>
	void ChangeScene(const NextScene& nextScene);

private:

	// セーブデータ選択スクリーンポインタ
	std::shared_ptr<SaveDataSelectScreen> m_pSaveDataSelectScreen;
};