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

	// プレイヤーのステータス
	struct PlayerStatus
	{
		// 重力方向
		Direction gravityDirection = Direction::Bottom;
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

private:

	// セーブポイントデータ
	SavePointData m_savePointData = SavePointData();

	// プレイヤー座標
	Vec2 m_playerPos = Vec2();

	// カメラ座標
	Vec2 m_cameraPos = Vec2();
};