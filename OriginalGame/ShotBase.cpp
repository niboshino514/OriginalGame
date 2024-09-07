#include "ShotBase.h"
#include "EvoLib.h"

namespace
{
	// �V���b�g�̃X�s�[�h
	constexpr float kShotSpeed = 10.0f;
}

namespace Size
{
	// �V���b�g���a�T�C�Y
	constexpr float kShotRadius = 5.0f;
}


ShotBase::ShotBase():
	m_isHitGround(false)
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Init()
{
}

void ShotBase::Update()
{
	// �V���b�g�̈ړ�
	m_pos.x += kShotSpeed;

	// �����蔻��
	GroundCollision();
}

void ShotBase::Draw()
{
	// �I�t�Z�b�g�l���擾
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// �V���b�g�̕`��
	DrawCircleAA(m_pos.x + offset.x, m_pos.y + offset.y, Size::kShotRadius, 5, GetColor(255, 255, 255), TRUE);
}

void ShotBase::GroundCollision()
{
	// �V���b�g�̉~
	m_circle = { m_pos, Size::kShotRadius };


	// �n�ʃZ���ԍ�
	std::vector<ObjectManager::ChipType>groundCellNumber;
	groundCellNumber.push_back(ObjectManager::ChipType::Ground);
	groundCellNumber.push_back(ObjectManager::ChipType::IceBlock);
	groundCellNumber.push_back(ObjectManager::ChipType::TopConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::BottomConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::LeftConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::RigthConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::TransparentBlock);

	

	// �}�b�v�Փ˔���f�[�^
	const std::vector<std::vector<ObjectManager::MapCollisionData>> mapCollData =
		m_pObjectManager->GetMapInfoData().mapCollisionData;


	// �}�b�v�`�b�v
	const auto& mapChip = m_pObjectManager->GetMapInfoData().mapChip;

	
	for (int y = 0; y < mapChip.mapHeight; y++)
	{
		for(int x = 0; x < mapChip.mapWidth; x++)
		{
			
			// �n�ʂƂ̓����蔻��
			for (int i = 0; i < static_cast<int>(groundCellNumber.size()); i++)
			{
				if (mapCollData[x][y].chipType == groundCellNumber[i])
				{
					// �~�Ǝl�p�`�̓����蔻��
					if (EvoLib::Collision::IsAABBCollisionDetection(m_circle, mapCollData[x][y].square))
					{
						// �n�ʂɓ��������t���O�𗧂Ă�
						m_isHitGround = true;
					}
				}
			}
		}
	}
}

void ShotBase::Move()
{
	
	// ���W�A�������߂�
	const float rad = EvoLib::Convert::ConvertAngleToRadian(m_shotData.angle);

	// ���W�A������x�N�g�������߂�
	m_vec.x = cos(rad);
	m_vec.y = sin(rad);

	// �x�N�g���̐��K��
	m_vec = m_vec.normalize();

	// �x�N�g���ɃX�s�[�h���|����
	m_vec *= m_shotData.speed;

	// �e���E�����ɐi��
	m_pos += m_vec;
}
