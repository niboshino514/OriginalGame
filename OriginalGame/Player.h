#pragma once
#include "ObjectBase.h"
#include "GameData.h"


template <class TState> class StateMachine;

class Player : public ObjectBase
{
public:

	// プレイヤーの当たり判定半径サイズ
	static constexpr float kCollisionRadius = 120.0f;


	// ジャンプ情報
	struct JumpInfo
	{
		// ジャンプしているかどうか
		bool isJump = false;

		// 落下速度
		float fallSpeed = 0.0f;

		// ジャンプ回数
		int jumpCount = 0;
	};

	// 状態
	enum State
	{
		// 通常状態
		Normal,
		// 死亡
		Dead,
	};

	// コンベア
	struct Conveyor
	{
		// コンベアに乗っているかどうか
		bool isFrag = false;

		// コンベアの速度
		float speed = 0.0f;

		// コンベアの方向
		Direction direction = Direction();
	};
	

public:
	Player();
	virtual ~Player();


	void Init()override;
	void Update()override;
	void Draw()override;

private:

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInit();

	
	// ノーマルステート処理
	void StateNormalEnter();
	void StateNormalUpdate();
	void StateNormalDraw();
	void StateNormalExit();

	



	/// <summary>
	/// リスポーン
	/// </summary>
	void Respawn();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void Jump();

	/// <summary>
	/// 衝突
	/// </summary>
	void Collision();


private:

	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation();

	/// <summary>
	/// 地面の衝突判定
	/// </summary>
	void GroundCollision();

	/// <summary>
	/// 座標線形補間
	/// </summary>
	void PosLinearInterpolation();

	/// <summary>
	/// マップチップの当たり判定
	/// </summary>
	void MapChipCollision(const Vec2& pos);

	/// <summary>
	/// 障害物の当たり判定
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	/// <param name="pos">プレイヤーの四角形情報</param>
	void ObstacleCollision(const ObjectManager::MapCollisionData& mapCollisionData, const Square& square);


	/// <summary>
	/// セーブポイントの衝突処理
	/// </summary>
	void SavePointCollision();

	/// <summary>
	/// マップ移動
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void MapMove(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// 重力処理
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void Gravity(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// 重力変更
	/// </summary>
	/// <param name="gravityDirection">重力変更処理</param>
	void ChangeGravityDirection(const Direction& gravityDirection);

	/// <summary>
	/// アイスブロックの衝突処理
	/// </summary>
	void IceBlockCollision();

	/// <summary>
	/// コンベアの衝突処理
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void ConveyorCollision(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// ジャンプタイプの衝突処理
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void JumpTypeCollision(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// ジャンプ力の衝突処理
	/// </summary>
	/// <param name="mapCollisionData"マップ判定データ></param>
	void JumpPowerCollision(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// 加速フラグの衝突処理
	/// </summary>
	/// <param name="mapCollisionData">マップ判定データ</param>
	void AccelerationCollision(const ObjectManager::MapCollisionData& mapCollisionData);

	/// <summary>
	/// ショット処理
	/// </summary>
	void Shot();

private:

	//////////////
	// 移動関連 //
	//////////////


	// 矩形
	Rect m_rect;

	// 移動矩形
	Rect m_moveRect;

	// サイズ
	Vec2 m_size;

	// 地面にいるかどうか
	bool m_isGround;

	// アイスブロックの上にいるかどうか
	bool m_isIceBlock;
	
	// コンベア
	Conveyor m_conveyor;

	////////////////////
	// プレイヤー状態 //
	////////////////////

	// プレイヤーステータス
	GameData::PlayerStatus m_playerStatus;

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシン
	StateMachine<State>m_pStateMachine;

	////////////////////
	/// ジャンプ関連 ///
	////////////////////
	
	// ジャンプ情報
	JumpInfo m_jumpInfo;

};

