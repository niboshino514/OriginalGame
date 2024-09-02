#include "BossSpawnFlagChip.h"


BossSpawnFlagChip::BossSpawnFlagChip()
{
}

BossSpawnFlagChip::~BossSpawnFlagChip()
{
}

void BossSpawnFlagChip::Init()
{
	// オブジェクトIDの設定
	m_objectID = ObjectID::BossSpawnFlagChip;
}

void BossSpawnFlagChip::Update()
{
	// 衝突処理
	Collision();

	// ギミックフラグが立っていれば、ステートをボススポーンに変更
	if (m_isGimmick &&
		!m_pObjectManager->GetIsSpawnBossState())
	{
		m_pObjectManager->SetState(ObjectManager::State::SpawnBoss);
	}
}