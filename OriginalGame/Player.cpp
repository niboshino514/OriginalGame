#include "Player.h"
#include "Pad.h"
#include "ObjectManager.h"
#include <tuple>
#include "Controller.h"
#include "GameData.h"
#include <string>
#include "Sound.h"


namespace
{
	// 通常移動スピード
	constexpr float kMoveNormalSpeed = 5.0f;
	// 速い移動スピード
	constexpr float kMoveFastSpeed = kMoveNormalSpeed * 1.8f;
	// 遅い移動スピード
	constexpr float kMoveSlowSpeed = kMoveNormalSpeed * 0.7f;

	// サイズ
	const Vec2 kSize(20.0f, 30.0f);

	// 氷の摩擦力
	constexpr float kFrictionIce = 0.98f;

	// コンベアの速度
	constexpr float kConveyorSpeed = kMoveNormalSpeed * 0.8f;
}

namespace
{
	// 通常ジャンプ力
	constexpr float kJumpNormalPower = 15.0f;
	// 強ジャンプ力
	constexpr float kJumpStrongPower = kJumpNormalPower * 1.6f;
	// 弱ジャンプ力
	constexpr float kJumpWeakPower = kJumpNormalPower * 0.7;



	// ジャンプ回数
	constexpr int kJumpCountMax = 2;

	// 重力大
	constexpr float kGravity = 1.5f;
	// 重力小
	constexpr float kSmallGravity = 0.8f;

	// 最大移動量
	constexpr float kMaxDir = 20.0f;
}


namespace Graph
{
	// 拡大率
	const double kScale = 1.5;
	// 縦幅調整値
	const float kAdjustmentValue = (5.0f);
	
	// キャラクター描画の調整値を表す構造体
	struct CharacterDrawAdjustment
	{
		// 座標調整値
		Vec2 adjustmentPos = Vec2();

		// 角度
		float angle = 0.0f;
	};

	// キャラクター描画の調整値の配列
	const CharacterDrawAdjustment kCharacterDrawAdjustment[4] =
	{
		// 上
		{Vec2(0.0f, kAdjustmentValue), 180.0f},
		// 下
		{Vec2(0.0f, -kAdjustmentValue), 0.0f},
		// 左
		{Vec2(kAdjustmentValue, 0.0f), 90.0f},
		// 右
		{Vec2(-kAdjustmentValue, 0.0f), 270.0f},
	};
}


namespace Anime
{
	const ObjectBase::AnimationRange kAnimationRange[4] =
	{
		{ 0, 2, 1,1 },// 上
		{ 6, 8, 7 ,7},	// 下
		{ 9, 11, 10 ,10},	// 左
		{ 3, 5, 4 ,4},	// 右
	};

	// アニメーション速度
	constexpr int kAnimeSpeed = 5;
}

namespace Shot
{
	// ショットのスピード
	constexpr float kSpeed = 10.0f;




}




Player::Player() :
	m_moveRect(),
	m_rect(),
	m_size(),
	m_isGround(),
	m_isIceBlock(),
	m_conveyor(),
	m_playerStatus(),
	m_pStateMachine(),
	m_jumpInfo()
{
}

Player::~Player()
{
}

