#pragma once
#include "SceneBase.h"
#include <memory>

class TitleScreen;

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init()override;
	virtual SceneBase* Update()override;
	virtual void Draw()override;


public:

	// 次のシーン
	enum class NextScene
	{
		// セーブデータ選択シーン
		SaveDataSelect,
	};



	/// <summary>
	/// シーン変更
	/// </summary>
	void ChangeScene(const NextScene& nextScene);

private:

	// タイトルスクリーンポインタ
	std::shared_ptr<TitleScreen> m_pTitleScreen;
};