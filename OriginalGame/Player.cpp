#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"
#include <tuple>


namespace
{
	// 移動スピード
	constexpr float kMoveSpeed = 5.0f;

	// サイズ
	const Vec2 kSize(20.0f, 30.0f);

	// 氷の摩擦力
	constexpr float kFrictionIce = 0.98f;

	// コンベアの速度
	constexpr float kConveyorSpeed = kMoveSpeed * 0.8f;

}

namespace
{
	// ジャンプ力
	constexpr float kJumpPower = 15.0f;

	// ジャンプ回数
	constexpr int kJumpCountMax = 5;

	// 重力大
	constexpr float kGravity = 1.5f;
	// 重力小
	constexpr float kSmallGravity = 0.8f;

	// 最大移動量
	constexpr float kMaxDir = 20.0f;


	// 重力反転フラグ
	constexpr bool kGravityReverseFlag = true;
}


Player::Player() :
	m_vec(),
	m_moveRect(),
	m_rect(),
	m_gravityDirection(),
	m_size(),
	m_isGround(),
	m_isIceBlock(),
	m_conveyor(),
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
	// 重力方向変更
	ChangeGravityDirection(Direction::Bottom);

	// アイスブロックフラグをfalseにする
	m_isIceBlock = false;



	// コンベア情報初期化
	{
		// コンベアに乗っているかどうか
		m_conveyor.isFrag = false;

		// コンベアの速度
		m_conveyor.speed = kConveyorSpeed;
	}

}

void Player::StateNormalUpdate()
{
	// 移動処理
	Move();

	// ジャンプ処理
	Jump();


	// 当たり判定
	Collision();
}

void Player::StateNormalDraw()
{
	// オフセット値を取得
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();


	// デバッグ描画
#if(true)
	// 描画座標を計算
	const Rect drawRect = EvoLib::Convert::PosToRect(m_pos + offset, m_size);
	
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
#endif
}

void Player::StateNormalExit()
{
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


	// 移動量
	Vec2 inputVec = Vec2();


	if (m_gravityDirection == Direction::Top ||
		m_gravityDirection == Direction::Bottom)
	{
		// パッドを使用した移動
		if (Pad::IsPress(PAD_INPUT_RIGHT))
		{
			inputVec.x += kMoveSpeed;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))
		{
			inputVec.x -= kMoveSpeed;
		}
	}
	else
	{
		// パッドを使用した移動
		if (Pad::IsPress(PAD_INPUT_DOWN))
		{
			inputVec.y += kMoveSpeed;
		}
		if (Pad::IsPress(PAD_INPUT_UP))
		{
			inputVec.y -= kMoveSpeed;
		}
	}


	// コンベアに乗っている場合、移動量を変更する
	if (m_isGround &&
		m_conveyor.isFrag)
	{
		if (m_gravityDirection == Direction::Top ||
			m_gravityDirection == Direction::Bottom)
		{
			if (m_conveyor.direction == Direction::Right)
			{
				inputVec.x += m_conveyor.speed;
			}
			else if (m_conveyor.direction == Direction::Left)
			{
				inputVec.x += -m_conveyor.speed;
			}
		}
		else
		{
			if (m_conveyor.direction == Direction::Top)
			{
				inputVec.y += -m_conveyor.speed;
			}
			else if (m_conveyor.direction == Direction::Bottom)
			{
				inputVec.y += m_conveyor.speed;
			}
		}

		// コンベアに乗っている場合、移動量を変更する
		m_vec = inputVec;
	}

	// アイスブロックの場合、移動量を変更する
	if(m_isIceBlock)
	{
		m_vec += inputVec;

		m_vec.x = EvoLib::Calculation::Clamp(m_vec.x, -kMoveSpeed, kMoveSpeed);
		m_vec.y = EvoLib::Calculation::Clamp(m_vec.y, -kMoveSpeed, kMoveSpeed);

		m_vec *= kFrictionIce;

		return;
	}

	// 移動量を代入
	m_vec = inputVec;
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

	
	if (m_gravityDirection == Direction::Top ||
		m_gravityDirection == Direction::Bottom)
	{
		// 移動量に落下スピードを代入する
		m_vec.y = m_jumpInfo.fallSpeed;


		if (m_gravityDirection == Direction::Top)
		{
			// 移動量に落下スピードを代入する
			m_vec.y = -m_jumpInfo.fallSpeed;
		}
		else
		{
			// 移動量に落下スピードを代入する
			m_vec.y = m_jumpInfo.fallSpeed;
		}

		// 一定以上の速度になったら速度を抑える
		if (m_vec.y > kMaxDir)
		{
			m_vec.y = kMaxDir;
		}
	}
	else
	{
		// 移動量に落下スピードを代入する
		m_vec.x = m_jumpInfo.fallSpeed;


		if (m_gravityDirection == Direction::Left)
		{
			// 移動量に落下スピードを代入する
			m_vec.x = -m_jumpInfo.fallSpeed;
		}
		else
		{
			// 移動量に落下スピードを代入する
			m_vec.x = m_jumpInfo.fallSpeed;
		}

		// 一定以上の速度になったら速度を抑える
		if (m_vec.x > kMaxDir)
		{
			m_vec.x = kMaxDir;
		}
	}

}

