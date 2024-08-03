#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"
#include <tuple>


namespace
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 5.0f;

	// �T�C�Y
	const Vec2 kSize(20.0f, 30.0f);

	// �X�̖��C��
	constexpr float kFrictionIce = 0.98f;

	// �R���x�A�̑��x
	constexpr float kConveyorSpeed = kMoveSpeed * 0.8f;

}

namespace
{
	// �W�����v��
	constexpr float kJumpPower = 15.0f;

	// �W�����v��
	constexpr int kJumpCountMax = 5;

	// �d�͑�
	constexpr float kGravity = 1.5f;
	// �d�͏�
	constexpr float kSmallGravity = 0.8f;

	// �ő�ړ���
	constexpr float kMaxDir = 20.0f;


	// �d�͔��]�t���O
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
	// �I�u�W�F�N�gID�ݒ�
	m_objectID = ObjectID::Player;

	// �X�e�[�g�}�V���̏�����
	StateInit();
}

void Player::Update()
{
	// �X�e�[�g�}�V���̍X�V
	m_pStateMachine.Update();

	// ���X�|�[��
	Respawn();
}

void Player::Draw()
{
	DrawFormatString(0, 15 * 1, 0xffffff, "���WX:%f,���WY:%f,", m_pos.x, m_pos.y);

	// �X�e�[�g�}�V���̕`��
	m_pStateMachine.Draw();
}


void Player::StateInit()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �ʏ�X�e�[�g
	{
		auto enter = [this]() { StateNormalEnter(); };
		auto update = [this]() { StateNormalUpdate(); };
		auto draw = [this]() { StateNormalDraw(); };
		auto exit = [this]() { StateNormalExit(); };

		m_pStateMachine.AddState(State::Normal, enter, update, draw, exit);
	}
	// ���S�X�e�[�g
	{
		m_pStateMachine.AddState(State::Dead, dummy, dummy, dummy, dummy);
	}

	// �����X�e�[�g��ݒ�
	m_pStateMachine.SetState(State::Normal);
}

void Player::StateNormalEnter()
{
	// �d�͕����ύX
	ChangeGravityDirection(Direction::Bottom);

	// �A�C�X�u���b�N�t���O��false�ɂ���
	m_isIceBlock = false;



	// �R���x�A��񏉊���
	{
		// �R���x�A�ɏ���Ă��邩�ǂ���
		m_conveyor.isFrag = false;

		// �R���x�A�̑��x
		m_conveyor.speed = kConveyorSpeed;
	}

}

void Player::StateNormalUpdate()
{
	// �ړ�����
	Move();

	// �W�����v����
	Jump();


	// �����蔻��
	Collision();
}

void Player::StateNormalDraw()
{
	// �I�t�Z�b�g�l���擾
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();


	// �f�o�b�O�`��
#if(true)
	// �`����W���v�Z
	const Rect drawRect = EvoLib::Convert::PosToRect(m_pos + offset, m_size);
	
	// �ړ��\�͈͂̋�`���擾
	Rect drawMoveRect = Rect();

	drawMoveRect.left = m_moveRect.left + offset.x;
	drawMoveRect.right = m_moveRect.right + offset.x;
	drawMoveRect.bottom = m_moveRect.bottom + offset.y;
	drawMoveRect.top = m_moveRect.top + offset.y;


	// �v���C���[�`��
	DrawBoxAA(drawRect.left, drawRect.top, drawRect.right, drawRect.bottom,
		0xff0000, true);

	// �ړ��ł���ꏊ��`��
	DrawBoxAA(drawMoveRect.left, drawMoveRect.top, drawMoveRect.right, drawMoveRect.bottom,
		0x0000ff, false);
#endif
}

void Player::StateNormalExit()
{
}


void Player::Respawn()
{
	// �Z�[�u�|�C���g�Z�����擾����
	if (Pad::IsPress(PAD_INPUT_1))
	{
		// �X�e�[�W��ύX���邩�ǂ���
		bool isChangeStage = false;

		// �Z�[�u�|�C���g���W���擾����
		std::tie(isChangeStage, m_pos) = m_pObjectFactory->GetSavePointPos();

		// �X�e�[�W��ύX���Ȃ��ꍇ�A�X�e�[�g���m�[�}���ɂ���
		if (!isChangeStage)
		{
			m_pStateMachine.SetState(State::Normal);
		}
	}
}

