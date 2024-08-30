#pragma once
#include <memory>

class SceneOpening;
class MessageWindow;

class OpeningScreen : public std::enable_shared_from_this<OpeningScreen>
{
public:
	OpeningScreen();
	virtual ~OpeningScreen();

	/// <summary>
	/// �V�[���I�[�v�j���O�|�C���^��ݒ�
	/// </summary>
	/// <param name="pSceneOpening">�V�[���I�[�v�j���O�|�C���^</param>
	void SetSceneOpeningPointer(SceneOpening* pSceneOpening) { m_pSceneOpening = pSceneOpening; }

	void Init();
	void Update();
	void Draw();	

private:

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// �V�[���I�[�v�j���O�|�C���^
	SceneOpening* m_pSceneOpening;
	
	// ���b�Z�[�W�E�B���h�E
	std::shared_ptr<MessageWindow> m_pMessageWindow;

};

