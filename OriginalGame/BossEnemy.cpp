
#include "BossEnemy.h"
#include "GameData.h"
#include <DxLib.h>
#include "EvoLib.h"
#include "game.h"

namespace
{
	// 初期座標
	const Vec2 kInitPos = Vec2(Game::kScreenWidth, 0.0f);

}


namespace Spawn
{
	// イージングデータ
	EvoLib::Calculation::EasingData kEasingData =
	{		
		// 現在のフレーム数
		0.0f,
		// 移動にかかる最大フレーム数
		40.0f,
		// 移動開始座標
		kInitPos,
		// 移動終了座標
		Vec2(),
	};
}

namespace Move
{
	// 移動イージングデータ
	struct MoveEasingData
	{
		// イージングデータ
		std::vector<EvoLib::Calculation::EasingData> easingData;
	};

	MoveEasingData kMoveEasingData[] =
	{
		{
			{
				// 移動1
				{
					// 現在のフレーム数
					0.0f,
					// 移動にかかる最大フレーム数
					40.0f,
					// 移動開始座標
					Vec2(),
					// 移動終了座標
					Vec2(100,200),
				},
				// 移動2
				{
					// 現在のフレーム数
					0.0f,
					// 移動にかかる最大フレーム数
					40.0f,
					// 移動開始座標
					Vec2(),
					// 移動終了座標
					Vec2(300,500),
				},
				// 移動3
				{
					// 現在のフレーム数
					0.0f,
					// 移動にかかる最大フレーム数
					40.0f,
					// 移動開始座標
					Vec2(),
					// 移動終了座標
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
				// ショットを撃つ準備時間
				20,
				// ショットの最大回数
				2,
				// ショットの個数
				4,
				// ショットデータ
				GameData::ShotData
				{
					// ショットの種類
					GameData::ShotType::PlayerShot,
					Vec2(),
					// ショットベースアングル
					0.0f,
					// ショットスピード
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
	// オブジェクトID設定
	m_objectID = ObjectID::BossEnemy;

	// 初期座標設定
	m_pos = kInitPos;

	m_count = 0;

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

void BossEnemy::ChangeState(const State& state)
{
	// ステート変更
	m_pStateMachine.SetState(state);
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

	// フェーズ移行
	{
		auto enter = [this]() {this->StatePhaseEnter(); };

		m_pStateMachine.AddState(State::Phase, enter, dummy, dummy, dummy);
	}

	// 移動
	{
		auto enter = [this]() {this->StateMoveEnter(); };
		auto update = [this]() {this->StateMoveUpdate(); };
		auto exit = [this]() {this->StateMoveExit(); };

		m_pStateMachine.AddState(State::Move, enter, update, dummy, exit);
	}

	// 攻撃
	{
		auto enter = [this]() {this->StateAttackEnter(); };
		auto update = [this]() {this->StateAttackUpdate(); };
		auto exit = [this]() {this->StateAttackExit(); };

		m_pStateMachine.AddState(State::Attack, enter, update, dummy, exit);
	}

	// ステートマシンの初期ステートを設定
	m_pStateMachine.SetState(State::Spawn);
}

void BossEnemy::StateSpawnEnter()
{

	// イージングデータを設定
	m_easingData = Spawn::kEasingData;

	// 終点座標を設定
	m_easingData.endPos = GameData::GetInstance()->GetBossEnemySpawnPos();
}

void BossEnemy::StateSpawnUpdate()
{
	// イージング移動
	m_pos = EvoLib::Calculation::EasingInOutSine(m_easingData);

	// ボス登場座標に到達したらボス会話ステートに遷移
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

	// ショットを撃つ座標を設定
	m_attackData.shotData.startPos = m_pos;

	m_shotFrameCount = 0;

	m_shotCount = 0;
}

void BossEnemy::StateAttackUpdate()
{



	// ショットのフレーム時間がショットの準備時間に達したらショットを撃つ
	m_shotFrameCount++;

	// ショットのフレーム時間がショットの準備時間に達したらショットを撃つ
	if(m_shotFrameCount >= m_attackData.shotSetupTime)
	{
		// ショットの個数がショットの最大数に達したらフェーズステートに遷移
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

	// 角度リスト
	std::vector<float> angleList =
		EvoLib::Calculation::AngleDivision(m_attackData.shotNum, shotData.angle);

	for (auto shot : angleList)
	{
		shotData.angle = shot;

		m_pObjectManager->SetShotCount(shotData);
	}
}

