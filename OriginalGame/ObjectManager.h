#pragma once

#include "StateMachine.h"
#include <memory>
#include <vector>
#include <string>
#include "Vec2.h"
#include "PlatinumLoader.h"
#include "SceneMain.h"
#include "GameData.h"

#include "EvoLib.h"

#include <tuple>


template <class TState> class StateMachine;



namespace
{
	// サウンドファイル名
	static std::vector<std::string> kSoundFileName =
	{
		"Bgm_0",		// BGM

		"Determination",// 決定音
		"Cancel",		// キャンセル音
		"Select",		// 選択音

		"Pause",		// ポーズ音

		"Jump",			// ジャンプ音
		"Dead",			// 死亡音
		"Restart",		// リスタート音
	};

	// サウンド名
	enum class SoundName
	{
		BGM,			// BGM

		Decision,		// 決定音
		Cancel,			// キャンセル音
		Select,			// 選択音

		Pause,			// ポーズ音

		Jump,			// ジャンプ音
		Dead,			// 死亡音
		Restart,		// リスタート音
	};

}



class MainScreen;
class ObjectBase;
class PlatinumLoader;
class Camera;
class Pause;
class MessageWindow;
class GameOver;

class ObjectManager : public std::enable_shared_from_this<ObjectManager>
{
public:

	// ステージ移動タイプ
	enum class MapSwitchType
	{
		Spawn,			// スポーン
		SaveData,		// セーブデータ
		Respawn,		// リスポーン
		NextStage,		// 次のステージ
		PreviouseStage	// 前のステージ
	};


	// チップタイプ
	enum class ChipType
	{
		None,				// 何もなし
		Ground,				// 地面
		NextStage,			// 次のステージ
		PreviouseStage,		// 前のステージ
		NextPos,			// 次のステージ座標
		PreviousePos,		// 前のステージ座標
		SpawnPos,			// スポーン座標
		Save,				// セーブ

		TopNeedle,			// 上針
		BottomNeedle,		// 下針
		LeftNeedle,			// 左針
		RightNeedle,		// 右針
		DiedBlock,			// 死亡ブロック
		
		TopGravity,			// 上重力
		BottomGravity,		// 下重力
		LeftGravity,		// 左重力
		RightGravity,		// 右重力

		IceBlock,			// アイスブロック
	
		TopConveyor,		// 上コンベア
		BottomConveyor,		// 下コンベア
		LeftConveyor,		// 左コンベア
		RigthConveyor,		// 右コンベア

		TransparentBlock,	// 透明ブロック
		
		InfiniteJump,		// 無限ジャンプ
		SecondJump,			// 二段ジャンプ

		StrongJumpPower,	// 強ジャンプ
		WeakJumpPower,		// 弱ジャンプ
		NormalJumpPower,		// 通常ジャンプ

		Acceleration,		// 加速
		Deceleration,		// 減速
		NormalSpeed,		// 通常速度

		BossSpawnFrag,		// ボススポーンフラグ
		BossSpawnPos,		// ボススポーン座標
		

		NotExists			// 存在しない
	};



	// マップ判定データ
	struct MapCollisionData
	{
		// チップタイプ
		ChipType chipType = ChipType::None;

		// マップ四角形情報
		Square square = Square();

		// マップ円情報
		Circle circle = Circle();

		// 画面内フラグ
		bool screenFlag = false;

		// プレイヤー範囲内フラグ
		bool playerRangeFlag = false;
	};

	// マップ情報データ
	struct MapInfoData
	{
		// マップデータのファイルパス
		std::vector<std::string> filePath;

		// マップ判定データ
		std::vector<std::vector<MapCollisionData>> mapCollisionData;

		// マップ情報
		PlatinumLoader::MapChip mapChip;
		
		// マップナンバー
		int mapNumber = 0;
	};


public:

	// ステート
	enum class State
	{
		// 設定
		Setting,
		// オープニング
		Opening,
		// 通常
		Normal,
		// ボス登場
		SpawnBoss,
		// ボス会話
		BossTalk,
		// ポーズ画面
		Pause,
		// エンディング
		Ending,
	};


public:
	ObjectManager();
	virtual ~ObjectManager();

	/// <summary>
	/// メインスクリーンポインタ取得
	/// </summary>
	/// <param name="pMainScreen">メインスクリーンポインタ</param>
	void SetMainScreenPointer(const std::shared_ptr<MainScreen>& pMainScreen) { m_pMainScreen = pMainScreen; }


	void Init();
	void Update();
	void Draw();

public:

	/// <summary>
	/// シーン変更
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	void ChangeScene(const SceneMain::Scene& nextScene);


public:

	/// <summary>
	/// セーブポイント設定
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="playerStatus">プレイヤーステータス</param>
	void SetSavePoint(const Vec2& pos, const GameData::PlayerStatus& playerStatus);

	/// <summary>
	/// ステート設定
	/// </summary>
	/// <param name="state">ステート</param>
	void SetState(const State& state);

	/// <summary>
	/// ロード
	/// </summary>
	void Load();

	/// <summary>
	/// キャラクター生成
	/// </summary>
	void PlayerCreate(const Vec2& pos);

	/// <summary>
	/// ボスエネミー生成
	/// </summary>
	/// <param name="pos">座標</param>
	void BossEnemyCreate(const Vec2& pos);


