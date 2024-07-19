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
	// �}�b�v�f�[�^�R�s�[
	m_currentMapData = mapData;

	// �}�b�v�`�b�v�̃n�[�g�{�b�N�X�i�[�ϐ�
	std::vector<std::vector<Hurtbox>> mapChipHurtbox(m_mapInfo.mapWidth, std::vector<Hurtbox>(m_mapInfo.mapHeight));

	// �}�b�v�`�b�v�̍�����W
	Vec2 topLeftmapChipPos = Vec2();

	// ���ׂẴZ��������
	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{

			// �}�b�v�`�b�v�^�C�v�ɕϊ�
			const MapChipType mapChipType = MapChipType(mapData[x][y]);


			// �n�[�g�{�b�N�X
			{
				mapChipHurtbox[x][y].centerPos = FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfo.chipSize);
				mapChipHurtbox[x][y].topPos = Vec2(mapChipHurtbox[x][y].centerPos.x, mapChipHurtbox[x][y].centerPos.y - (m_mapInfo.chipSize * 0.5f));
				mapChipHurtbox[x][y].bottomPos = Vec2(mapChipHurtbox[x][y].centerPos.x, mapChipHurtbox[x][y].centerPos.y + (m_mapInfo.chipSize * 0.5f));
				mapChipHurtbox[x][y].leftPos = Vec2(mapChipHurtbox[x][y].centerPos.x - (m_mapInfo.chipSize * 0.5f), mapChipHurtbox[x][y].centerPos.y);
				mapChipHurtbox[x][y].rightPos = Vec2(mapChipHurtbox[x][y].centerPos.x + (m_mapInfo.chipSize * 0.5f), mapChipHurtbox[x][y].centerPos.y);
			}


			if (mapChipType == MapChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y),m_mapInfo.chipSize));
			}
		}
	}


	// �}�b�v�`�b�v�n�[�g�{�b�N�X�R�r�[
	m_mapChipHurtbox = mapChipHurtbox;
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
	m_pPlatinumLoader->Load(m_mapDataFilePath[m_stageNumber].c_str());
	

	// �v���`�i���f�[�^�擾
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();


	// �}�b�v�̃��C���[
	const int mapLayer = 0;

	
	// �}�b�v���擾
	m_mapInfo = m_pPlatinumLoader->GetMapInfo();


	// ���̃X�e�[�W�̃}�b�v
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


ObjectFactory::MapChipType ObjectFactory::MapChipTypeFromCoordinate(const Vec2& pos)
{
	// �}�b�v�`�b�v��񂩂�`�b�v�^�C�v��Ԃ�
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	// �Z�����͈͊O���ǂ����𒲂ׁA�͈͊O�Ȃ�Ώ������I������
	if (IsCellCheckOutOfRange(cell))
	{
		// ���݂��Ȃ�
		return MapChipType::NotExists;
	}

	// �`�b�v�^�C�v��Ԃ�
	return MapChipType(m_currentMapData[cell.x][cell.y]);
}

Vec2 ObjectFactory::CorrectionCoordinateValue(const Vec2& pos, const HurtboxDrection& hurtboxDrection)
{
	// �␳���W
	Vec2 correctionPos;


	// �}�b�v�`�b�v��񂩂�`�b�v�^�C�v��Ԃ�
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfo.chipSize);

	if (hurtboxDrection == HurtboxDrection::Top)
	{
		correctionPos.y = ((m_mapChipHurtbox[cell.x][cell.y].bottomPos.y - pos.y) - 1.0f);
	}
	else if (hurtboxDrection == HurtboxDrection::Bottom)
	{
		correctionPos.y = (m_mapChipHurtbox[cell.x][cell.y].topPos.y - pos.y);
	}
	else if (hurtboxDrection == HurtboxDrection::Left)
	{
		correctionPos.x = ((m_mapChipHurtbox[cell.x][cell.y].rightPos.x - pos.x) - 1.0f);
	}
	else if (hurtboxDrection == HurtboxDrection::Right)
	{
		correctionPos.x = (m_mapChipHurtbox[cell.x][cell.y].leftPos.x - pos.x);
	}


	return correctionPos;
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