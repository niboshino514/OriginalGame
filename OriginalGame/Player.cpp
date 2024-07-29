#include "Player.h"
#include "Pad.h"
#include "ObjectFactory.h"
#include <tuple>
#include "GameData.h"

namespace
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 2.0f;

	// �T�C�Y
	const Vec2 kSize(20.0f, 30.0f);
}

namespace
{
	// �W�����v��
	constexpr float kJumpPower = 15.0f;

	// �W�����v��
	constexpr int kJumpCountMax = 2;

	// �d�͑�
	constexpr float kGravity = 1.5f;
	// �d�͏�
	constexpr float kSmallGravity = 0.8f;

	// �ő�ړ���
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
	// �I�u�W�F�N�gID�ݒ�
	m_objectID = ObjectID::Player;

	m_pos = m_circle.centerPos;

	// �X�|�[���n�_������
	SpawnPointInit();
	
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
}

void Player::StateNormalUpdate()
{
	// �ړ�����
	Move();

	// �W�����v����
	Jump();

	// �X�|�[���n�_���痣�ꂽ���ǂ���
	SpawnPointLeave();

	// �����蔻��
	Collision();
}

void Player::StateNormalDraw()
{
	// �I�t�Z�b�g�l���擾
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// �`����W���v�Z
	const Rect drawRect = FunctionConclusion::RectangleCalculation(m_pos + offset, kSize);
	
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
}

void Player::StateNormalExit()
{
}

void Player::SpawnPointInit()
{
	// �}�b�v�`�b�v�̃T�C�Y���擾
	const float mapChipSize = m_pObjectFactory->GetMapInfoData().mapChip.chipSize;

	// �X�|�[���n�_�̃Z��
	const Cell spawnCell = FunctionConclusion::CoordinateWithCellToConversion(m_pos, mapChipSize);

	// �Z���̒��S���W���擾
	Vec2 cellCenterPos = FunctionConclusion::CellWithCoordinateToConversion(spawnCell, mapChipSize);
	cellCenterPos = Vec2(cellCenterPos.x , cellCenterPos.y);

	// �X�|�[���n�_�̎l�p�`�����擾����
	m_spawnPoint.square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(cellCenterPos, Vec2(mapChipSize, mapChipSize)));

	// �X�|�[���n�_���痣�ꂽ���ǂ����̃t���O��false�ɂ���
	m_spawnPoint.isLeave = false;
}

void Player::SpawnPointLeave()
{
	// �X�|�[���n�_���痣��Ă����ꍇ�A�����ŏ������I������
	if(m_spawnPoint.isLeave)
	{
		return;
	}

	// ���ݍ��W���l�p�`���ɕϊ�
	const Square currentSquare = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(m_pos, kSize));

	// ���݂̍��W�̎l�p�`���ƃX�|�[���n�_�̎l�p�`��񂪌������Ă��Ȃ��ꍇ�A�X�|�[���n�_���痣�ꂽ�Ɣ��f����
	if(!FunctionConclusion::CollisionDetectionOfQuadrangleAndQuadrangle(currentSquare, m_spawnPoint.square))
	{
		m_spawnPoint.isLeave = true;
	}
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
	// �ړ��ʏ�����
	m_vec = Vec2();

	// �p�b�h���g�p�����ړ�
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

	// �ړ��ʂɗ����X�s�[�h��������
	m_vec.y = m_jumpInfo.fallSpeed;


	// ���ȏ�̑��x�ɂȂ����瑬�x��}����
	if (m_vec.y > kMaxDirY)
	{
		m_vec.y = kMaxDirY;
	}
}

void Player::Collision()
{
	// �n�ʂ̓����蔻��
	GroundCollision();

	// ���`���
	PosLinearInterpolation();
}

void Player::GroundCollision()
{
	// ���S���W�����`�����߂�
	m_rect = FunctionConclusion::RectangleCalculation(m_pos, kSize);

	// �ړ��\�͈͂̋�`���擾
	m_moveRect = FunctionConclusion::GetMoveEnableRect
	(m_rect, m_pObjectFactory->GetMapInfoData().mapChip, m_pObjectFactory->GetMapChipNumber());

	// �擾������`��m_pos�̈ړ��\�͈͂ɕϊ�
	m_moveRect.left += kSize.x * 0.5f;
	m_moveRect.right -= kSize.x * 0.5f;
	m_moveRect.top += kSize.y * 0.5f;
	m_moveRect.bottom -= kSize.y * 0.5f;


	// �ړ��ʂ����W�ɑ��
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

		// �������x��0.0f�ɂ���
		m_jumpInfo.fallSpeed = 0.0f;

		m_vec.y = 0.0f;
	}
	if (m_pos.y > m_moveRect.bottom)
	{
		// �n�ʂɒ����Ă���̂ŁA�W�����v�t���O��false�ɂ���
		m_jumpInfo.isJump = false;

		// �W�����v�J�E���g�̍ő�l��������
		m_jumpInfo.jumpCount = kJumpCountMax;

		// �������x��0.0f�ɂ���
		m_jumpInfo.fallSpeed = 0.0f;

		m_pos.y = m_moveRect.bottom;
		m_vec.y = 0.0f;
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
		FunctionConclusion::IinearInterpolationCountCalculation(beforePos, m_vec, kSize);

	// ���`��ԍ��W���v�Z
	std::vector<Vec2> iinearInterpolationPos =
		FunctionConclusion::IinearInterpolationPos(beforePos, m_vec, iinearInterpolationCount);

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
				m_spawnPoint.isLeave &&
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::NextStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::PreviouseStage ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::Save;


			// ��Q���̓����蔻����s�����ǂ���
			const bool isObstacleCollision =
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::TopNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::BottomNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::LeftNeedle ||
				mapCollisionData[x][y].chipType == ObjectFactory::ChipType::RightNeedle;


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
		}
	}
}

void Player::ObstacleCollision(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{
	// ���W���l�p�`���ɕϊ�
	const Square square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(pos, kSize));

	// �O�p�`�̏����擾
	const Triangle needle = m_pObjectFactory->ChipTypeToTriangle(mapCollisionData.chipType, mapCollisionData.square);

	if (FunctionConclusion::CollisionDetectionOfQuadrangleAndTriangle(square, needle))
	{
		// �X�e�[�g���f�b�h�ɂ���
		m_pStateMachine.SetState(State::Dead);
	}
}


void Player::MapMove(const ObjectFactory::MapCollisionData& mapCollisionData, const Vec2& pos)
{

	// ���W���l�p�`���ɕϊ�
	const Square square = FunctionConclusion::RectToSquare(FunctionConclusion::RectangleCalculation(pos, kSize));

	// �l�p�`���m�̓����蔻��
	if (!FunctionConclusion::CollisionDetectionOfQuadrangleAndQuadrangle(mapCollisionData.square, square))
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