#pragma once
#include "SceneBase.h"
#include <memory>

class OpeningScreen;

class SceneOpening : public SceneBase
{
public:
	SceneOpening();
	virtual ~SceneOpening();


	virtual void Init() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;

public:



	/// <summary>
	/// �V�[���ύX
	/// </summary>
	void ChangeScene();



private:

	/// <summary>
	/// �t�F�[�h�A�E�g���A���񂾂񉹂��������Ȃ�T�E���h����
	/// </summary>
	void FadeOutSound();

private:

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// �I�[�v�j���O�X�N���[���|�C���^
	std::shared_ptr<OpeningScreen> m_pOpeningScreen;

};