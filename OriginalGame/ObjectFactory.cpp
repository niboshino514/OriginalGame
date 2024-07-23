#include "ObjectFactory.h"
#include "Player.h"


#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"
#include "PreviousStageMapChip.h"
#include <cassert>
#include <string>
#include <filesystem>
#include <DxLib.h>

namespace
{
	// �}�b�v�f�[�^�t�@�C���p�X
	const std::string kFieldDataFilePath = "Data/mapData_";

	// �t�@�C���p�X�g���q
	const std::string kFilePathExtension = ".fmf";

	// ���蔼�a
	constexpr float kHurtboxRadius = 2.0f;
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
	// �}�b�v�f�[�^�����ݒ�
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
	// ���ׂẴZ��������
	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// �}�b�v�`�b�v�^�C�v�ɕϊ�
			const MapChipType mapChipType = MapChipType(mapData[x][y]);

			// �X�|�[��
			if (mapChipType == MapChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y),m_mapInfo.chipSize));
			}

			// ���̃X�e�[�W
			if (mapChipType == MapChipType::NextPos &&
				mapSwitchType == MapSwitchType::NextStage)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize));
			}

			// �O�̃X�e�[�W
			if (mapChipType == MapChipType::PreviousePos &&
				mapSwitchType == MapSwitchType::PreviouseStage)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize));
			}
		}
	}
}

void ObjectFactory::ObjectErase()
{	
	// ���ׂẴI�u�W�F�N�g�����āA�폜����
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
	else if(mapSwitchType == MapSwitchType::PreviouseStage)
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
	m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	

	// �v���`�i���f�[�^�擾
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// �}�b�v�̃��C���[
	const int mapLayer = 0;

	
	// �}�b�v���擾
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();

	// ���̃X�e�[�W�̃}�b�v�f�[�^��������
	m_currentMapData = mapData[mapLayer].mapData;

	// ���̃X�e�[�W�̃}�b�v
	MapChipCreate(m_currentMapData, mapSwitchType);
}


ObjectFactory::MapChipType ObjectFactory::GetMapChipType(const Vec2& pos)
{
	// ���W����Z�������߂�
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	// �Z�����͈͊O�Ȃ�΁A�����ŏ������I������
	if (!FunctionConclusion::IsCellRange(cell, Cell(m_mapInfo.mapWidth, m_mapInfo.mapHeight), Cell(0, 0)))
	{
		return MapChipType::NotExists;
	}

	// �}�b�v�`�b�v�^�C�v���i�[
	const MapChipType mapChipType = MapChipType(m_currentMapData[cell.x][cell.y]);

	// �}�b�v�`�b�v�^�C�v��Ԃ�
	return mapChipType;
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

bool ObjectFactory::IsCellCheckOutOfRange(const Cell& cell)
{
	if (cell.x < 0)return true;
	if (cell.x >= m_mapInfo.mapWidth)return true;
	if (cell.y < 0)return true;
	if (cell.y >= m_mapInfo.mapHeight)return true;

	return false;
}

void ObjectFactory::TestMapDraw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);

	Vec2 pos1;
	Vec2 pos2;


	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			// �`�b�v�ԍ��̑��
			const int chipNum = m_currentMapData[x][y];

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

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}