#pragma once
#include <memory>

class SceneEnding;

class EndingScreen
{
public:
	EndingScreen();
	virtual ~EndingScreen();

	/// <summary>
	/// シーンエンディングのポインタを取得
	/// </summary>
	/// <param name="sceneEnding">シーンエンディングポインター</param>
	void SetSceneEndingPointer(SceneEnding* sceneEnding) { m_pSceneEnding = sceneEnding; }

	void Init();
	void Update();
	void Draw();


private:

	// シーンエンディングポインタ
	SceneEnding* m_pSceneEnding;
};