void Player::Init()
{
	// 座標をゲームデータに代入
	GameData::GetInstance()->SetPlayerPos(m_pos);

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

	// プレイヤーが生きているかどうかを設定
	GameData::GetInstance()->SetIsPlayerAlive(true);

	// プレイヤーステータスを取得
	m_playerStatus = GameData::GetInstance()->GetPlayerStatus();

	// 重力方向変更
	ChangeGravityDirection(m_playerStatus.gravityDirection);

	// アイスブロックフラグをfalseにする
	m_isIceBlock = false;


	// コンベア情報初期化
	{
		// コンベアに乗っているかどうか
		m_conveyor.isFrag = false;

		// コンベアの速度
		m_conveyor.speed = kConveyorSpeed;
	}

	// アニメーション
	{
		// アニメーションデータ初期化
		m_animationDetails.frameSpeed = Anime::kAnimeSpeed;

		// 向き初期化
		m_animationDetails.direction[0] = Direction::Left;

		// アニメーション番号初期化
		m_animationDetails.number = Anime::kAnimationRange[static_cast<int>(m_animationDetails.direction[0])].stopNo;

		// アニメーションタイプ初期化
		m_animationDetails.type = AnimationType::Idle;
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

	// アニメーション処理
	Animation();

	// ショット処理
	Shot();



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


	Vec2 pos = m_pos + offset;
	pos += Graph::kCharacterDrawAdjustment[static_cast<int>(m_playerStatus.gravityDirection)].adjustmentPos;
	
	// 回転角度
	const float rota = EvoLib::Convert::ConvertAngleToRadian(Graph::kCharacterDrawAdjustment[static_cast<int>(m_playerStatus.gravityDirection)].angle);

	// 反転フラグ
	const bool isReverse = 
		m_playerStatus.gravityDirection == Direction::Right ||
		m_playerStatus.gravityDirection == Direction::Top;

	// プレイヤー描画
	DrawRotaGraphF(
		pos.x,
		pos.y,
		Graph::kScale, rota,
		m_graphicHandle[m_animationDetails.number], true, isReverse);
}

void Player::StateNormalExit()
{
	// 死亡音を再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Dead)]);

	// プレイヤーが生きているかどうかを設定
	GameData::GetInstance()->SetIsPlayerAlive(false);
}


void Player::Respawn()
{
	// セーブポイントセルを取得する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RESPAWN))
	{
		// ステージを変更するかどうか
		bool isChangeStage = false;

		// セーブポイント座標を取得する
		std::tie(isChangeStage, m_pos) = m_pObjectManager->GetSavePointPos();

		// 座標を四角形情報に変換
		m_square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(m_pos, m_size));


		// ステージを変更しない場合、ステートをノーマルにする
		if (!isChangeStage)
		{
			m_pStateMachine.SetState(State::Normal);
		}

		// リスタート音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Restart)]);
	}
}

void Player::Move()
{

	// 前のフレームのプレイヤーの向きを保存
	for (int i = 1; i > 0; i--)
	{
		m_animationDetails.direction[i] = m_animationDetails.direction[i - 1];
	}


	// 移動量
	Vec2 inputVec = Vec2();

	// 移動スピード
	float moveSpeed = 0.0f;

	if(m_playerStatus.moveSpeed == GameData::MoveSpeed::Normal)
	{
		moveSpeed = kMoveNormalSpeed;
	}
	else if (m_playerStatus.moveSpeed == GameData::MoveSpeed::Fast)
	{
		moveSpeed = kMoveFastSpeed;
	}
	else if (m_playerStatus.moveSpeed == GameData::MoveSpeed::Slow)
	{
		moveSpeed = kMoveSlowSpeed;
	}


	if (m_playerStatus.gravityDirection == Direction::Top ||
		m_playerStatus.gravityDirection == Direction::Bottom)
	{
		// パッドを使用した移動
		if (Controller::GetInstance()->IsPress(Controller::ControllerButton::RIGHT))
		{
			inputVec.x += moveSpeed;

			// 向いている方向を右にする
			m_animationDetails.direction[0] = Direction::Right;
		}
		if (Controller::GetInstance()->IsPress(Controller::ControllerButton::LEFT))
		{
			inputVec.x -= moveSpeed;

			// 向いている方向を左にする
			m_animationDetails.direction[0] = Direction::Left;
		}

		// 移動量が0.0fの場合、アニメーションをアイドルにする
		if (inputVec.x == 0.0f)
		{
			m_animationDetails.type = AnimationType::Idle;
			m_animationDetails.number = 
				Anime::kAnimationRange[static_cast<int>(m_animationDetails.direction[0])].stopNo;
		}
		else
		{
			m_animationDetails.type = AnimationType::Move;
		}

	}
	else
	{
		// パッドを使用した移動
		if (Controller::GetInstance()->IsPress(Controller::ControllerButton::DOWN))
		{
			inputVec.y += moveSpeed;

			// 向いている方向を右にする
			m_animationDetails.direction[0] = Direction::Right;
		}
		if (Controller::GetInstance()->IsPress(Controller::ControllerButton::UP))
		{
			inputVec.y -= moveSpeed;

			// 向いている方向を左にする
			m_animationDetails.direction[0] = Direction::Left;
		}

		// 移動量が0.0fの場合、アニメーションをアイドルにする
		if (inputVec.y == 0.0f)
		{
			m_animationDetails.type = AnimationType::Idle;
			m_animationDetails.number = 
				Anime::kAnimationRange[static_cast<int>(m_animationDetails.direction[0])].stopNo;
		}
		else
		{
			m_animationDetails.type = AnimationType::Move;
		}

	}


	// コンベアに乗っている場合、移動量を変更する
	if (m_isGround &&
		m_conveyor.isFrag)
	{
		if (m_playerStatus.gravityDirection == Direction::Top ||
			m_playerStatus.gravityDirection == Direction::Bottom)
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

		m_vec.x = EvoLib::Calculation::Clamp(m_vec.x, -kMoveNormalSpeed, kMoveNormalSpeed);
		m_vec.y = EvoLib::Calculation::Clamp(m_vec.y, -kMoveNormalSpeed, kMoveNormalSpeed);

		m_vec *= kFrictionIce;

		return;
	}

	// 移動量を代入
	m_vec = inputVec;
}

