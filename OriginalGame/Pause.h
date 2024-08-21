#pragma once
#include "StateMachine.h"
#include <memory>

template <class TState> class StateMachine;

class ObjectManager;

class Pause
{
public:

	// ���
	enum class State
	{
		// �ʏ���
		Normal,
		// ���S
		Pause,
	};

public:
	Pause();
	virtual ~Pause();

	/// <summary>
	/// �I�u�W�F�N�g�t�@�N�g���[�N���X�|�C���^���擾
	/// </summary>
	/// <param name="objectFactory">�I�u�W�F�N�g�t�@�N�g���[�|�C���^</param>
	void SetObjectFactoryPointer(const std::shared_ptr<ObjectManager>& objectFactory) { m_pObjectFactory = objectFactory; }

	void Init();
	void Update();
	void Draw();

private:

	/// <summary>
	/// �X�e�[�g������
	/// </summary>
	void StateInit();

	// �m�[�}���X�e�[�g����
	void StateNormalUpdate();

	// �|�[�Y�X�e�[�g����
	void StatePauseUpdate();
	void StatePauseDraw();

private:

	/// <summary>
	/// �|�[�Y�E�B���h�E�`��
	/// </summary>
	void DrawPauseWindow();

private:

	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g�}�V��
	StateMachine<State>m_pStateMachine;

	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �I�u�W�F�N�g�t�@�N�g���[�N���X
	std::shared_ptr<ObjectManager> m_pObjectFactory;


};