void Player::Move()
{


	// �ړ���
	Vec2 inputVec = Vec2();


	if (m_gravityDirection == Direction::Top ||
		m_gravityDirection == Direction::Bottom)
	{
		// �p�b�h���g�p�����ړ�
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
		// �p�b�h���g�p�����ړ�
		if (Pad::IsPress(PAD_INPUT_DOWN))
		{
			inputVec.y += kMoveSpeed;
		}
		if (Pad::IsPress(PAD_INPUT_UP))
		{
			inputVec.y -= kMoveSpeed;
		}
	}


	// �R���x�A�ɏ���Ă���ꍇ�A�ړ��ʂ�ύX����
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

		// �R���x�A�ɏ���Ă���ꍇ�A�ړ��ʂ�ύX����
		m_vec = inputVec;
	}

	// �A�C�X�u���b�N�̏ꍇ�A�ړ��ʂ�ύX����
	if(m_isIceBlock)
	{
		m_vec += inputVec;

		m_vec.x = EvoLib::Calculation::Clamp(m_vec.x, -kMoveSpeed, kMoveSpeed);
		m_vec.y = EvoLib::Calculation::Clamp(m_vec.y, -kMoveSpeed, kMoveSpeed);

		m_vec *= kFrictionIce;

		return;
	}

	// �ړ��ʂ���
	m_vec = inputVec;
}

void Player::Jump()
{
	// �{�^�����������Ƃ��A�W�����v�J�E���g��0�ȏ�Ȃ�΃W�����v�͂�^����
	if (Pad::IsTrigger(PAD_INPUT_10)&&
		m_jumpInfo.jumpCount > 0)
	{
		// �W�����v�t���O��true�ɂ���
		m_jumpInfo.isJump = true;

		// �W�����v�͂�^����
		m_jumpInfo.fallSpeed = -kJumpPower;

		// �W�����v�J�E���g�����炷
		m_jumpInfo.jumpCount--;
	}

	// ���ł����ꍇ�̓{�^���������Ă��邩�ǂ����ŗ����X�s�[�h���ς��
	if (m_jumpInfo.isJump)
	{
		// �{�^���������Ă��邩�ǂ����ŁA�W�����v�͂��ς��
		if (Pad::IsPress(PAD_INPUT_10))
		{
			// �������d�͂�^����
			m_jumpInfo.fallSpeed += kSmallGravity;
		}
		else
		{
			// �傫���d�͂�^����
			m_jumpInfo.fallSpeed += kGravity;
		}
	}
	else
	{
		// �W�����v���Ă��Ȃ�������ʏ�̏d�͂�^����
		m_jumpInfo.fallSpeed += kGravity;
	}

	
	if (m_gravityDirection == Direction::Top ||
		m_gravityDirection == Direction::Bottom)
	{
		// �ړ��ʂɗ����X�s�[�h��������
		m_vec.y = m_jumpInfo.fallSpeed;


		if (m_gravityDirection == Direction::Top)
		{
			// �ړ��ʂɗ����X�s�[�h��������
			m_vec.y = -m_jumpInfo.fallSpeed;
		}
		else
		{
			// �ړ��ʂɗ����X�s�[�h��������
			m_vec.y = m_jumpInfo.fallSpeed;
		}

		// ���ȏ�̑��x�ɂȂ����瑬�x��}����
		if (m_vec.y > kMaxDir)
		{
			m_vec.y = kMaxDir;
		}
	}
	else
	{
		// �ړ��ʂɗ����X�s�[�h��������
		m_vec.x = m_jumpInfo.fallSpeed;


		if (m_gravityDirection == Direction::Left)
		{
			// �ړ��ʂɗ����X�s�[�h��������
			m_vec.x = -m_jumpInfo.fallSpeed;
		}
		else
		{
			// �ړ��ʂɗ����X�s�[�h��������
			m_vec.x = m_jumpInfo.fallSpeed;
		}

		// ���ȏ�̑��x�ɂȂ����瑬�x��}����
		if (m_vec.x > kMaxDir)
		{
			m_vec.x = kMaxDir;
		}
	}

}

void Player::Collision()
{

	// ���W���l�p�`���ɕϊ�
	m_square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(m_pos, m_size));


	// �n�ʂ̓����蔻��
	GroundCollision();

	// ���`���
	PosLinearInterpolation();
}

