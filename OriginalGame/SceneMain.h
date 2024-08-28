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

	// �V�[��
	enum class Scene
	{
		// �^�C�g���V�[��
		Title,

	};

	/// <summary>
	/// �V�[���ύX
	/// </summary>
	void ChangeScene(const Scene& nextScene);



private:

	/// <summary>
	/// �t�F�[�h�A�E�g���A���񂾂񉹂��������Ȃ�T�E���h����
	/// </summary>
	void FadeOutSound();

private:

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// ���C���X�N���[��
	std::shared_ptr<MainScreen>m_pMainScreen;




};