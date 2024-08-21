#include "SceneSaveDataSelect.h"

#include "SaveDataSelectScreen.h"

#include "SceneTitle.h"
#include "SceneMain.h"

namespace
{
	// �t�F�[�h�C���A�E�g�̃X�s�[�h
	constexpr int kFadeSpeed = 5;

	// �t�F�[�h�̃J���[
	constexpr int kFadeColor = 0x000000;
}

SceneSaveDataSelect::SceneSaveDataSelect():
	m_pSaveDataSelectScreen(std::make_shared<SaveDataSelectScreen>())
{
}

SceneSaveDataSelect::~SceneSaveDataSelect()
{
}

void SceneSaveDataSelect::Init()
{
	// �t�F�[�h�C���ݒ�
	SetFadeIn(kFadeSpeed, kFadeColor);

	// �Z�[�u�f�[�^�I���X�N���[���Ɏ��g�̃|�C���^��n��
	m_pSaveDataSelectScreen->SetSceneSaveDataSelect(this);
	// ����������
	m_pSaveDataSelectScreen->Init();
}

SceneBase* SceneSaveDataSelect::Update()
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
	m_pSaveDataSelectScreen->Update();

	return this;
}

void SceneSaveDataSelect::Draw()
{
	// �`�揈��
	m_pSaveDataSelectScreen->Draw();

	// �t�F�[�h�`��(��ԉ��ɗ���悤�ɂ���)
	DrawFade();
}

void SceneSaveDataSelect::ChangeScene(const NextScene& nextScene)
{
	// ���̃V�[����ݒ�
	switch (nextScene)
	{
	case SceneSaveDataSelect::NextScene::Title:
		// �^�C�g���V�[���ɑJ��
		m_nextScene = new SceneTitle();

		break;
	case SceneSaveDataSelect::NextScene::GameMain:
		// �Q�[�����C���V�[���ɑJ��
		m_nextScene = new SceneMain();

		break;
	default:
		break;
	}

	// �t�F�[�h�A�E�g�ݒ�
	SetFadeOut(kFadeSpeed, kFadeColor);
}
