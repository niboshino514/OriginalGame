#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"


#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
#include <cassert>
#include <string>
#include <filesystem>

namespace
{



	// �}�b�v�f�[�^�t�@�C���p�X
	const std::string kFieldDataFilePath = "Data/mapData_";

	// �t�@�C���p�X�g���q
	const std::string kFilePathExtension = ".fmf";


	const std::string kTestFilePath = "Data/mapData_1.fmf";
}

namespace
{
	// �~�̔��a
	constexpr float kCircleRadius = 10.0f;
}


ObjectFactory::ObjectFactory() :
	m_object(),
	m_mapInfo(),
	m_stageNumber(0),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>())
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	InitMapDataFilePath();


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

	TestMapDraw();
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

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType)
{

	testMapData = mapData;

	// �l�p�`���
	Square square = Square();

	// �}�b�v�`�b�v�̍�����W
	Vec2 topLeftmapChipPos = Vec2();


	//// ���ׂẴZ��������
	//for (int y = 0; y < m_mapInfo.mapHeight; y++)
	//{
	//	for (int x = 0; x < m_mapInfo.mapWidth; x++)
	//	{
	//		// �}�b�v�`�b�v�̍�����W���
	//		topLeftmapChipPos = Vec2(x * m_mapInfo.chipSize, y * m_mapInfo.chipSize);

	//		// �l�p�`�����
	//		square.A = topLeftmapChipPos;
	//		square.B = Vec2(square.A.x + m_mapInfo.chipSize, square.A.y);
	//		square.C = Vec2(square.A.x + m_mapInfo.chipSize, square.A.y + m_mapInfo.chipSize);
	//		square.D = Vec2(square.A.x, square.A.y + m_mapInfo.chipSize);




	//		switch (mapData[x][y])
	//		{
	//		case 0:
	//			// �N���s�}�b�v�`�b�v����
	//			//m_object.push_back(std::make_shared<NoneMapChip>());

	//			break;
	//
	//		case 1:
	//			// ��Q���}�b�v�`�b�v����
	//			m_object.push_back(std::make_shared<ObstacleMapChip>());

	//			break;
	//		case 2:
	//			// ���̃X�e�[�W�ɐi�ރ}�b�v�`�b�v����
	//			m_object.push_back(std::make_shared<NextStageMapChip>());

	//			break;
	//		case 3:
	//			// �O�̃X�e�[�W�ɖ߂�}�b�v�`�b�v����
	//			m_object.push_back(std::make_shared<PreviousStageMapChip>());

	//			break;

	//		default:

	//			if (mapSwitchType == MapSwitchType::Spawn && mapData[y][x] == 6)
	//			{
	//				// �L�����N�^�[����
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//				
	//			}
	//			else if ((mapSwitchType == MapSwitchType::NextStage && mapData[y][x] == 4))
	//			{
	//				// �L�����N�^�[����
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//			
	//			}
	//			else if ((mapSwitchType == MapSwitchType::PreviousStage && mapData[y][x] == 5))
	//			{
	//				// �L�����N�^�[����
	//				CharacterCreate(MapChipCenterPos(topLeftmapChipPos));

	//			}

	//			break;
	//		}
	//	

	//		// �|�C���^�𑗂�
	//		m_object.back()->SetObjectFactory(shared_from_this());

	//		// ���W������
	//		m_object.back()->SetSquare(square);

	//		// �����ݒ�
	//		m_object.back()->Init();
	//	}
	//}
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
	}
	else if(mapSwitchType == MapSwitchType::PreviousStage)
	{
		// �X�e�[�W�i���o�[�̐��������炷;
		m_stageNumber--;
	}

	// �v�f�폜
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// �v���`�i���f�[�^���[�h
	//m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	m_pPlatinumLoader->Load(kTestFilePath.c_str());

	// �v���`�i���f�[�^�擾
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// �}�b�v�̃��C���[
	const int mapLayer = 0;

	
	// �}�b�v���擾
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();


	// ���̃X�e�[�W�̃}�b�v
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


Vec2 ObjectFactory::MapChipCenterPos(const Vec2& topLeftmapChipPos)
{
	// ���S���W�Ƀ}�b�v�`�b�v�̍�����W����
	Vec2 centerPos = topLeftmapChipPos;

	// �}�b�v�`�b�v�̔����T�C�Y�𑫂�
	centerPos.x += (m_mapInfo.chipSize * 0.5f);
	centerPos.y += (m_mapInfo.chipSize * 0.5f);

	// �}�b�v�`�b�v�̒��S���W��Ԃ�
	return centerPos;
}

void ObjectFactory::InitMapDataFilePath()
{

	// �������J�E���g
	int processCount = 0;

	while (true)
	{


		// �������J�E���g��string�^�ɕϊ�
		std::string numberStr = std::to_string(processCount);

		// �t�@�C���p�X���쐬
		std::string filePath =
			kFieldDataFilePath +
			numberStr +
			kFilePathExtension;


		// ���̃t�@�C���p�X�����݂��邩�ǂ����𒲂ׂ�
		if (!std::filesystem::is_regular_file(filePath))
		{
			break;
		}

		

		// �t�@�C���p�X����ǉ�����
		m_mapDataFilePath.push_back(filePath);


		// �����J�E���g�𑝂₷
		processCount++;
	}



}

void ObjectFactory::TestMapDraw()
{
	Vec2 pos1;
	Vec2 pos2;


	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// �`�b�v�ԍ��̑��
			int chipNum = testMapData[x][y];

			// ���W�̑��
			pos1 = Vec2(x * m_mapInfo.chipSize, y * m_mapInfo.chipSize);


			pos2 = Vec2(pos1.x + m_mapInfo.chipSize, pos1.y + m_mapInfo.chipSize);




			int color = 0x00ff00;

			if (chipNum == 1)
			{
				color = 0xff0000;
			}

			Vec2 stringPos = Vec2(pos1.x + m_mapInfo.chipSize * 0.5, pos1.y + m_mapInfo.chipSize * 0.5);

			DrawFormatString(stringPos.x, stringPos.y, 0xffffff, "%d", chipNum);

			// �}�b�v�̕`��
			DrawBox(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}
}