void Player::Jump()
{
	// ボタンを押したとき、ジャンプカウントが0以上ならばジャンプ力を与える
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::JUMP))
	{

		// ジャンプカウントが0以上ならばジャンプ力を与える
		if (m_jumpInfo.jumpCount > 0 ||
			m_playerStatus.jumpType == GameData::JumpType::Infinite)
		{
			// ジャンプフラグをtrueにする
			m_jumpInfo.isJump = true;

			// ジャンプ力
			float jumpPower = 0.0f;

			if(m_playerStatus.jumpPower == GameData::JumpPower::Normal)
			{
				jumpPower = kJumpNormalPower;
			}
			else if (m_playerStatus.jumpPower == GameData::JumpPower::Strong)
			{
				jumpPower = kJumpStrongPower;
			}
			else if (m_playerStatus.jumpPower == GameData::JumpPower::Weak)
			{
				jumpPower = kJumpWeakPower;
			}

			// ジャンプ力を与える
			m_jumpInfo.fallSpeed = -jumpPower;


			// ジャンプタイプがSecondならば、ジャンプカウントを減らす
			if (m_playerStatus.jumpType == GameData::JumpType::Second)
			{
				// ジャンプカウントを減らす
				m_jumpInfo.jumpCount--;
			}
			
			// ジャンプ音を再生
			Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Jump)]);
		}
	}

	// 飛んでいた場合はボタンを押しているかどうかで落下スピードが変わる
	if (m_jumpInfo.isJump)
	{
		// ボタンを押しているかどうかで、ジャンプ力が変わる
		if (Controller::GetInstance()->IsPress(Controller::ControllerButton::JUMP))
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

	
	if (m_playerStatus.gravityDirection == Direction::Top ||
		m_playerStatus.gravityDirection == Direction::Bottom)
	{
		// 移動量に落下スピードを代入する
		m_vec.y = m_jumpInfo.fallSpeed;


		if (m_playerStatus.gravityDirection == Direction::Top)
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


		if (m_playerStatus.gravityDirection == Direction::Left)
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

void Player::Animation()
{

	DrawFormatString(0, 15 * 4, 0xffffff, "アニメーション番号:%d", m_animationDetails.number);


	if (m_animationDetails.direction[0] != m_animationDetails.direction[1])
	{
		m_animationDetails.number = Anime::kAnimationRange[static_cast<int>(m_animationDetails.direction[0])].dirNo;
	}

	// 移動以外の場合、アニメーションを行わない
	if (m_animationDetails.type != AnimationType::Move)
	{
		return;
	}

	
	// アニメーション番号を更新
	m_animationDetails.number = AnimationNamberUpdate(m_animationDetails, Anime::kAnimationRange[static_cast<int>(m_animationDetails.direction[0])]);
}

void Player::GroundCollision()
{
	// 中心座標から矩形を求める
	m_rect = EvoLib::Convert::PosToRect(m_pos, m_size);

	// マップチップのサイズを取得
	const float mapChipSize = m_pObjectManager->GetMapInfoData().mapChip.chipSize;

	// マップチップの最大セルを取得
	const Cell maxCell = Cell(m_pObjectManager->GetMapInfoData().mapChip.mapWidth, m_pObjectManager->GetMapInfoData().mapChip.mapHeight);

	// 地面セル番号
	std::vector<int>groundCellNumber;
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::Ground));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::IceBlock));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TopConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::BottomConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::LeftConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::RigthConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TransparentBlock));


	// 移動可能範囲の矩形を取得
	m_moveRect = EvoLib::Calculation::CalculateRectangleMovementRange
	(m_rect, maxCell, mapChipSize, m_pObjectManager->GetMapChipNumber(), groundCellNumber);



	// 取得した矩形をm_posの移動可能範囲に変換
	m_moveRect.left += m_size.x * 0.5f;
	m_moveRect.right -= m_size.x * 0.5f;
	m_moveRect.top += m_size.y * 0.5f;
	m_moveRect.bottom -= m_size.y * 0.5f;


	// 移動量を座標に代入
	m_pos += m_vec;



	// 地面判定初期化
	m_isGround = false;


	if (m_playerStatus.gravityDirection == Direction::Top ||
		m_playerStatus.gravityDirection == Direction::Bottom)
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

		if (m_playerStatus.gravityDirection == Direction::Bottom)
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

		if (m_playerStatus.gravityDirection == Direction::Left)
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
	std::vector<std::vector<ObjectManager::MapCollisionData>> mapCollisionData =
		m_pObjectManager->GetMapInfoData().mapCollisionData;

	// マップの幅と高さを取得
	const int mapWidth = static_cast<int>(mapCollisionData.size());
	const int mapHeight = static_cast<int>(mapCollisionData[0].size());


	// 座標を四角形情報に変換
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));


	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			//マップ判定データが画面外の場合、次のループに移る
			if (!mapCollisionData[x][y].screenFlag)
			{
				continue;
			}

			// 四角形同士の当たり判定
			if (!EvoLib::Collision::IsSquareToSquare(mapCollisionData[x][y].square, square))
			{
				continue;
			}

			// セーブポイントの当たり判定を行うかどうか
			const bool isSavePointCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::Save;


			// マップ移動フラグを行うかどうか
			const bool isMapMove =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::NextStage ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::PreviouseStage;


			// 障害物の当たり判定を行うかどうか
			const bool isObstacleCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::TopNeedle ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::BottomNeedle ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::LeftNeedle ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::RightNeedle ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::DiedBlock;


			// 重力方向を変更するかどうか
			const bool isGravity =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::TopGravity ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::BottomGravity ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::LeftGravity ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::RightGravity;
				
			// アイスブロックの当たり判定を行うかどうか
			const bool isIceBlockCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::IceBlock;

			// コンベアの当たり判定を行うかどうか
			const bool isConveyorCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::TopConveyor ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::BottomConveyor ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::LeftConveyor ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::RigthConveyor;

			// ジャンプタイプブロックの当たり判定を行うかどうか
			const bool isJumpTypeCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::InfiniteJump ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::SecondJump;

			// ジャンプ力の変更の当たり判定を行うかどうか
			const bool isJumpPowerCollision =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::NormalJumpPower ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::StrongJumpPower ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::WeakJumpPower;
	
			// 加速度を変更するかどうか
			const bool isAcceleration =
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::Acceleration ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::Deceleration ||
				mapCollisionData[x][y].chipType == ObjectManager::ChipType::NormalSpeed;

			// セーブポイントの当たり判定を行うかどうか
			if (isSavePointCollision)
			{
				// セーブポイントの当たり判定
				SavePointCollision();
			}

			// マップ移動の当たり判定を行うかどうか
			if (isMapMove)
			{
				// マップチップの当たり判定
				MapMove(mapCollisionData[x][y]);
			}
			
			// 障害物の当たり判定を行うかどうか
			if (isObstacleCollision)
			{
				// 障害物の当たり判定
				ObstacleCollision(mapCollisionData[x][y], square);
			}

			// 重力方向を変更するかどうか
			if(isGravity)
			{
				// 重力方向を変更する
				Gravity(mapCollisionData[x][y]);
			}
			
			// アイスブロックの当たり判定を行うかどうか
			if (isIceBlockCollision)
			{
				// アイスブロックの当たり判定
				IceBlockCollision();
			}

			// コンベアの当たり判定を行うかどうか
			if(isConveyorCollision)
			{
				// コンベアの当たり判定
				ConveyorCollision(mapCollisionData[x][y]);
			}

			// ジャンプタイプブロックの当たり判定を行うかどうか
			if(isJumpTypeCollision)
			{
				// ジャンプタイプブロックの当たり判定
				JumpTypeCollision(mapCollisionData[x][y]);
			}

			// ジャンプ力の変更の当たり判定を行うかどうか
			if(isJumpPowerCollision)
			{
				// ジャンプ力の変更の当たり判定
				JumpPowerCollision(mapCollisionData[x][y]);
			}

			// 加速度を変更するかどうか
			if(isAcceleration)
			{
				// 加速度を変更する
				AccelerationCollision(mapCollisionData[x][y]);
			}


			// 存在しない場合、ループを抜ける
			if (!m_isExlist)
			{
				return;
			}
		}
	}
}

