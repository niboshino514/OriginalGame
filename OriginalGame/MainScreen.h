#pragma once
#include <memory>
#include "SceneMain.h"

class ObjectManager;
class SceneMain;

class MainScreen : public std::enable_shared_from_this<MainScreen>
{
public:
	MainScreen();
	virtual ~MainScreen();

	/// <summary>
	/// シーンメインクラスポインタを取得
	/// </summary>
	/// <param name="sceneMain"></param>
	void SetSceneMain(SceneMain* sceneMain) { m_pSceneMain = sceneMain; }


	void Init();
	void Update();
	void Draw();

public:

	/// <summary>
	/// シーン変更
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	void ChangeScene(const SceneMain::Scene& nextScene);
	


private:




	////////////////////
	// クラスポインタ //
	////////////////////

	// シーンメインポインタ
	SceneMain* m_pSceneMain;

	// オブジェクトファクトリーポインタ
	std::shared_ptr<ObjectManager>m_pObjectFactory;

};