#pragma once
#include "SceneBase.h"
#include <memory>

class EndingScreen;

class SceneEnding : public SceneBase
{
public:
	SceneEnding();

	virtual ~SceneEnding();

	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;

public:

	// シーン
	enum class Scene
	{
		// タイトルシーン
		Title,

	};

	/// <summary>
	/// シーン変更
	/// </summary>
	void ChangeScene(const Scene& nextScene);

private:

	// エンディングスクリーンポインタ
	std::shared_ptr<EndingScreen> m_pEndingScreen;
};