void Player::ObstacleCollision(const ObjectManager::MapCollisionData& mapCollisionData, const Square& square)
{
	// 四角形と四角形の当たり判定
	if (mapCollisionData.chipType == ObjectManager::ChipType::DiedBlock)
	{
		// ステートをデッドにする
		m_pStateMachine.SetState(State::Dead);

		return;
	}

	// 三角形の情報を取得
	const Triangle needle = m_pObjectManager->ChipTypeToTriangle(mapCollisionData.chipType, mapCollisionData.square);

	// 三角形と四角形の当たり判定
	if (EvoLib::Collision::IsTriangleToSquare(needle, square))
	{
		// ステートをデッドにする
		m_pStateMachine.SetState(State::Dead);
	}
}

void Player::SavePointCollision()
{
	// セーブポイントをセットする
	m_pObjectManager->SetSavePoint(m_pos, m_playerStatus);
}

void Player::MapMove(const ObjectManager::MapCollisionData& mapCollisionData)
{

	// 次のステージに進む
	if (mapCollisionData.chipType == ObjectManager::ChipType::NextStage)
	{
		m_pObjectManager->StageMove(ObjectManager::MapSwitchType::NextStage);

		return;
	}

	// 前のステージに戻る
	if (mapCollisionData.chipType == ObjectManager::ChipType::PreviouseStage)
	{
		m_pObjectManager->StageMove(ObjectManager::MapSwitchType::PreviouseStage);

		return;
	}
}

