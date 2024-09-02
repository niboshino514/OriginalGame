#include "BossSpawnFlagChip.h"


BossSpawnFlagChip::BossSpawnFlagChip()
{
}

BossSpawnFlagChip::~BossSpawnFlagChip()
{
}

void BossSpawnFlagChip::Init()
{
	// �I�u�W�F�N�gID�̐ݒ�
	m_objectID = ObjectID::BossSpawnFlagChip;
}

void BossSpawnFlagChip::Update()
{
	// �Փˏ���
	Collision();

	// �M�~�b�N�t���O�������Ă���΁A�X�e�[�g���{�X�X�|�[���ɕύX
	if (m_isGimmick &&
		!m_pObjectManager->GetIsSpawnBossState())
	{
		m_pObjectManager->SetState(ObjectManager::State::SpawnBoss);
	}
}