void Player::Collision()
{

	// 座標を四角形情報に変換
	m_square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(m_pos, m_size));


	// 地面の当たり判定
	GroundCollision();

	// 線形補間
	PosLinearInterpolation();
}

void Player::GroundCollision()
{
	// 中心座標から矩形を求める
	m_rect = EvoLib::Convert::PosToRect(m_pos, m_size);

	// マップチップのサイズを取得
	const float mapChipSize = m_pObjectFactory->GetMapInfoData().mapChip.chipSize;

	// マップチップの最大セルを取得
	const Cell maxCell = Cell(m_pObjectFactory->GetMapInfoData().mapChip.mapWidth, m_pObjectFactory->GetMapInfoData().mapChip.mapHeight);

	// 地面セル番号
	std::vector<int>groundCellNumber;
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::Ground));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::IceBlock));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::TopConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::BottomConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::LeftConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::RigthConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::TransparentBlock));


	// 移動可能範囲の矩形を取得
	m_moveRect = EvoLib::Calculation::CalculateRectangleMovementRange
	(m_rect, maxCell, mapChipSize, m_pObjectFactory->GetMapChipNumber(), groundCellNumber);



	// 取得した矩形をm_posの移動可能範囲に変換
	m_moveRect.left += m_size.x * 0.5f;
	m_moveRect.right -= m_size.x * 0.5f;
	m_moveRect.top += m_size.y * 0.5f;
	m_moveRect.bottom -= m_size.y * 0.5f;


	// 移動量を座標に代入
	m_pos += m_vec;



	// 地面判定初期化
	m_isGround = false;


	if (m_gravityDirection == Direction::Top ||
		m_gravityDirection == Direction::Bottom)
	{
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

		if (m_gravityDirection == Direction::Bottom)
		{

			if (m_pos.y < m_moveRect.top)
			{
				m_pos.y = m_moveRect.top;

				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				m_vec.y = 0.0f;
			}
			if (m_pos.y > m_moveRect.bottom)
			{
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				// 地面フラグをtrueにする
				m_isGround = true;

				m_pos.y = m_moveRect.bottom;
				m_vec.y = 0.0f;
			}
		}
		else
		{
			if (m_pos.y < m_moveRect.top)
			{
				// 地面フラグをtrueにする
				m_isGround = true;

				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.y = m_moveRect.top;
				m_vec.y = 0.0f;
			}
			if (m_pos.y > m_moveRect.bottom)
			{
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.y = m_moveRect.bottom;
				m_vec.y = 0.0f;
			}
		}
	}
	else
	{
	
		if (m_pos.y < m_moveRect.top)
		{
			m_pos.y = m_moveRect.top;
			m_vec.y = 0.0f;
		}
		if (m_pos.y > m_moveRect.bottom)
		{
			m_pos.y = m_moveRect.bottom;
			m_vec.y = 0.0f;
		}

		if (m_gravityDirection == Direction::Left)
		{

			if (m_pos.x < m_moveRect.left)
			{
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				// 地面フラグをtrueにする
				m_isGround = true;

				m_pos.x = m_moveRect.left;
				m_vec.x = 0.0f;
			}
			if (m_pos.x > m_moveRect.right)
			{
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.x = m_moveRect.right;
				m_vec.x = 0.0f;
			}
		}
		else
		{
			if (m_pos.x < m_moveRect.left)
			{
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.x = m_moveRect.left;
				m_vec.x = 0.0f;
			}
			if (m_pos.x > m_moveRect.right)
			{
		
				// 落下速度を0.0fにする
				m_jumpInfo.fallSpeed = 0.0f;

				// 地面フラグをtrueにする
				m_isGround = true;

				m_pos.x = m_moveRect.right;
				m_vec.x = 0.0f;
			}
		}
	}


	if(m_isGround)
	{
		// 地面に着いているので、ジャンプフラグをfalseにする
		m_jumpInfo.isJump = false;

		// ジャンプカウントの最大値を代入する
		m_jumpInfo.jumpCount = kJumpCountMax;

		// アイスブロックフラグをfalseにする
		m_isIceBlock = false;
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
		EvoLib::Calculation::InearInterpolationCount(beforePos, m_vec, m_size);

	

	// 線形補間座標を計算
	std::vector<Vec2> iinearInterpolationPos =
		EvoLib::Calculation::InearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

	

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
	// コンベアフラグ初期化
	m_conveyor.isFrag = false;

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
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::NextStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::PreviouseStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::Save;


			// 障害物の当たり判定を行うかどうか
			const bool isObstacleCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::DiedBlock;


			// 重力方向を変更するかどうか
			const bool isGravity =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightGravity;
				
			// アイスブロックの当たり判定を行うかどうか
			const bool isIceBlockCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::IceBlock;

			// コンベアの当たり判定を行うかどうか
			const bool isConveyorCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RigthConveyor;


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

			// 重力方向を変更するかどうか
			if(isGravity)
			{
				// 重力方向を変更する
				Gravity(mapCollisionData[x][y], pos);
			}
			
			// アイスブロックの当たり判定を行うかどうか
			if (isIceBlockCollision)
			{
				// アイスブロックの当たり判定
				IceBlockCollision(mapCollisionData[x][y], pos);
			}

			// コンベアの当たり判定を行うかどうか
			if(isConveyorCollision)
			{
				// コンベアの当たり判定
				ConveyorCollision(mapCollisionData[x][y], pos);
			}


			// 存在しない場合、ループを抜ける
			if (!m_isExlist)
			{
				return;
			}
		}
	}
}

