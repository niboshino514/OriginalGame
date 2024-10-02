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

	// シーン
	enum class Scene
	{
		// ゲームメインシーン
		GameMain,
	};



	/// <summary>
	/// シーン変更
	/// </summary>
	void ChangeScene(const Scene& nextScene);


	

private:

	// タイトルスクリーンポインタ
	std::shared_ptr<TitleScreen> m_pTitleScreen;
};