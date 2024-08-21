#pragma once
#include "StateMachine.h"
#include <memory>

template <class TState> class StateMachine;

class ObjectManager;

class Pause
{
public:

	// 状態
	enum class State
	{
		// 通常状態
		Normal,
		// 死亡
		Pause,
	};

public:
	Pause();
	virtual ~Pause();

	/// <summary>
	/// オブジェクトファクトリークラスポインタを取得
	/// </summary>
	/// <param name="objectFactory">オブジェクトファクトリーポインタ</param>
	void SetObjectFactoryPointer(const std::shared_ptr<ObjectManager>& objectFactory) { m_pObjectFactory = objectFactory; }

	void Init();
	void Update();
	void Draw();

private:

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	// ノーマルステート処理
	void StateNormalUpdate();

	// ポーズステート処理
	void StatePauseUpdate();
	void StatePauseDraw();

private:

	/// <summary>
	/// ポーズウィンドウ描画
	/// </summary>
	void DrawPauseWindow();

private:

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// オブジェクトファクトリークラス
	std::shared_ptr<ObjectManager> m_pObjectFactory;


};

