#pragma once

#include "SceneBase.h"

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void init();
	void end();

	void update();
	void draw();


private:
	SceneBase* m_pScene;

	int m_updateTime = 0;
	int m_drawTime = 0;
};