void Player::ObstacleCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// 四角形と四角形の当たり判定
	if (mapCollisionData.chipType == ObjectFactory::ChipType::DiedBlock)
	{
		// 四角形同士の当たり判定
		if (EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
		{
			// ステートをデッドにする
			m_pStateMachine.SetState(State::Dead);
		}

		return;
	}


	// 三角形の情報を取得
	const Triangle needle = m_pObjectFactory->ChipTypeToTriangle(mapCollisionData.chipType, mapCollisionData.square);

	// 三角形と四角形の当たり判定
	if (EvoLib::Collision::IsTriangleToSquare(needle, square))
	{
		// ステートをデッドにする
		m_pStateMachine.SetState(State::Dead);
	}
}

void Player::MapMove(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));


	// 四角形同士の当たり判定
	if (!EvoLib::Collision::IsSquareToSquare(mapCollisionData.square, square))
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

void Player::Gravity(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	
	// 四角形同士の当たり判定
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}


	Direction gravityDirection = Direction();

	// 重力方向を変更する
	if (mapCollisionData.chipType == ObjectFactory::ChipType::TopGravity)
	{
		gravityDirection = Direction::Top;
	}
	else if (mapCollisionData.chipType == ObjectFactory::ChipType::BottomGravity)
	{
		gravityDirection = Direction::Bottom;
	}
	else if(mapCollisionData.chipType == ObjectFactory::ChipType::LeftGravity)
	{
		gravityDirection = Direction::Left;
	}
	else if (mapCollisionData.chipType == ObjectFactory::ChipType::RightGravity)
	{
		gravityDirection = Direction::Right;
	}

	ChangeGravityDirection(gravityDirection);

}

void Player::ChangeGravityDirection(const Direction& gravityDirection)
{
	if (gravityDirection == Direction::Top ||
		gravityDirection == Direction::Bottom)
	{
		m_size = kSize;
	}
	else
	{
		m_size = Vec2(kSize.y, kSize.x);
	}

	// 重力方向を変更する
	m_gravityDirection = gravityDirection;
}

void Player::IceBlockCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// 四角形同士の当たり判定
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}

	m_isIceBlock = true;
}

void Player::ConveyorCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// 四角形同士の当たり判定
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}

	// コンベアの方向を取得
	if(mapCollisionData.chipType == ObjectFactory::ChipType::TopConveyor)
	{
		m_conveyor.direction = Direction::Top;
	}
	else if(mapCollisionData.chipType == ObjectFactory::ChipType::BottomConveyor)
	{
		m_conveyor.direction = Direction::Bottom;
	}
	else if(mapCollisionData.chipType == ObjectFactory::ChipType::LeftConveyor)
	{
		m_conveyor.direction = Direction::Left;
	}
	else if(mapCollisionData.chipType == ObjectFactory::ChipType::RigthConveyor)
	{
		m_conveyor.direction = Direction::Right;
	}

	// コンベアに乗っている
	m_conveyor.isFrag = true;
}