void Player::GroundCollision()
{
	// ���S���W�����`�����߂�
	m_rect = EvoLib::Convert::PosToRect(m_pos, m_size);

	// �}�b�v�`�b�v�̃T�C�Y���擾
	const float mapChipSize = m_pObjectFactory->GetMapInfoData().mapChip.chipSize;

	// �}�b�v�`�b�v�̍ő�Z�����擾
	const Cell maxCell = Cell(m_pObjectFactory->GetMapInfoData().mapChip.mapWidth, m_pObjectFactory->GetMapInfoData().mapChip.mapHeight);

	// �n�ʃZ���ԍ�
	std::vector<int>groundCellNumber;
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::Ground));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::IceBlock));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::TopConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::BottomConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::LeftConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::RigthConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectFactory::ChipType::TransparentBlock));


	// �ړ��\�͈͂̋�`���擾
	m_moveRect = EvoLib::Calculation::CalculateRectangleMovementRange
	(m_rect, maxCell, mapChipSize, m_pObjectFactory->GetMapChipNumber(), groundCellNumber);



	// �擾������`��m_pos�̈ړ��\�͈͂ɕϊ�
	m_moveRect.left += m_size.x * 0.5f;
	m_moveRect.right -= m_size.x * 0.5f;
	m_moveRect.top += m_size.y * 0.5f;
	m_moveRect.bottom -= m_size.y * 0.5f;


	// �ړ��ʂ����W�ɑ��
	m_pos += m_vec;



	// �n�ʔ��菉����
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

				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				m_vec.y = 0.0f;
			}
			if (m_pos.y > m_moveRect.bottom)
			{
				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				// �n�ʃt���O��true�ɂ���
				m_isGround = true;

				m_pos.y = m_moveRect.bottom;
				m_vec.y = 0.0f;
			}
		}
		else
		{
			if (m_pos.y < m_moveRect.top)
			{
				// �n�ʃt���O��true�ɂ���
				m_isGround = true;

				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.y = m_moveRect.top;
				m_vec.y = 0.0f;
			}
			if (m_pos.y > m_moveRect.bottom)
			{
				// �������x��0.0f�ɂ���
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
				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				// �n�ʃt���O��true�ɂ���
				m_isGround = true;

				m_pos.x = m_moveRect.left;
				m_vec.x = 0.0f;
			}
			if (m_pos.x > m_moveRect.right)
			{
				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.x = m_moveRect.right;
				m_vec.x = 0.0f;
			}
		}
		else
		{
			if (m_pos.x < m_moveRect.left)
			{
				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				m_pos.x = m_moveRect.left;
				m_vec.x = 0.0f;
			}
			if (m_pos.x > m_moveRect.right)
			{
		
				// �������x��0.0f�ɂ���
				m_jumpInfo.fallSpeed = 0.0f;

				// �n�ʃt���O��true�ɂ���
				m_isGround = true;

				m_pos.x = m_moveRect.right;
				m_vec.x = 0.0f;
			}
		}
	}


	if(m_isGround)
	{
		// �n�ʂɒ����Ă���̂ŁA�W�����v�t���O��false�ɂ���
		m_jumpInfo.isJump = false;

		// �W�����v�J�E���g�̍ő�l��������
		m_jumpInfo.jumpCount = kJumpCountMax;

		// �A�C�X�u���b�N�t���O��false�ɂ���
		m_isIceBlock = false;
	}



	// ���W���Q�[���f�[�^�ɑ��
	GameData::GetInstance()->SetPlayerPos(m_pos);
}

void Player::PosLinearInterpolation()
{
	// �O�̍��W
	const Vec2 beforePos = m_pos - m_vec;

	// ���`��Ԑ����v�Z
	const int iinearInterpolationCount =
		EvoLib::Calculation::InearInterpolationCount(beforePos, m_vec, m_size);

	

	// ���`��ԍ��W���v�Z
	std::vector<Vec2> iinearInterpolationPos =
		EvoLib::Calculation::InearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

	

	// ���`��Ԑ���0�Ȃ�΁A���W�Ɉړ��ʂ𑫂������̂�z��ɓ����
	if (iinearInterpolationCount == 0)
	{
		// �ړ���̍��W��������
		iinearInterpolationPos.push_back(m_pos);
	}

	// ���`�␳��for������
	for (int i = 0; i < static_cast<int>(iinearInterpolationPos.size()); i++)
	{
		// �}�b�v�`�b�v�̓����蔻��
		MapChipCollision(iinearInterpolationPos[i]);
	}
}