void Player::Gravity(const ObjectManager::MapCollisionData& mapCollisionData)
{

	Direction gravityDirection = Direction();

	// 重力方向を変更する
	if (mapCollisionData.chipType == ObjectManager::ChipType::TopGravity)
	{
		gravityDirection = Direction::Top;
	}
	else if (mapCollisionData.chipType == ObjectManager::ChipType::BottomGravity)
	{
		gravityDirection = Direction::Bottom;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::LeftGravity)
	{
		gravityDirection = Direction::Left;
	}
	else if (mapCollisionData.chipType == ObjectManager::ChipType::RightGravity)
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
	m_playerStatus.gravityDirection = gravityDirection;
}

void Player::IceBlockCollision()
{
	m_isIceBlock = true;
}

void Player::ConveyorCollision(const ObjectManager::MapCollisionData& mapCollisionData)
{



	// コンベアの方向を取得
	if(mapCollisionData.chipType == ObjectManager::ChipType::TopConveyor)
	{
		m_conveyor.direction = Direction::Top;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::BottomConveyor)
	{
		m_conveyor.direction = Direction::Bottom;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::LeftConveyor)
	{
		m_conveyor.direction = Direction::Left;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::RigthConveyor)
	{
		m_conveyor.direction = Direction::Right;
	}

	// コンベアに乗っている
	m_conveyor.isFrag = true;
}

