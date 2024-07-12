#pragma once
#include "SceneBase.h"
#include <memory>


class MainScreen;

class SceneMain : public SceneBase
{
public:
	SceneMain();

	virtual ~SceneMain();


	virtual void init() override;
	virtual void end() override {}

	virtual SceneBase* update() override;
	virtual void draw() override;
private:


	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// ���C���X�N���[��
	std::shared_ptr<MainScreen>m_pMainScreen;




};