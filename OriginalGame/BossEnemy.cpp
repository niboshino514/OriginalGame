
#include "BossEnemy.h"
#include "GameData.h"
#include <DxLib.h>
#include "EvoLib.h"
#include "game.h"

namespace
{
	// �������W
	const Vec2 kInitPos = Vec2(Game::kScreenWidth, 0.0f);

}


namespace Spawn
{
	// �C�[�W���O�f�[�^
	EvoLib::Calculation::EasingData kEasingData =
	{		
		// ���݂̃t���[����
		0.0f,
		// �ړ��ɂ�����ő�t���[����
		40.0f,
		// �ړ��J�n���W
		kInitPos,
		// �ړ��I�����W
		Vec2(),
	};
}

namespace Move
{
	// �ړ��C�[�W���O�f�[�^
	struct MoveEasingData
	{
		// �C�[�W���O�f�[�^
		std::vector<EvoLib::Calculation::EasingData> easingData;
	};

	MoveEasingData kMoveEasingData[] =
	{
		{
			{
				// �ړ�1
				{
					// ���݂̃t���[����
					0.0f,
					// �ړ��ɂ�����ő�t���[����
					40.0f,
					// �ړ��J�n���W
					Vec2(),
					// �ړ��I�����W
					Vec2(100,200),
				},
				// �ړ�2
				{
					// ���݂̃t���[����
					0.0f,
					// �ړ��ɂ�����ő�t���[����
					40.0f,
					// �ړ��J�n���W
					Vec2(),
					// �ړ��I�����W
					Vec2(300,500),
				},
				// �ړ�3
				{
					// ���݂̃t���[����
					0.0f,
					// �ړ��ɂ�����ő�t���[����
					40.0f,
					// �ړ��J�n���W
					Vec2(),
					// �ړ��I�����W
					Vec2(700,400),
				},
			}
		}
	};

}


namespace Shot
{

	


	std::vector<std::vector<BossEnemy::AttackData>> kShotData =
	{
		{
			{
				// �V���b�g������������
				20,
				// �V���b�g�̍ő��
				2,
				// �V���b�g�̌�
				4,
				// �V���b�g�f�[�^
				GameData::ShotData
				{
					// �V���b�g�̎��
					GameData::ShotType::PlayerShot,
					Vec2(),
					// �V���b�g�x�[�X�A���O��
					0.0f,
					// �V���b�g�X�s�[�h
					5.0f,
				}
			}
		}
	
	
	};

}



BossEnemy::BossEnemy()
{
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Init()
{
	// �I�u�W�F�N�gID�ݒ�
	m_objectID = ObjectID::BossEnemy;

	// �������W�ݒ�
	m_pos = kInitPos;

	m_count = 0;

	// �X�e�[�g������
	StateInit();
}

void BossEnemy::Update()
{
	// �X�e�[�g�}�V���̍X�V
	m_pStateMachine.Update();
}

void BossEnemy::Draw()
{
	// �J�������W�擾
	const Vec2 cameraPos = GameData::GetInstance()->GetCameraPos();


	// �{�X�G�l�~�[�̕`��
	DrawCircleAA(m_pos.x + cameraPos.x, m_pos.y + cameraPos.y, 50, 50, GetColor(255, 0, 0), TRUE);

}

void BossEnemy::ChangeState(const State& state)
{
	// �X�e�[�g�ύX
	m_pStateMachine.SetState(state);
}

void BossEnemy::StateInit()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �X�|�[��
	{
		auto enter = [this]() {this->StateSpawnEnter(); };
		auto update = [this]() {this->StateSpawnUpdate(); };
		auto exit = [this]() {this->StateSpawnExit(); };

		m_pStateMachine.AddState(State::Spawn, enter, update, dummy, exit);
	}

	// �t�F�[�Y�ڍs
	{
		auto enter = [this]() {this->StatePhaseEnter(); };

		m_pStateMachine.AddState(State::Phase, enter, dummy, dummy, dummy);
	}

	// �ړ�
	{
		auto enter = [this]() {this->StateMoveEnter(); };
		auto update = [this]() {this->StateMoveUpdate(); };
		auto exit = [this]() {this->StateMoveExit(); };

		m_pStateMachine.AddState(State::Move, enter, update, dummy, exit);
	}

	// �U��
	{
		auto enter = [this]() {this->StateAttackEnter(); };
		auto update = [this]() {this->StateAttackUpdate(); };
		auto exit = [this]() {this->StateAttackExit(); };

		m_pStateMachine.AddState(State::Attack, enter, update, dummy, exit);
	}

	// �X�e�[�g�}�V���̏����X�e�[�g��ݒ�
	m_pStateMachine.SetState(State::Spawn);
}

void BossEnemy::StateSpawnEnter()
{

	// �C�[�W���O�f�[�^��ݒ�
	m_easingData = Spawn::kEasingData;

	// �I�_���W��ݒ�
	m_easingData.endPos = GameData::GetInstance()->GetBossEnemySpawnPos();
}

void BossEnemy::StateSpawnUpdate()
{
	// �C�[�W���O�ړ�
	m_pos = EvoLib::Calculation::EasingInOutSine(m_easingData);

	// �{�X�o����W�ɓ��B������{�X��b�X�e�[�g�ɑJ��
	if (m_easingData.currentFrame == m_easingData.totalFrame)
	{
		m_pObjectManager->SetState(ObjectManager::State::BossTalk);
	}



}

void BossEnemy::StateSpawnExit()
{
}

void BossEnemy::StatePhaseEnter()
{
	
	m_easingData = Move::kMoveEasingData[0].easingData[m_count];

	m_easingData.startPos = m_pos;


	m_count++;

	m_count %= static_cast<int>(Move::kMoveEasingData[0].easingData.size());


	ChangeState(State::Move);
}

void BossEnemy::StateMoveEnter()
{
}

void BossEnemy::StateMoveUpdate()
{
	m_pos = EvoLib::Calculation::EasingInOutSine(m_easingData);

	if (m_easingData.currentFrame == m_easingData.totalFrame)
	{
		ChangeState(State::Attack);
	}
}

void BossEnemy::StateMoveExit()
{

	
}

void BossEnemy::StateAttackEnter()
{
	m_attackData = Shot::kShotData[0][0];

	// �V���b�g�������W��ݒ�
	m_attackData.shotData.startPos = m_pos;

	m_shotFrameCount = 0;

	m_shotCount = 0;
}

void BossEnemy::StateAttackUpdate()
{



	// �V���b�g�̃t���[�����Ԃ��V���b�g�̏������ԂɒB������V���b�g������
	m_shotFrameCount++;

	// �V���b�g�̃t���[�����Ԃ��V���b�g�̏������ԂɒB������V���b�g������
	if(m_shotFrameCount >= m_attackData.shotSetupTime)
	{
		// �V���b�g�̌����V���b�g�̍ő吔�ɒB������t�F�[�Y�X�e�[�g�ɑJ��
		if(m_shotCount >= m_attackData.shotMaxCountNum)
		{
			ChangeState(State::Phase);
			return;
		}

		ShotAttack();
		m_shotFrameCount = 0;
		m_shotCount++;
	}

}

void BossEnemy::StateAttackExit()
{
}

void BossEnemy::ShotAttack()
{

	GameData::ShotData shotData = m_attackData.shotData;

	// �p�x���X�g
	std::vector<float> angleList =
		EvoLib::Calculation::AngleDivision(m_attackData.shotNum, shotData.angle);

	for (auto shot : angleList)
	{
		shotData.angle = shot;

		m_pObjectManager->SetShotCount(shotData);
	}
}

