
#include "BossEnemy.h"
#include "GameData.h"
#include <DxLib.h>
#include "EvoLib.h"


namespace
{
	// �������W
	const Vec2 kInitPos = Vec2(100, 100);
}

namespace Move
{
	// �X�|�[���ړ����x
	constexpr float kSpawnSpeed = 10.0f;
}

BossEnemy::BossEnemy()
{
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Init()
{
	// ���W���Q�[���f�[�^�ɑ��
	GameData::GetInstance()->SetPlayerPos(m_pos);

	// �I�u�W�F�N�gID�ݒ�
	m_objectID = ObjectID::BossEnemy;

	// �������W�ݒ�
	m_pos = kInitPos;


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

	// �ʏ�
	{
		auto enter = [this]() {this->StateNormalEnter(); };
		auto update = [this]() {this->StateNormalUpdate(); };
		auto exit = [this]() {this->StateNormalExit(); };

		m_pStateMachine.AddState(State::Normal, enter, update, dummy, exit);
	}

	// �X�e�[�g�}�V���̏����X�e�[�g��ݒ�
	m_pStateMachine.SetState(State::Spawn);
}

void BossEnemy::StateSpawnEnter()
{
}

void BossEnemy::StateSpawnUpdate()
{

	// �{�X�o����W���擾
	const Vec2 targetPos = GameData::GetInstance()->GetBossEnemySpawnPos();

	// �v���C���[�̍��W�Ɍ������Ĉړ�
	m_vec = EvoLib::Calculation::TargetMoveValue(m_pos, targetPos, Move::kSpawnSpeed);

	// ���W�X�V
	m_pos += m_vec;


	if(EvoLib::Calculation::IsTargetRangeValue(m_pos.x, targetPos.x, Move::kSpawnSpeed))
	{
		m_pos.x = targetPos.x;
	}

	if (EvoLib::Calculation::IsTargetRangeValue(m_pos.y, targetPos.y, Move::kSpawnSpeed))
	{
		m_pos.y = targetPos.y;
	}

	// ���W���Q�[���f�[�^�ɑ��
	GameData::GetInstance()->SetPlayerPos(m_pos);


	// �{�X�o����W�ɓ��B������{�X��b�X�e�[�g�ɑJ��
	if (m_pos.x == targetPos.x &&
		m_pos.y == targetPos.y)
	{
		m_pObjectManager->SetState(ObjectManager::State::BossTalk);
		m_pStateMachine.SetState(State::Normal);
	}
}

void BossEnemy::StateSpawnExit()
{
}

void BossEnemy::StateNormalEnter()
{
}

void BossEnemy::StateNormalUpdate()
{
}

void BossEnemy::StateNormalExit()
{
}
