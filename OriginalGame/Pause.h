#pragma once
#include "StateMachine.h"
#include <memory>
#include <vector>
#include "Vec2.h"

template <class TState> class StateMachine;

class ObjectManager;
class ControllerOption;

class Pause
{
public:

	// 状態
	enum class State
	{
		// 通常状態
		Normal,
		// ポーズ
		Pause,
		// 入力切替
		ChangeInput,
		// バックタイトル
		BackTitle,
	};

	// ポーズ選択
	enum class PauseSelect
	{
		// ゲーム再開
		Resume,

		// 音量調整
		Volume,

		// 入力切替
		ChangeInput,

		// タイトルへ
		Title,

		// セレクト数
		SelectNum,
	};

	// バックタイトル選択
	enum class BackTitleSelect
	{
		// はい
		Yes,

		// いいえ
		No,

		// セレクト数
		SelectNum
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
	void StateNormalEnter();
	void StateNormalUpdate();

	// ポーズステート処理
	void StatePauseEnter();
	void StatePauseUpdate();
	void StatePauseDraw();
	void StatePauseExit();

	// 入力切替ステート処理
	void StateChangeInputEnter();
	void StateChangeInputUpdate();
	void StateChangeInputDraw();


	// バックタイトルステート処理
	void StateBackTitleEnter();
	void StateBackTitleUpdate();
	void StateBackTitleDraw();

private:


	/// <summary>
	/// グラフィックのロード
	/// </summary>
	void LoadGraph();



	/// <summary>
	/// ポーズ選択更新
	/// </summary>
	void PauseSelectUpdate();
	/// <summary>
	/// ポーズ決定処理
	/// </summary>
	void PauseSelectDecision();

	/// <summary>
	/// バックタイトル選択更新
	/// </summary>
	void BackTitleSelectUpdate();
	/// <summary>
	/// バックタイトル決定処理
	/// </summary>
	void BackTitleSelectDecision();



	/// <summary>
	/// ポーズウィンドウ描画
	/// </summary>
	void DrawPauseWindow();

	/// <summary>
	/// ポーズ選択描画
	/// </summary>
	void PauseSelectDraw();

	/// <summary>
	/// バックタイトル選択描画
	/// </summary>
	void BackTitleSelectDraw();


	// ゲーム再開処理
	void ResumeProcess();
	




	// ポーズセレクトグラフィック構造体
	struct PauseSelectGraph
	{
		// グラフィック
		std::vector<int> handle;
		// 座標
		std::vector<Vec2> pos;
	};

	// バックタイトルセレクトグラフィック構造体
	struct BackTitleSelectGraph
	{
		// グラフィック
		std::vector<int> handle;
		// 座標
		std::vector<Vec2> pos;
	};

private:

	//////////////
	// 選択関連 //
	//////////////

	// ポーズ選択
	PauseSelect m_pauseSelect;

	// バックタイトル選択
	BackTitleSelect m_backTitleSelect;


	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// ポーズ選択グラフィック
	PauseSelectGraph m_pauseSelectGprah;

	// バックタイトル選択グラフィック
	BackTitleSelectGraph m_backTitleSelectGraph;

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

	// コントローラーオプションクラス
	std::shared_ptr<ControllerOption> m_pControllerOption;
};

