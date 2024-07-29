#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"
#include <tuple>
#include "GameData.h"

namespace
{
	// 移動スピード
	constexpr float kMoveSpeed = 2.0f;

	// サイズ
	const Vec2 kSize(20.0f, 30.0f);
}

namespace
{
	// ジャンプ力
	constexpr float kJumpPower = 15.0f;

	// ジャンプ回数
	constexpr int kJumpCountMax = 2;

	// 重力大
	constexpr float kGravity = 1.5f;
	// 重力小
	constexpr float kSmallGravity = 0.8f;

	// 最大移動量
	constexpr float kMaxDirY = 20.0f;
}


Player::Player() :
	m_pos(),
	m_vec(),
	m_moveRect(),
	m_rect(),
	m_spawnPoint(),
	m_pStateMachine(),
	m_jumpInfo()
{
}

Player::~Player()
{
}

void Player::Init()
{
	// オブジェクトID設定
	m_objectID = ObjectID::Player;

	m_pos = m_circle.centerPos;

	// スポーン地点初期化
	SpawnPointInit();
	
	// ステートマシンの初期化
	StateInit();
}

void Player::Update()
{
	// ステートマシンの更新
	m_pStateMachine.Update();

	// リスポーン
	Respawn();
}

void Player::Draw()
{
	DrawFormatString(0, 15 * 1, 0xffffff, "座標X:%f,座標Y:%f,", m_pos.x, m_pos.y);

	// ステートマシンの描画
	m_pStateMachine.Draw();
}


void Player::StateInit()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 通常ステート
	{
		auto enter = [this]() { StateNormalEnter(); };
		auto update = [this]() { StateNormalUpdate(); };
		auto draw = [this]() { StateNormalDraw(); };
		auto exit = [this]() { StateNormalExit(); };

		m_pStateMachine.AddState(State::Normal, enter, update, draw, exit);
	}
	// 死亡ステート
	{
		m_pStateMachine.AddState(State::Dead, dummy, dummy, dummy, dummy);
	}

	// 初期ステートを設定
	m_pStateMachine.SetState(State::Normal);
}

void Player::StateNormalEnter()
{
}

void Player::StateNormalUpdate()
{
	// 移動処理
	Move();

	// ジャンプ処理
	Jump();

	// スポーン地点から離れたかどうか
	SpawnPointLeave();

	// 当たり判定
	Collision();
}

void Player::StateNormalDraw()
{
	// オフセット値を取得
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// 描画座標を計算
	const Rect drawRect = FunctionConclusion::RectangleCalculation(m_pos + offset, kSize);
	
	// 移動可能範囲の矩形を取得
	Rect drawMoveRect = Rect();

	drawMoveRect.left = m_moveRect.left + offset.x;
	drawMoveRect.right = m_moveRect.right + offset.x;
	drawMoveRect.bottom = m_moveRect.bottom + offset.y;
	drawMoveRect.top = m_moveRect.top + offset.y;


	// プレイヤー描画
	DrawBoxAA(drawRect.left, drawRect.top, drawRect.right, drawRect.bottom,
		0xff0000, true);

	// 移動できる場所を描画
	DrawBoxAA(drawMoveRect.left, drawMoveRect.top, drawMoveRect.right, drawMoveRect.bottom,
		0x0000ff, false);
}

void Player::StateNormalExit()
{
}

void Player::SpawnPointInit()
{
	// マップチップのサイズを取得
	const float mapChipSize = m_pObjectFactory->GetMapInfoData().mapChip.chipSize;

	// スポーン地点のセル
	const Cell spawnCell = FunctionConclusion::CoordinateWithCellToConversion(m_pos, mapChipSize);

	// セルの中心座標を取得
	Vec2 cellCenterPos = FunctionConclusion::CellWithCoordinateToConversion(spawnCell, mapChipSize);
	cellCenterPos = Vec2(cellCenterPos.x , cellCenterPos.y);

	// スポーン地点の四角形情報を取得する
	m_spawnPoint.square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(cellCenterPos, Vec2(mapChipSize, mapChipSize)));

	// スポーン地点から離れたかどうかのフラグをfalseにする
	m_spawnPoint.isLeave = false;
}

