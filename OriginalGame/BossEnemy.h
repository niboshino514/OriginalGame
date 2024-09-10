#pragma once
#include "ObjectBase.h"
#include "GameData.h"

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

		// �t�F�[�Y�ڍs
		Phase,

		// �ړ�
		Move,

		// �U��
		Attack,

		// ���S
		Dead,
	};

	// �V���b�g�^�C�v
	enum class ShotType
	{
		// �ʏ�
		Normal,
	};


	// �U���f�[�^
	struct AttackData
	{
		// �V���b�g������������
		int shotSetupTime = 0;

		// �V���b�g�̍ő��
		int shotMaxCountNum = 0;
		
		// �V���b�g�̌�
		int shotNum = 0;

		// �V���b�g�f�[�^
		GameData::ShotData shotData = GameData::ShotData();
	};



	


	








	// �X�e�[�g�ύX
	void ChangeState(const State& state);

private:

	/// <summary>
	/// �X�e�[�g������
	/// </summary>
	void StateInit();

	// �X�|�[���X�e�[�g����
	void StateSpawnEnter();
	void StateSpawnUpdate();
	void StateSpawnExit();

	
	// �t�F�[�Y�ڍs�X�e�[�g����
	void StatePhaseEnter();
	
	// �ړ��X�e�[�g����
	void StateMoveEnter();
	void StateMoveUpdate();
	void StateMoveExit();

	// �U���X�e�[�g����
	void StateAttackEnter();
	void StateAttackUpdate();
	void StateAttackExit();


private:

	// �V���b�g�U��
	void ShotAttack();


private:

	//////////////
	// �U���֘A //
	//////////////

	// �U���f�[�^
	AttackData m_attackData;

	// �V���b�g�̃t���[���J�E���g
	int m_shotFrameCount;

	// �V���b�g�̃J�E���g
	int m_shotCount;
	

	//////////////
	// �ړ��֘A //
	//////////////

	// �C�[�W���O�f�[�^
	EvoLib::Calculation::EasingData m_easingData;


	int m_count;

	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g�}�V��
	StateMachine<State>m_pStateMachine;

};