void Player::MapChipCollision(const Vec2& pos)
{
	// �R���x�A�t���O������
	m_conveyor.isFrag = false;

	// �}�b�v����f�[�^���擾
	std::vector<std::vector<ObjectFactory::MapCollisionData>> mapCollisionData =
		m_pObjectFactory->GetMapInfoData().mapCollisionData;

	// �}�b�v�̕��ƍ������擾
	const int mapWidth = static_cast<int>(mapCollisionData.size());
	const int mapHeight = static_cast<int>(mapCollisionData[0].size());


	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			//�}�b�v����f�[�^����ʊO�̏ꍇ�A���̃��[�v�Ɉڂ�
			if (!mapCollisionData[x][y].screenFlag)
			{
				continue;
			}

			// �}�b�v�ړ��t���O���s�����ǂ���
			const bool isMapMove =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::NextStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::PreviouseStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::Save;


			// ��Q���̓����蔻����s�����ǂ���
			const bool isObstacleCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::DiedBlock;


			// �d�͕�����ύX���邩�ǂ���
			const bool isGravity =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftGravity ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightGravity;
				
			// �A�C�X�u���b�N�̓����蔻����s�����ǂ���
			const bool isIceBlockCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::IceBlock;

			// �R���x�A�̓����蔻����s�����ǂ���
			const bool isConveyorCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftConveyor ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RigthConveyor;


			// �}�b�v�ړ��̓����蔻����s�����ǂ���
			if (isMapMove)
			{
				// �}�b�v�`�b�v�̓����蔻��
				MapMove(mapCollisionData[x][y], pos);
			}
			
			// ��Q���̓����蔻����s�����ǂ���
			if (isObstacleCollision)
			{
				// ��Q���̓����蔻��
				ObstacleCollision(mapCollisionData[x][y], pos);
			}

			// �d�͕�����ύX���邩�ǂ���
			if(isGravity)
			{
				// �d�͕�����ύX����
				Gravity(mapCollisionData[x][y], pos);
			}
			
			// �A�C�X�u���b�N�̓����蔻����s�����ǂ���
			if (isIceBlockCollision)
			{
				// �A�C�X�u���b�N�̓����蔻��
				IceBlockCollision(mapCollisionData[x][y], pos);
			}

			// �R���x�A�̓����蔻����s�����ǂ���
			if(isConveyorCollision)
			{
				// �R���x�A�̓����蔻��
				ConveyorCollision(mapCollisionData[x][y], pos);
			}


			// ���݂��Ȃ��ꍇ�A���[�v�𔲂���
			if (!m_isExlist)
			{
				return;
			}
		}
	}
}

void Player::ObstacleCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// �l�p�`�Ǝl�p�`�̓����蔻��
	if (mapCollisionData.chipType == ObjectFactory::ChipType::DiedBlock)
	{
		// �l�p�`���m�̓����蔻��
		if (EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
		{
			// �X�e�[�g���f�b�h�ɂ���
			m_pStateMachine.SetState(State::Dead);
		}

		return;
	}


	// �O�p�`�̏����擾
	const Triangle needle = m_pObjectFactory->ChipTypeToTriangle(mapCollisionData.chipType, mapCollisionData.square);

	// �O�p�`�Ǝl�p�`�̓����蔻��
	if (EvoLib::Collision::IsTriangleToSquare(needle, square))
	{
		// �X�e�[�g���f�b�h�ɂ���
		m_pStateMachine.SetState(State::Dead);
	}
}

void Player::MapMove(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));


	// �l�p�`���m�̓����蔻��
	if (!EvoLib::Collision::IsSquareToSquare(mapCollisionData.square, square))
	{
		return;
	}

	// �Z�[�u�|�C���g�ɓ��������ꍇ�A�Z�[�u����
	if (mapCollisionData.chipType == ObjectFactory::ChipType::Save)
	{
		m_pObjectFactory->SetSavePoint(mapCollisionData.circle.centerPos);
	}

	// ���̃X�e�[�W�ɐi��
	if (mapCollisionData.chipType == ObjectFactory::ChipType::NextStage)
	{
		m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::NextStage);

		return;
	}

	// �O�̃X�e�[�W�ɖ߂�
	if (mapCollisionData.chipType == ObjectFactory::ChipType::PreviouseStage)
	{
		m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::PreviouseStage);

		return;
	}
}

void Player::Gravity(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	
	// �l�p�`���m�̓����蔻��
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}


	Direction gravityDirection = Direction();

	// �d�͕�����ύX����
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

	// �d�͕�����ύX����
	m_gravityDirection = gravityDirection;
}

void Player::IceBlockCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// �l�p�`���m�̓����蔻��
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}

	m_isIceBlock = true;
}

void Player::ConveyorCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = EvoLib::Convert::RectToSquare(EvoLib::Convert::PosToRect(pos, m_size));

	// �l�p�`���m�̓����蔻��
	if (!EvoLib::Collision::IsSquareToSquare(square, mapCollisionData.square))
	{
		return;
	}

	// �R���x�A�̕������擾
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

	// �R���x�A�ɏ���Ă���
	m_conveyor.isFrag = true;
}
