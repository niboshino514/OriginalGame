#include "SceneTitle.h"

#include "TitleScreen.h"

#include "SceneSaveDataSelect.h"
#include "SceneMain.h"

namespace
{
	// �t�F�[�h�C���A�E�g�̃X�s�[�h
	constexpr int kFadeSpeed = 5;

	// �t�F�[�h�̃J���[
	constexpr int kFadeColor = 0x000000;
}

SceneTitle::SceneTitle():
	m_pTitleScreen(std::make_shared<TitleScreen>())
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// �t�F�[�h�C���ݒ�
	SetFadeIn(kFadeSpeed, kFadeColor);



	// �^�C�g���X�N���[���Ɏ��g�̃|�C���^��n��
	m_pTitleScreen->SetSceneTitle(this);
	// ����������
	m_pTitleScreen->Init();
}

SceneBase* SceneTitle::Update()
{
	// �t�F�[�h����
	UpdateFade();


	// �t�F�[�h�C�����I�����Ă�����V�[���J�ڂ���
	if (IsFadeOutEnd())
	{
		// �V�[���J��
		return m_nextScene;
	}

	// �X�V����
	m_pTitleScreen->Update();	



	return this;
}

void SceneTitle::Draw()
{
	// �`�揈��
	m_pTitleScreen->Draw();

	// �t�F�[�h�`��(��ԉ��ɗ���悤�ɂ���)
	DrawFade();
}

void SceneTitle::ChangeScene(const NextScene& nextScene)
{
	// ���̃V�[����ݒ�
	switch (nextScene)
	{
	case SceneTitle::NextScene::SaveDataSelect:

		// �Z�[�u�f�[�^�I���V�[��
		m_nextScene = new SceneSaveDataSelect();

		break;
	default:
		break;
	}



	// �t�F�[�h�A�E�g�ݒ�
	SetFadeOut(kFadeSpeed, kFadeColor);
}

