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

	// 状態
	enum State
	{
		// スポーン
		Spawn,
		// 通常状態
		Normal,
		// 死亡
		Dead,
	};

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	// スポーンステート処理
	void StateSpawnEnter();
	void StateSpawnUpdate();
	void StateSpawnExit();

	// 通常ステート処理
	void StateNormalEnter();
	void StateNormalUpdate();
	void StateNormalExit();



private:





	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

};