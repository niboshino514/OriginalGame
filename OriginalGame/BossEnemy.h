#pragma once
#include "ObjectBase.h"

template <class TState> class StateMachine;

class BossEnemy : public ObjectBase
{
public :
	BossEnemy();
	virtual ~BossEnemy();

	void Init() override;
	void Update() override;
	void Draw() override;

	// ���
	enum State
	{
		// �X�|�[��
		Spawn,
		// �ʏ���
		Normal,
		// ���S
		Dead,
	};

	/// <summary>
	/// �X�e�[�g������
	/// </summary>
	void StateInit();

	// �X�|�[���X�e�[�g����
	void StateSpawnEnter();
	void StateSpawnUpdate();
	void StateSpawnExit();




private:






	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g�}�V��
	StateMachine<State>m_pStateMachine;

};