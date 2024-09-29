#pragma once
#include "EvoLib.h"
#include "Vec2.h"
#include <tuple>

// シングルトン
class GameData
{
private:
	GameData() = default;
	virtual ~GameData() = default;

	static GameData* m_pInstance;

public:

	// コピーインストラクタの禁止
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;
	GameData(GameData&&) = delete;
	GameData& operator=(GameData&&) = delete;

	static GameData* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new GameData();
		}

		return m_pInstance;
	}

	static void DeleteInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:

	// ジャンプタイプ
	enum class JumpType
	{
		Infinite,	// 無限ジャンプ
		Second,	// 2段ジャンプ
	};

	// ジャンプ力
	enum class JumpPower
	{
		// 通常
		Normal,
		// 強
		Strong,
		// 弱
		Weak,
	};

	// 移動速度
	enum class MoveSpeed
	{
		Normal,// 通常
		Slow,// 遅い
		Fast,// 速い
	};

	// プレイヤーのステータス
	struct PlayerStatus
	{
		// 重力方向
		Direction gravityDirection = Direction::Bottom;

		// ジャンプタイプ
		JumpType jumpType = JumpType::Second;

		// ジャンプ力
		JumpPower jumpPower = JumpPower::Normal;

		// 加速フラグ
		MoveSpeed moveSpeed = MoveSpeed::Normal;

		// 死亡回数
		int deathCount = 0;
	};


	// セーブポイントデータ
	struct SaveData
	{
		// 初期化状態のデータかどうか
		bool isInit = true;

		// 時間
		Time time = Time();

		// ステージナンバー
		int stageNumber = 0;

		// セーブしたセル
		Cell cell = Cell();

		// プレイヤーステータス
		PlayerStatus playerStatus = PlayerStatus();
	}; 


	// ショットの種類
	enum class ShotType
	{
		// プレイヤーショット
		PlayerShot,
		// エネミーショット
		EnemyShot,
		// サインカーブショット
		SineCurveShot,
		// 反射ショット
		ReflectionShot
	};

	// 反射データ
	struct Reflect
	{
		// 反射最大数
		int maxCount = 2;

		// 反射回数
		int count = 0;
	};

	// ショットデータ
	struct ShotData
	{
		// ショットの種類
		ShotType type = ShotType::PlayerShot;

		// 座標
		Vec2 startPos = Vec2(0.0f, 0.0f);

		// ショットアングル
		float angle = 0.0f;

		// ショットスピード
		float speed = 0.0f;

		// 反射データ
		Reflect reflect = Reflect();
	};


public:


	/// <summary>
	/// セーブデータの読み込み
	/// </summary>
	void LoadSaveData();

	/// <summary>
	/// セーブデータの書き込み
	/// </summary>
	void WriteSaveData();

	/// <summary>
	/// セーブデータが初期化されているかどうかを返す
	/// </summary>
	/// <returns>セーブデータの初期化状態</returns>
	bool IsSaveDataInit() { return m_saveData.isInit; }

	/// <summary>
	/// セーブデータを設定
	/// </summary>
	/// <param name="savePointData">セーブポイントデータ</param>
	void SetSaveData(const SaveData& savePointData) { m_saveData = savePointData; }

	/// <summary>
	/// セーブポイントデータを返す
	/// </summary>
	/// <returns>セーブポイントデータ</returns>
	SaveData GetSaveData() { return m_saveData; }

	/// <summary>
	/// セーブポイントデータを設定
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	/// <param name="cell">セル</param>
	/// <param name="playerStatus">プレイヤーステータス</param>
	void SetSavePointData(const int& stageNumber = 0, const Cell& cell = Cell(), const PlayerStatus& playerStatus = PlayerStatus());

	/// <summary>
	/// プレイヤー座標設定
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void SetPlayerPos(const Vec2& playerPos) { m_playerPos = playerPos; }

	/// <summary>
	/// プレイヤー座標を返す
	/// </summary>
	/// <returns>プレイヤー座標</returns>
	Vec2 GetPlayerPos() { return m_playerPos; }

	/// <summary>
	/// ボスエネミースポーン座標を設定
	/// </summary>
	/// <param name="bossEnemySpawnPos">ボスエネミースポーン座標</param>
	void SetBossEnemySpawnPos(const Vec2& bossEnemySpawnPos) { m_bossEnemySpawnPos = bossEnemySpawnPos; }

	/// <summary>
	/// ボスエネミースポーン座標を返す
	/// </summary>
	/// <returns>ボスエネミースポーン座標</returns>
	Vec2 GetBossEnemySpawnPos() { return m_bossEnemySpawnPos; }



	/// <summary>
	/// カメラ座標を設定
	/// </summary>
	/// <param name="cameraPos">カメラ座標</param>
	void SetCameraPos(const Vec2& cameraPos) { m_cameraPos = cameraPos; }

	/// <summary>
	/// カメラ座標を返す
	/// </summary>
	/// <returns>カメラ座標</returns>
	Vec2 GetCameraPos() { return m_cameraPos; }


	/// <summary>
	/// プレイヤーステータスを返す
	/// </summary>
	/// <returns>プレイヤーステータスの取得</returns>
	PlayerStatus GetPlayerStatus() { return m_saveData.playerStatus; }

	/// <summary>
	/// プレイヤーステータスを設定
	/// </summary>
	/// <param name="playerStatus">プレイヤーのステータスを設定</param>
	void SetPlayerStatus(const PlayerStatus& playerStatus) { m_saveData.playerStatus = playerStatus; }

	/// <summary>
	/// プレイヤーが生きているかどうかを設定
	/// </summary>
	/// <param name="isPlayerAlive">プレイヤーが生きているかどうか</param>
	void SetIsPlayerAlive(const bool& isPlayerAlive) { m_isPlayerAlive = isPlayerAlive; }

	/// <summary>
	/// プレイヤーが生きているかどうかを返す
	/// </summary>
	/// <returns>プレイヤーが生きているかどうか</returns>
	bool GetIsPlayerAlive() { return m_isPlayerAlive; }

	/// <summary>
	/// 死亡回数を設定
	/// </summary>
	/// <param name="deathCount">死亡回数</param>
	void SetDeathCount(const int& deathCount) { m_saveData.playerStatus.deathCount = deathCount; }

	/// <summary>
	/// 時間計測
	/// </summary>
	void TimeCount();

	// 60カウントしたかどうか
	bool IsTimeCount60(const int& count);

	/// <summary>
	/// 時間を返す
	/// </summary>
	/// <returns>時間</returns>
	Time GetTime() { return m_saveData.time; }

private:

	// セーブポイントデータ
	SaveData m_saveData = SaveData();

	// プレイヤー座標
	Vec2 m_playerPos = Vec2();

	// ボスエネミースポーン座標
	Vec2 m_bossEnemySpawnPos = Vec2();

	// カメラ座標
	Vec2 m_cameraPos = Vec2();


	// プレイヤーが生きているかどうか
	bool m_isPlayerAlive = true;
};