	/// <summary>
	/// マップ生成
	/// </summary>
	/// <param name="マップ情報"></param>
	void MapCollisionDataCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType);

	/// <summary>
	/// オブジェクト削除
	/// </summary>
	void ObjectErase();

	/// <summary>
	/// ステージ移動
	/// </summary>
	void StageMove(const MapSwitchType& mapSwitchType);

	/// <summary>
	/// マップ情報データを返す
	/// </summary>
	/// <returns>マップ情報データ</returns>
	MapInfoData GetMapInfoData() { return m_mapInfoData; }
	
	/// <summary>
	/// マップチップ番号を返す
	/// </summary>
	/// <returns></returns>
	std::vector<std::vector<int>>GetMapChipNumber();


	/// <summary>
	/// セーブポイントの座標を返す
	/// </summary>
	/// <returns>bool : ステージを変更するかどうか　Vec2 : 座標</returns>
	std::tuple<bool, Vec2> GetSavePointPos();

	/// <summary>
	/// ニードル情報を返す
	/// </summary>
	/// <param name="needleDirection">ニードルの向き</param>
	/// <param name="square">四角形情報</param>
	/// <returns>ニードル情報</returns>
	Triangle ChipTypeToTriangle(const ChipType& needleDirection, const Square& square);

	/// <summary>
	/// フィールドを確認する
	/// </summary>
	void FieldCheck();

	/// <summary>
	/// マップ当たり判定データを返す
	/// </summary>
	std::vector<std::vector<MapCollisionData>> GetMapCollisionData() { return m_mapInfoData.mapCollisionData; }

	/// <summary>
	/// オブジェクト情報を返す
	/// </summary>
	/// <returns>オブジェクト情報</returns>
	std::vector<std::shared_ptr<ObjectBase>> GetObjectInfo() { return m_object; }
	

	/// <summary>
	/// スポーンボスステートが終わったかどうかを返す
	/// </summary>
	/// <returns></returns>
	bool GetIsSpawnBossStateEnd() { return m_isSpawnBossStateEnd; }

	/// <summary>
	/// ボスステートにするかどうかを渡す
	/// </summary>
	/// <param name="isSpawnBossState"></param>
	void SetIsSpawnBossState(const bool& isSpawnBossState) { m_isSpawnBossState = isSpawnBossState; }

	/// <summary>
	/// ショット生成個数を設定
	/// </summary>
	void SetShotCount(const GameData::ShotData& shotData);



private:

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	// 設定ステート処理
	void StateSettingInit();

	// オープニングステート処理
	void StateOpeningEnter();
	void StateOpeningUpdate();
	void StateOpeningDraw();
	void StateOpeningExit();

	// ノーマルステート処理
	void StateNormalUpdate();
	void StateNormalDraw();

	// ボス登場ステート処理
	void StateSpawnBossEnter();
	void StateSpawnBossUpdate();
	void StateSpawnBossDraw();
	void StateSpawnBossExit();

	// ボス会話ステート処理
	void StateBossTalkEnter();
	void StateBossTalkUpdate();
	void StateBossTalkDraw();
	void StateBossTalkExit();

	// ポーズステート処理
	void StatePauseUpdate();
	void StatePauseDraw();

	// エンディングステート処理
	void StateEndingEnter();


private:

	/// <summary>
	/// マップ関連初期設定
	/// </summary>
	void InitMap();
	
	/// <summary>
	/// スクリーンサークル初期化
	/// </summary>
	void InitScreenCircle();

	/// <summary>
	/// マップデータファイルパスの初期設定
	/// </summary>
	void InitMapDataFilePath();

	/// <summary>
	/// マップチップ生成
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void MapChipCreate(const MapCollisionData& mapCollisionData);

	/// <summary>
	/// ギミックリセット処理
	/// </summary>
	void GimmickReset();

	/// <summary>
	/// セルが範囲外かどうかを確認する
	/// </summary>
	/// <param name="cell">セル</param>
	/// <returns>セルが範囲外かどうかのフラグ</returns>
	bool IsCellCheckOutOfRange(const Cell& cell);


	/// <summary>d
	/// オブジェクト更新
	/// </summary>
	/// <param name="isStopPlayer">プレイヤーを止めるかどうか</param>
	void ObjectUpdate(const bool isStopPlayer = false);

	/// <summary>
	/// オブジェクト描画
	/// </summary>
	void ObjectDraw();


	/// <summary>
	/// マップ描画
	/// </summary>
	void TestMapDraw();


	/// <summary>
	/// ショット生成
	/// </summary>
	void CreateShot();

private:

	// オブジェクト
	std::vector<std::shared_ptr<ObjectBase>>m_object;

	// マップ情報データ
	MapInfoData m_mapInfoData;

	// スクリーンサークル
	Circle m_screenCircle;

	// マップグラフィック
	std::vector<int>m_testMapGraph;

	//////////////////
	// ショット関連 //
	//////////////////

	// ショットデータ
	std::vector<GameData::ShotData> m_shotData;

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// プレイヤーグラフィックハンドル
	std::vector<int>m_playerGraphHandle;

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

	// ボスステートシーンに移行したかどうか
	bool m_isSpawnBossStateEnd;
	
	// ボスステートに移行するかどうか
	bool m_isSpawnBossState;


	////////////////////
	// クラスポインタ //
	////////////////////

	// メインスクリーン
	std::shared_ptr<MainScreen>m_pMainScreen;

	// プラチナムローダー
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

	// カメラ
	std::shared_ptr<Camera>m_pCamera;

	// ポーズ
	std::shared_ptr<Pause>m_pPause;

	// オープニングメッセージウィンドウ
	std::shared_ptr<MessageWindow>m_pOpeningMessageWindow;

	// ボス会話メッセージウィンドウ
	std::shared_ptr<MessageWindow>m_pBossTalkMessageWindow;

	// ゲームオーバー
	std::shared_ptr<GameOver>m_pGameOver;
};