#pragma once

#include "SceneBase.h"

class SceneManager
{


public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void End();

	void Update();
	void Draw();

private:

	// スクリーンタイプの変更
	void ScreenTypeChange();


private:
	// シーン
	SceneBase* m_pScene;

	// 更新処理速度
	int m_updateTime = 0;
	// 描画処理速度
	int m_drawTime = 0;
};