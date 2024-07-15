#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"

#include "NoramalMapChip.h"
#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
#include <cassert>

namespace
{

	// 0:�����Ȃ�(�N���s��)
	// 1:�ʏ�Z��
	// 2:��Q���Z��(�N���s��)
	// 3:���̃X�e�[�W�ɐi��
	// 4:�O�̃X�e�[�W�ɖ߂�
	// 5:���̃X�e�[�W�ɐi�񂾍ۂ̃v���C���[�̏����Z��
	// 6:�O�̃X�e�[�W�ɖ߂����ۂ̃v���C���[�̏����Z��
	// 7:�X�|�[���Z��(�Q�[�����n�߂��ۂ̏����Z����)
	
	
	// �Ֆʏ��
	const std::vector<std::vector<int>> kMap_1 =
	{
		{0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 6, 3},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	};

	// �Ֆʏ��
	const std::vector<std::vector<int>> kMap_2 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{4, 5, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 2, 1},
	};


	// �`�b�v�T�C�Y
	constexpr float kChipSize = 64.0f;


	// �}�b�v���
	std::vector<Map>kMapInfo
	{
		Map(kMap_1),
		Map(kMap_2)
	};


}

namespace
{
	// �~�̔��a
	constexpr float kCircleRadius = 10.0f;
}


ObjectFactory::ObjectFactory() :
	m_stageNumber(0)
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	
	// �}�b�v����
	StageMove(MapSwitchType::Spawn);
}

void ObjectFactory::Update()
{
	// �X�V����
	for (auto& object : m_object)
	{
		object->Update();
	}

	// �I�u�W�F�N�g�폜
	ObjectErase();
}

void ObjectFactory::Draw()
{
	// �`�揈��
	for (auto& object : m_object)
	{
		object->Draw();
	}
}

void ObjectFactory::CharacterCreate(const Vec2& pos)
{
	// �~�����
	Circle circle = Circle(pos, kCircleRadius);


	// �L�����N�^�[����
	m_object.push_back(std::make_shared<Player>());

	// �|�C���^�𑗂�
	m_object.back()->SetObjectFactory(shared_from_this());

	// �~��������
	m_object.back()->SetCircle(circle);

	// �����ݒ�
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const Map& mapData, const MapSwitchType& mapSwitchType)
{
	// �}�b�v�c�������
	const int cellWidth = static_cast<int>(mapData.mapInfo[0].size());
	const int cellHeight = static_cast<int>(mapData.mapInfo.size());


	// �l�p�`���
	Square square = Square();

	// �}�b�v�`�b�v�̍�����W
	Vec2 topLeftmapChipPos = Vec2();


	// ���ׂẴZ��������
	for (int y = 0; y < cellHeight; y++) 
	{
		for (int x = 0; x < cellWidth; x++)
		{
			// �}�b�v�`�b�v�̍�����W���
			topLeftmapChipPos = Vec2(x * kChipSize, y * kChipSize);

			// �l�p�`�����
			square.A = topLeftmapChipPos;
			square.B = Vec2(square.A.x + kChipSize, square.A.y);
			square.C = Vec2(square.A.x + kChipSize, square.A.y + kChipSize);
			square.D = Vec2(square.A.x, square.A.y + kChipSize);

			switch (mapData.mapInfo[y][x])
			{
			case 0:
				// �N���s�}�b�v�`�b�v����
				m_object.push_back(std::make_shared<NoneMapChip>());

				break;
			case 1:
				// �ʏ�}�b�v�`�b�v����
				m_object.push_back(std::make_shared<NoramalMapChip>());

				break;

			case 2:
				// ��Q���}�b�v�`�b�v����
				m_object.push_back(std::make_shared<ObstacleMapChip>());

				break;
			case 3:
				// ���̃X�e�[�W�ɐi�ރ}�b�v�`�b�v����
				m_object.push_back(std::make_shared<NextStageMapChip>());

				break;
			case 4:
				// �O�̃X�e�[�W�ɖ߂�}�b�v�`�b�v����
				m_object.push_back(std::make_shared<PreviousStageMapChip>());

				break;

			default:

				if (mapSwitchType == MapSwitchType::Spawn && mapData.mapInfo[y][x] == 7)
				{
					// �L�����N�^�[����
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// �ʏ�}�b�v�`�b�v����
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}
				else if ((mapSwitchType == MapSwitchType::NextStage && mapData.mapInfo[y][x] == 5))
				{
					// �L�����N�^�[����
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// �ʏ�}�b�v�`�b�v����
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}
				else if ((mapSwitchType == MapSwitchType::PreviousStage && mapData.mapInfo[y][x] == 6))
				{
					// �L�����N�^�[����
					CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

					// �ʏ�}�b�v�`�b�v����
					m_object.push_back(std::make_shared<NoramalMapChip>());
				}

				break;
			}
		

			// �|�C���^�𑗂�
			m_object.back()->SetObjectFactory(shared_from_this());

			// ���W������
			m_object.back()->SetSquare(square);

			// �����ݒ�
			m_object.back()->Init();
		}
	}
}

void ObjectFactory::ObjectErase()
{	// ���ׂẴI�u�W�F�N�g�����āA�폜����
	auto rmIt = std::remove_if
	(m_object.begin(), m_object.end(), [](const std::shared_ptr<ObjectBase>& object)
		{
			return !object->GetIsExlist();
		}
	);

	// �폜
	m_object.erase(rmIt, m_object.end());
}

void ObjectFactory::StageMove(const MapSwitchType& mapSwitchType)
{
	// ���̃X�e�[�W�Ɉړ����邩�ǂ����̃t���O����X�e�[�W�i���o�[�𑝂₷�����߂�
	if (mapSwitchType == MapSwitchType::NextStage)
	{
		// �X�e�[�W�i���o�[�̐����𑝂₷
		m_stageNumber++;


		int test = static_cast<int>(kMapInfo.size());


		assert((test > m_stageNumber) &&
			"�X�e�[�W�i���o�[�ɏ���ȏ�̐����������Ă���悤�ł��B");

	}
	else if(mapSwitchType == MapSwitchType::PreviousStage)
	{
		// �X�e�[�W�i���o�[�̐��������炷;
		m_stageNumber--;

		assert((-1 < m_stageNumber) &&
			"�X�e�[�W�i���o�[��0�ȉ��̐����������Ă���悤�ł��B");
	}

	// �v�f�폜
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}


	// ���̃X�e�[�W�̃}�b�v
	MapChipCreate(kMapInfo[m_stageNumber], mapSwitchType);
}


Vec2 ObjectFactory::MapChipCenterPos(const Vec2& topLeftmapChipPos)
{
	// ���S���W�Ƀ}�b�v�`�b�v�̍�����W����
	Vec2 centerPos = topLeftmapChipPos;

	// �}�b�v�`�b�v�̔����T�C�Y�𑫂�
	centerPos.x += (kChipSize * 0.5f);
	centerPos.y += (kChipSize * 0.5f);

	// �}�b�v�`�b�v�̒��S���W��Ԃ�
	return centerPos;
}