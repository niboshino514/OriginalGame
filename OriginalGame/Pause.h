#pragma once
#include "StateMachine.h"
#include <memory>
#include <vector>
#include "Vec2.h"

template <class TState> class StateMachine;

class ObjectManager;
class ControllerOption;
class SoundOption;

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

		// 音量調整
		SoundOption,

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

	// 音量調整ステート処理
	void StateSoundOptionEnter();
	void StateSoundOptionUpdate();
	void StateSoundOptionDraw();


	// バックタイトルステート処理
	void StateBackTitleEnter();
	void StateBackTitleUpdate();
	void StateBackTitleDraw();

private:


	/// <summary>
	/// グラフィックのロード
	/// </summary>
	void Load();



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

	// ゲーム再開処理
	void ResumeProcess();

	/// <summary>
	/// ポーズウィンドウ描画
	/// </summary>
	void DrawPauseWindow();

	/// <summary>
	/// ポーズ選択描画
	/// </summary>
	void DrawPauseSelect();

	// タイトルに戻るウィンドウ描画
	void DrawBackTitleWindow();

	/// <summary>
	/// バックタイトル選択描画
	/// </summary>
	void DrawBackTitleSelect();
	







	// グラフィック情報
	struct GraphInfo
	{
		// ハンドル
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

	// ポーズウィンドウ名グラフィック
	GraphInfo m_windowNameGraph;

	// ポーズ選択グラフィック
	GraphInfo m_pauseSelectGprah;

	// バックタイトルグラフィック
	GraphInfo m_backTitleGraph;

	// バックタイトル選択グラフィック
	GraphInfo m_backTitleSelectGraph;

	// セレクト三角形グラフィック
	int m_selectTriangleGraph;

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

	// サウンドオプションクラス
	std::shared_ptr<SoundOption> m_pSoundOption;
};

