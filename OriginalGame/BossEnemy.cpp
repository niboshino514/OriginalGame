
#include "BossEnemy.h"
#include "GameData.h"
#include <DxLib.h>
#include "EvoLib.h"


namespace
{
	// 初期座標
	const Vec2 kInitPos = Vec2(100, 100);
}

namespace Move
{
	// スポーン移動速度
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
	// 座標をゲームデータに代入
	GameData::GetInstance()->SetPlayerPos(m_pos);

	// オブジェクトID設定
	m_objectID = ObjectID::BossEnemy;

	// 初期座標設定
	m_pos = kInitPos;


	// ステート初期化
	StateInit();
}

void BossEnemy::Update()
{



	// ステートマシンの更新
	m_pStateMachine.Update();
}

void BossEnemy::Draw()
{
	// カメラ座標取得
	const Vec2 cameraPos = GameData::GetInstance()->GetCameraPos();


	// ボスエネミーの描画
	DrawCircleAA(m_pos.x + cameraPos.x, m_pos.y + cameraPos.y, 50, 50, GetColor(255, 0, 0), TRUE);

}

void BossEnemy::StateInit()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// スポーン
	{
		auto enter = [this]() {this->StateSpawnEnter(); };
		auto update = [this]() {this->StateSpawnUpdate(); };
		auto exit = [this]() {this->StateSpawnExit(); };

		m_pStateMachine.AddState(State::Spawn, enter, update, dummy, exit);
	}

	// 通常
	{
		auto enter = [this]() {this->StateNormalEnter(); };
		auto update = [this]() {this->StateNormalUpdate(); };
		auto exit = [this]() {this->StateNormalExit(); };

		m_pStateMachine.AddState(State::Normal, enter, update, dummy, exit);
	}

	// ステートマシンの初期ステートを設定
	m_pStateMachine.SetState(State::Spawn);
}

void BossEnemy::StateSpawnEnter()
{
}

void BossEnemy::StateSpawnUpdate()
{

	// ボス登場座標を取得
	const Vec2 targetPos = GameData::GetInstance()->GetBossEnemySpawnPos();

	// プレイヤーの座標に向かって移動
	m_vec = EvoLib::Calculation::TargetMoveValue(m_pos, targetPos, Move::kSpawnSpeed);

	// 座標更新
	m_pos += m_vec;


	if(EvoLib::Calculation::IsTargetRangeValue(m_pos.x, targetPos.x, Move::kSpawnSpeed))
	{
		m_pos.x = targetPos.x;
	}

	if (EvoLib::Calculation::IsTargetRangeValue(m_pos.y, targetPos.y, Move::kSpawnSpeed))
	{
		m_pos.y = targetPos.y;
	}

	// 座標をゲームデータに代入
	GameData::GetInstance()->SetPlayerPos(m_pos);


	// ボス登場座標に到達したらボス会話ステートに遷移
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
