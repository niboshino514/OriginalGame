#pragma once
#include "SceneBase.h"
#include <memory>

class SaveDataSelectScreen;

class SceneSaveDataSelect : public SceneBase
{
public:
	SceneSaveDataSelect();
	virtual ~SceneSaveDataSelect();

	virtual void Init()override;
	virtual SceneBase* Update()override;
	virtual void Draw()override;

public:

	// ���̃V�[��
	enum class NextScene
	{
		// �^�C�g���V�[��
		Title,

		// �Q�[�����C���V�[��
		GameMain,
	};

	/// <summary>
	/// �V�[���ύX
	/// </summary>
	void ChangeScene(const NextScene& nextScene);

private:

	// �Z�[�u�f�[�^�I���X�N���[���|�C���^
	std::shared_ptr<SaveDataSelectScreen> m_pSaveDataSelectScreen;
};