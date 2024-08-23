#pragma once
#include "SceneBase.h"
#include <memory>


class MainScreen;

class SceneMain : public SceneBase
{
public:
	SceneMain();

	virtual ~SceneMain();


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


	////////////////////
	// クラスポインタ //
	////////////////////

	// メインスクリーン
	std::shared_ptr<MainScreen>m_pMainScreen;




};