void Player::SpawnPointLeave()
{
	// スポーン地点から離れていた場合、ここで処理を終了する
	if(m_spawnPoint.isLeave)
	{
		return;
	}

	// 現在座標を四角形情報に変換
	const Square currentSquare = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(m_pos, kSize));

	// 現在の座標の四角形情報とスポーン地点の四角形情報が交差していない場合、スポーン地点から離れたと判断する
	if(!FunctionConclusion::CollisionDetectionOfQuadrangleAndQuadrangle(currentSquare, m_spawnPoint.square))
	{
		m_spawnPoint.isLeave = true;
	}
}

void Player::Respawn()
{
	// セーブポイントセルを取得する
	if (Pad::IsPress(PAD_INPUT_1))
	{
		// ステージを変更するかどうか
		bool isChangeStage = false;

		// セーブポイント座標を取得する
		std::tie(isChangeStage, m_pos) = m_pObjectFactory->GetSavePointPos();

		// ステージを変更しない場合、ステートをノーマルにする
		if (!isChangeStage)
		{
			m_pStateMachine.SetState(State::Normal);
		}
	}
}

void Player::Move()
{
	// 移動量初期化
	m_vec = Vec2();

	// パッドを使用した移動
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_vec.x += kMoveSpeed;
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_vec.x -= kMoveSpeed;
	}
}

void Player::Jump()
{
	// ボタンを押したとき、ジャンプカウントが0以上ならばジャンプ力を与える
	if (Pad::IsTrigger(PAD_INPUT_10)&&
		m_jumpInfo.jumpCount > 0)
	{
		// ジャンプフラグをtrueにする
		m_jumpInfo.isJump = true;

		// ジャンプ力を与える
		m_jumpInfo.fallSpeed = -kJumpPower;

		// ジャンプカウントを減らす
		m_jumpInfo.jumpCount--;
	}

	// 飛んでいた場合はボタンを押しているかどうかで落下スピードが変わる
	if (m_jumpInfo.isJump)
	{
		// ボタンを押しているかどうかで、ジャンプ力が変わる
		if (Pad::IsPress(PAD_INPUT_10))
		{
			// 小さい重力を与える
			m_jumpInfo.fallSpeed += kSmallGravity;
		}
		else
		{
			// 大きい重力を与える
			m_jumpInfo.fallSpeed += kGravity;
		}
	}
	else
	{
		// ジャンプしていなかったら通常の重力を与える
		m_jumpInfo.fallSpeed += kGravity;
	}

	// 移動量に落下スピードを代入する
	m_vec.y = m_jumpInfo.fallSpeed;


	// 一定以上の速度になったら速度を抑える
	if (m_vec.y > kMaxDirY)
	{
		m_vec.y = kMaxDirY;
	}
}

void Player::Collision()
{
	// 地面の当たり判定
	GroundCollision();

	// 線形補間
	PosLinearInterpolation();
}

void Player::GroundCollision()
{
	// 中心座標から矩形を求める
	m_rect = FunctionConclusion::RectangleCalculation(m_pos, kSize);

	// 移動可能範囲の矩形を取得
	m_moveRect = FunctionConclusion::GetMoveEnableRect
	(m_rect, m_pObjectFactory->GetMapInfoData().mapChip, m_pObjectFactory->GetMapChipNumber());

	// 取得した矩形をm_posの移動可能範囲に変換
	m_moveRect.left += kSize.x * 0.5f;
	m_moveRect.right -= kSize.x * 0.5f;
	m_moveRect.top += kSize.y * 0.5f;
	m_moveRect.bottom -= kSize.y * 0.5f;


	// 移動量を座標に代入
	m_pos += m_vec;


	if (m_pos.x < m_moveRect.left)
	{
		m_pos.x = m_moveRect.left;
		m_vec.x = 0.0f;
	}
	if (m_pos.x > m_moveRect.right)
	{
		m_pos.x = m_moveRect.right;
		m_vec.x = 0.0f;
	}
	if (m_pos.y < m_moveRect.top)
	{
		m_pos.y = m_moveRect.top;

		// 落下速度を0.0fにする
		m_jumpInfo.fallSpeed = 0.0f;

		m_vec.y = 0.0f;
	}
	if (m_pos.y > m_moveRect.bottom)
	{
		// 地面に着いているので、ジャンプフラグをfalseにする
		m_jumpInfo.isJump = false;

		// ジャンプカウントの最大値を代入する
		m_jumpInfo.jumpCount = kJumpCountMax;

		// 落下速度を0.0fにする
		m_jumpInfo.fallSpeed = 0.0f;

		m_pos.y = m_moveRect.bottom;
		m_vec.y = 0.0f;
	}

	// 座標をゲームデータに代入
	GameData::GetInstance()->SetPlayerPos(m_pos);
}

