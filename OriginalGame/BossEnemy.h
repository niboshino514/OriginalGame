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

	// 状態
	enum State
	{
		// スポーン
		Spawn,

		// フェーズ移行
		Phase,

		// 移動
		Move,

		// 攻撃
		Attack,

		// 死亡
		Dead,
	};

	// ショットタイプ
	enum class ShotType
	{
		// 通常
		Normal,
	};


	// 攻撃データ
	struct AttackData
	{
		// ショットを撃つ準備時間
		int shotSetupTime = 0;

		// ショットの最大回数
		int shotMaxCountNum = 0;
		
		// ショットの個数
		int shotNum = 0;

		// ショットデータ
		GameData::ShotData shotData = GameData::ShotData();
	};



	


	








	// ステート変更
	void ChangeState(const State& state);

private:

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	// スポーンステート処理
	void StateSpawnEnter();
	void StateSpawnUpdate();
	void StateSpawnExit();

	
	// フェーズ移行ステート処理
	void StatePhaseEnter();
	
	// 移動ステート処理
	void StateMoveEnter();
	void StateMoveUpdate();
	void StateMoveExit();

	// 攻撃ステート処理
	void StateAttackEnter();
	void StateAttackUpdate();
	void StateAttackExit();


private:

	// ショット攻撃
	void ShotAttack();


private:

	//////////////
	// 攻撃関連 //
	//////////////

	// 攻撃データ
	AttackData m_attackData;

	// ショットのフレームカウント
	int m_shotFrameCount;

	// ショットのカウント
	int m_shotCount;
	

	//////////////
	// 移動関連 //
	//////////////

	// イージングデータ
	EvoLib::Calculation::EasingData m_easingData;


	int m_count;

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

};