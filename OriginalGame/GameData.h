#pragma once
#include "EvoLib.h"

#include "Vec2.h"

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
	};


	// セーブポイントデータ
	struct SavePointData
	{
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
	};


	// ショットデータ
	struct ShotData
	{
		// ショットの種類
		ShotType type = ShotType::PlayerShot;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);

		// ショットアングル
		float angle = 0.0f;

		// ショットスピード
		float speed = 0.0f;
	};


public:


	/// <summary>
	/// セーブポイントデータを設定
	/// </summary>
	/// <param name="savePointData">セーブポイントデータ</param>
	void SetSavePointData(const SavePointData& savePointData) { m_savePointData = savePointData; }

	/// <summary>
	/// セーブポイントデータを返す
	/// </summary>
	/// <returns>セーブポイントデータ</returns>
	SavePointData GetSavePointData() { return m_savePointData; }

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
	/// ボスエネミー座標を設定
	/// </summary>
	/// <param name="enemyBossPos">ボスエネミー座標</param>
	void SetBossEnemyPos(const Vec2& bossEnemyPos) { m_bossEnemyPos = bossEnemyPos; }

	/// <summary>
	/// ボスエネミー座標を返す
	/// </summary>
	/// <returns>ボスエネミー座標</returns>
	Vec2 GetBossEnemyPos() { return m_bossEnemyPos; }

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
	PlayerStatus GetPlayerStatus() { return m_savePointData.playerStatus; }

	/// <summary>
	/// プレイヤーステータスを設定
	/// </summary>
	/// <param name="playerStatus">プレイヤーのステータスを設定</param>
	void SetPlayerStatus(const PlayerStatus& playerStatus) { m_savePointData.playerStatus = playerStatus; }

private:

	// セーブポイントデータ
	SavePointData m_savePointData = SavePointData();

	// プレイヤー座標
	Vec2 m_playerPos = Vec2();

	// ボスエネミー座標
	Vec2 m_bossEnemyPos = Vec2();

	// ボスエネミースポーン座標
	Vec2 m_bossEnemySpawnPos = Vec2();


	// カメラ座標
	Vec2 m_cameraPos = Vec2();

};