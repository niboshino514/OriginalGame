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

	// ���̃V�[��
	enum class NextScene
	{
		// �Z�[�u�f�[�^�I���V�[��
		SaveDataSelect,
	};



	/// <summary>
	/// �V�[���ύX
	/// </summary>
	void ChangeScene(const NextScene& nextScene);

private:

	// �^�C�g���X�N���[���|�C���^
	std::shared_ptr<TitleScreen> m_pTitleScreen;
};