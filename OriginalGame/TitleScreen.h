#pragma once
#include "StateMachine.h"
#include <vector>
#include "Vec2.h"
#include "EvoLib.h"
#include "GameData.h"

template <class TState> class StateMachine;

class SceneTitle;

class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	/// <summary>
	/// シーンタイトルのポインタを取得
	/// </summary>
	/// <param name="sceneTitle">シーンタイトルポインタ</param>
	void SetSceneTitle(SceneTitle* sceneTitle) { m_pSceneTitle = sceneTitle; }

	void Init();
	void Update();
	void Draw();

public:

	// 状態
	enum State
	{
		// 入力待ち
		WaitInput,
		// 選択中
		Selecting,
	};

	// 選択
	enum class Select
	{
		// 続きから
		Continue,
		// 新規ゲーム
		NewGame,
		// ゲーム終了
		GameEnd,
		// セレクト数
		SelectNum,
	};

private:

	/// <summary>
	/// グラフィックの読み込み
	/// </summary>
	void Load();


	/// <summary>
	/// ステートの初期化
	/// </summary>
	void StateInit();

	// 入力待ちステート処理
	void StateWaitEnter();
	void StateWaitUpdate();
	void StateWaitDraw();
	void StateWaitExit();

	// 選択中ステート処理
	void StateSelectEnter();
	void StateSelectUpdate();
	void StateSelectDraw();
	void StateSelectExit();

	// セーブデータ確認
	void CheckSaveData();

	// スコアデータ確認
	void CheckScoreData();


	// 選択処理
	void SelectProcess();
	
	// 決定処理
	void DecideProcess();

	// スコアの描画
	void DrawScore();

private:


	////////////////
	// スコア関連 //
	////////////////


	GameData::ScoreData m_scoreData;	// スコア

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// 選択グラフィックハンドル
	EvoLib::Load::DivGraphInfo m_selectGraphInfo;

	// セレクト三角形グラフィック
	int m_selectTriangleGraph;

	//////////////
	// 選択関連 //
	//////////////

	// 続きからプレイ可能かどうか
	bool m_isContinue;

	// 選択
	Select m_select;

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;


	// シーンタイトルポインタ
	SceneTitle* m_pSceneTitle;
};