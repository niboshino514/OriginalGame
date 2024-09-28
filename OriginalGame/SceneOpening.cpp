#include "SceneOpening.h"
#include "SceneMain.h"
#include "Sound.h"
#include "OpeningScreen.h"

namespace
{
	// �t�F�[�h�C���A�E�g�̃X�s�[�h
	constexpr int kFadeSpeed = 10;

	// �t�F�[�h�̃J���[
	constexpr int kFadeColor = 0x000000;
}

SceneOpening::SceneOpening():
	m_pOpeningScreen(std::make_shared<OpeningScreen>())
{
}

SceneOpening::~SceneOpening()
{
}

void SceneOpening::Init()
{
	// �t�F�[�h�C���ݒ�
	SetFadeIn(kFadeSpeed, kFadeColor);

	// �I�[�v�j���O�X�N���[���Ɏ��g�̃|�C���^��n��
	m_pOpeningScreen->SetSceneOpeningPointer(this);

	// ����������
	m_pOpeningScreen->Init();
}

SceneBase* SceneOpening::Update()
{
	// �t�F�[�h����
	UpdateFade();

	// �t�F�[�h�A�E�g���A���񂾂񉹂��������Ȃ�T�E���h����
	FadeOutSound();

	// �t�F�[�h�C�����I�����Ă�����V�[���J�ڂ���
	if (IsFadeOutEnd())
	{
		// �V�[���J��
		return m_nextScene;
	}

	// �X�V����
	m_pOpeningScreen->Update();


	return this;
}

void SceneOpening::Draw()
{
	// �`�揈��
	m_pOpeningScreen->Draw();

	// �t�F�[�h�`��
	DrawFade();
}

void SceneOpening::ChangeScene()
{
	// ���̃V�[����ݒ�
	m_nextScene = new SceneMain();


	// �t�F�[�h�A�E�g�ݒ�
	SetFadeOut(kFadeSpeed, kFadeColor);
}

void SceneOpening::FadeOutSound()
{
	// �t�F�[�h�A�E�g���Ŗ�����Ώ��������Ȃ�
	if (!GetIsFadeOut())
	{
		return;
	}

	// �t�F�[�h�ɍ��킹��BGM�̉��ʂ�������
	Sound::GetInstance()->ScreenFadeBGMStop(GetFadeAlpha());
}