void Player::JumpTypeCollision(const ObjectManager::MapCollisionData& mapCollisionData)
{
	if(mapCollisionData.chipType == ObjectManager::ChipType::InfiniteJump)
	{
		m_playerStatus.jumpType = GameData::JumpType::Infinite;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::SecondJump)
	{
		m_playerStatus.jumpType = GameData::JumpType::Second;
	}
}

void Player::JumpPowerCollision(const ObjectManager::MapCollisionData& mapCollisionData)
{
	if (mapCollisionData.chipType == ObjectManager::ChipType::StrongJumpPower)
	{
		m_playerStatus.jumpPower = GameData::JumpPower::Strong;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::WeakJumpPower)
	{
		m_playerStatus.jumpPower = GameData::JumpPower::Weak;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::NormalJumpPower)
	{
		m_playerStatus.jumpPower = GameData::JumpPower::Normal;
	}
}

void Player::AccelerationCollision(const ObjectManager::MapCollisionData& mapCollisionData)
{
	if(mapCollisionData.chipType == ObjectManager::ChipType::NormalSpeed)
	{
		m_playerStatus.moveSpeed = GameData::MoveSpeed::Normal;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::Acceleration)
	{
		m_playerStatus.moveSpeed = GameData::MoveSpeed::Fast;
	}
	else if(mapCollisionData.chipType == ObjectManager::ChipType::Deceleration)
	{
		m_playerStatus.moveSpeed = GameData::MoveSpeed::Slow;
	}
}

void Player::Shot()
{

	if (!Pad::IsTrigger(PAD_INPUT_2))
	{
		return;
	}


	// 方向
	Direction direction = m_animationDetails.direction[0];



	if (m_playerStatus.gravityDirection == Direction::Left ||
		m_playerStatus.gravityDirection == Direction::Right)
	{
		if (m_animationDetails.direction[0] == Direction::Left)
		{
			direction = Direction::Top;
		}
		else if(m_animationDetails.direction[0] == Direction::Right)
		{
			direction = Direction::Bottom;
		}
	}





	//// ショットデータを代入
	//GameData::ShotData shotData =
	//{
	//	GameData::ShotType::ReflectionShot,
	//	m_pos,
	//	EvoLib::Convert::ConvertDirectionToAngle(direction),
	//	Shot::kSpeed,
	//};



	//m_pObjectManager->SetShotCount(shotData);
	


	// 分割する個数
	const int splitCount = 2;
	// 基準角度
	const float baseAngle = 300.0f;



	// ショットデータを代入
	GameData::ShotData shotData =
	{
		GameData::ShotType::SineCurveShot,
		m_pos,
		0.0f,
		Shot::kSpeed,
	};

	// 角度リスト
	std::vector<float> angleList = 
		EvoLib::Calculation::AngleDivision(splitCount, baseAngle);
	

	for(auto shot: angleList)
	{
		shotData.angle = shot;

		m_pObjectManager->SetShotCount(shotData);
	}


}