void Player::PosLinearInterpolation()
{
	// 前の座標
	const Vec2 beforePos = m_pos - m_vec;

	// 線形補間数を計算
	const int iinearInterpolationCount =
		FunctionConclusion::IinearInterpolationCountCalculation(beforePos, m_vec, kSize);

	// 線形補間座標を計算
	std::vector<Vec2> iinearInterpolationPos =
		FunctionConclusion::IinearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

	// 線形補間数が0ならば、座標に移動量を足したものを配列に入れる
	if (iinearInterpolationCount == 0)
	{
		// 移動後の座標を代入する
		iinearInterpolationPos.push_back(m_pos);
	}

	// 線形補正分for分を回す
	for (int i = 0; i < static_cast<int>(iinearInterpolationPos.size()); i++)
	{
		// マップチップの当たり判定
		MapChipCollision(iinearInterpolationPos[i]);
	}
}

void Player::MapChipCollision(const Vec2& pos)
{

	// マップ判定データを取得
	std::vector<std::vector<ObjectFactory::MapCollisionData>> mapCollisionData =
		m_pObjectFactory->GetMapInfoData().mapCollisionData;

	// マップの幅と高さを取得
	const int mapWidth = static_cast<int>(mapCollisionData.size());
	const int mapHeight = static_cast<int>(mapCollisionData[0].size());



	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			//マップ判定データが画面外の場合、次のループに移る
			if (!mapCollisionData[x][y].screenFlag)
			{
				continue;
			}

			// マップ移動フラグを行うかどうか
			const bool isMapMove =
				m_spawnPoint.isLeave &&
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::NextStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::PreviouseStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::Save;


			// 障害物の当たり判定を行うかどうか
			const bool isObstacleCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightNeedle;


			// マップ移動の当たり判定を行うかどうか
			if (isMapMove)
			{
				// マップチップの当たり判定
				MapMove(mapCollisionData[x][y], pos);
			}
			
			// 障害物の当たり判定を行うかどうか
			if (isObstacleCollision)
			{
				// 障害物の当たり判定
				ObstacleCollision(mapCollisionData[x][y], pos);
			}
		}
	}
}

void Player::ObstacleCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(pos, kSize));

	// 三角形の情報を取得
	const Triangle needle = m_pObjectFactory->ChipTypeToTriangle(mapCollisionData.chipType, mapCollisionData.square);

	if (FunctionConclusion::CollisionDetectionOfQuadrangleAndTriangle(square, needle))
	{
		// ステートをデッドにする
		m_pStateMachine.SetState(State::Dead);
	}
}


void Player::MapMove(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{

	// 座標を四角形情報に変換
	const Square square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(pos, kSize));

	// 四角形同士の当たり判定
	if (!FunctionConclusion::CollisionDetectionOfQuadrangleAndQuadrangle(mapCollisionData.square, square))
	{
		return;
	}

	// セーブポイントに当たった場合、セーブする
	if (mapCollisionData.chipType == ObjectFactory::ChipType::Save)
	{
		m_pObjectFactory->SetSavePoint(mapCollisionData.circle.centerPos);
	}

	// 次のステージに進む
	if (mapCollisionData.chipType == ObjectFactory::ChipType::NextStage)
	{
		m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::NextStage);

		return;
	}

	// 前のステージに戻る
	if (mapCollisionData.chipType == ObjectFactory::ChipType::PreviouseStage)
	{
		m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::PreviouseStage);

		return;
	}
}