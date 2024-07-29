#include "ObjectFactory.h"
#include "Player.h"
#include "GameData.h"
#include "Camera.h"
#include "game.h"

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
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>()),
	m_pCamera(std::make_shared<Camera>())
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	// �}�b�v�֘A�����ݒ�
	{
		// �}�b�v�f�[�^�����ݒ�
		InitMapDataFilePath();

		// �}�b�v����
		StageMove(MapSwitchType::Spawn);
	}

	// �J�����N���X�ɃI�u�W�F�N�g�t�@�N�g���[�N���X�|�C���^�𑗂�
	m_pCamera->SetObjectFactoryPointer(shared_from_this());

	// �X�N���[���T�[�N��������
	{
		// ��ʂ̎l�p�`���
		Square screenSquare = Square();

		// ��ʂ̎l�p�`������
		screenSquare.A = Vec2(0, 0);
		screenSquare.B = Vec2(Game::kScreenWidth, 0);
		screenSquare.C = Vec2(Game::kScreenWidth, Game::kScreenHeight);
		screenSquare.D = Vec2(0, Game::kScreenHeight);

		// ��ʂ̒��S�_����сA��ʂ���̊e���_�����̍ő�l�𔼌a�Ƃ����~�̏���Ԃ�
		m_screenCircle =
			FunctionConclusion::CalculateQuadrangularCenter(screenSquare);
	}
}

void ObjectFactory::Update()
{
	// �X�V����(�����_���g�p)
	std::for_each(m_object.begin(), m_object.end(),
		[](std::shared_ptr<ObjectBase> object)
		{
			object->Update(); 
		});

	// �X�N���[���`�F�b�N
	ScreenCheck();

	// �J�����X�V
	m_pCamera->Update();

	// �I�u�W�F�N�g�폜
	ObjectErase();
}

void ObjectFactory::Draw()
{
	// �`�惉���N�̋t���ɃI�u�W�F�N�g��`�悷�郋�[�v
	for (int i = static_cast<int>(ObjectBase::DrawRank::RankNum) - 1; i >= 0; --i) {
		// �`�惉���N
		ObjectBase::DrawRank drawRank = static_cast<ObjectBase::DrawRank>(i);

		// �����𖞂������ׂẴI�u�W�F�N�g��`�悷��
		std::for_each(m_object.begin(), m_object.end(),
			[drawRank](const std::shared_ptr<ObjectBase>& object) {
				if (object->GetDrawRank() == drawRank) {
					object->Draw();
				}
			});
	}
	
	// �}�b�v�`��
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

	// �`�惉���N����
	m_object.back()->SetDrawRank(ObjectBase::DrawRank::Rank_1);

	// �~��������
	m_object.back()->SetCircle(circle);

	// �����ݒ�
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType)
{
	// �}�b�v�f�[�^
	std::vector<std::vector<MapCollisionData>>mapCollisionData(
		m_mapInfoData.mapChip.mapWidth, 
		std::vector<MapCollisionData>(m_mapInfoData.mapChip.mapHeight));

	// ���ׂẴZ��������
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// �}�b�v�`�b�v�^�C�v�ɕϊ�
			const ChipType mapChipType = ChipType(mapData[x][y]);

			// �X�|�[��
			if (mapChipType == ChipType::SpawnPos &&
				mapSwitchType == MapSwitchType::Spawn)
			{
				// �Z�[�u���𑗂�
				GameData::GetInstance()->SetSavePointData(GameData::SavePointData(m_mapInfoData.stageNumber, Cell(x, y)));

				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x,y), m_mapInfoData.mapChip.chipSize));
			}

			// ���̃X�e�[�W
			if (mapChipType == ChipType::NextPos &&
				mapSwitchType == MapSwitchType::NextStage)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfoData.mapChip.chipSize));
			}

			// �O�̃X�e�[�W
			if (mapChipType == ChipType::PreviousePos &&
				mapSwitchType == MapSwitchType::PreviouseStage)
			{
				// �L�����N�^�[����
				CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(Cell(x, y), m_mapInfoData.mapChip.chipSize));
			}

			// �}�b�v����f�[�^
			{
				// �`�b�v�^�C�v������
				mapCollisionData[x][y].chipType = ChipType(mapData[x][y]);


				// �l�p�`���v�Z
				mapCollisionData[x][y].square.A = Vec2((m_mapInfoData.mapChip.chipSize * x), (m_mapInfoData.mapChip.chipSize * y));
				mapCollisionData[x][y].square.B = Vec2((mapCollisionData[x][y].square.A.x + m_mapInfoData.mapChip.chipSize), mapCollisionData[x][y].square.A.y);
				mapCollisionData[x][y].square.C = Vec2(mapCollisionData[x][y].square.B.x, (mapCollisionData[x][y].square.B.y + m_mapInfoData.mapChip.chipSize));
				mapCollisionData[x][y].square.D = Vec2(mapCollisionData[x][y].square.A.x, mapCollisionData[x][y].square.C.y);


				// �~���v�Z
				mapCollisionData[x][y].circle =
					FunctionConclusion::CalculateQuadrangularCenter(mapCollisionData[x][y].square);
			}
		}
	}

	// ���X�|�[��
	if (mapSwitchType == MapSwitchType::Respawn)
	{
		// �L�����N�^�[����
		CharacterCreate(FunctionConclusion::CellWithCoordinateToConversion(GameData::GetInstance()->GetSavePointData().cell, m_mapInfoData.mapChip.chipSize));
	}

	// ���̃X�e�[�W�̃}�b�v�f�[�^��������
	m_mapInfoData.mapCollisionData = mapCollisionData;
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
		m_mapInfoData.stageNumber++;
	}
	else if(mapSwitchType == MapSwitchType::PreviouseStage)
	{
		// �X�e�[�W�i���o�[�̐��������炷;
		m_mapInfoData.stageNumber--;
	}
	else if (mapSwitchType == MapSwitchType::Respawn)
	{
		// �Z�[�u�|�C���g�f�[�^�̃X�e�[�W�i���o�[���X�e�[�W�i���o�[�̐����ɑ��
		m_mapInfoData.stageNumber = GameData::GetInstance()->GetSavePointData().stageNumber;
	}


	// �v�f�폜
	for (auto& object : m_object)
	{
		object->SetIsExlist(false);
	}

	// �v���`�i���f�[�^���[�h
	m_pPlatinumLoader->Load(m_mapInfoData.filePath[m_mapInfoData.stageNumber].c_str());
	
	// �v���`�i���f�[�^�擾
	const std::vector<PlatinumLoader::MapData>mapData = m_pPlatinumLoader->GetMapAllData();

	// �}�b�v�̃��C���[
	const int mapLayer = 0;
	
	// �}�b�v���擾
	m_mapInfoData.mapChip = m_pPlatinumLoader->GetMapChip();

	// �}�b�v����
	MapChipCreate(mapData[mapLayer].mapData, mapSwitchType);
}


std::vector<std::vector<int>> ObjectFactory::GetMapChipNumber()
{
	// �񎟌��̗v�f�����������ϒ��z��
	std::vector<std::vector<int>>mapChipNumber(m_mapInfoData.mapChip.mapWidth, std::vector<int>(m_mapInfoData.mapChip.mapHeight));

	
	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			mapChipNumber[x][y] = static_cast<int>(m_mapInfoData.mapCollisionData[x][y].chipType);
		}
	}

	return mapChipNumber;
}

ObjectFactory::ChipType ObjectFactory::GetMapChipType(const Vec2& pos)
{
	// ���W����Z�������߂�
	const Cell cell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfoData.mapChip.chipSize);

	// �Z�����͈͊O�Ȃ�΁A�����ŏ������I������
	if (!FunctionConclusion::IsCellRange(cell, Cell(m_mapInfoData.mapChip.mapWidth, m_mapInfoData.mapChip.mapHeight), Cell(0, 0)))
	{
		return ChipType::NotExists;
	}

	// �}�b�v�`�b�v���Z�[�u�������ꍇ�A�Q�[���f�[�^�N���X�ɃZ�[�u���𑗂�
	if (m_mapInfoData.mapCollisionData[cell.x][cell.y].chipType == ChipType::Save)
	{
		// �Z�[�u���𑗂�
		GameData::GetInstance()->SetSavePointData(GameData::SavePointData(m_mapInfoData.stageNumber, cell));
	}

	// �}�b�v�`�b�v�^�C�v��Ԃ�
	return m_mapInfoData.mapCollisionData[cell.x][cell.y].chipType;
}

void ObjectFactory::SetSavePoint(const Vec2& pos)
{
	// ���W����Z�������߂�
	const Cell saveCell = FunctionConclusion::CoordinateWithCellToConversion(pos, m_mapInfoData.mapChip.chipSize);

	// �Z�[�u�|�C���g�̃Z����ݒ�
	GameData::GetInstance()->SetSavePointData(
		GameData::SavePointData(m_mapInfoData.stageNumber, saveCell));

	return;
}

std::tuple<bool, Vec2>  ObjectFactory::GetSavePointPos()
{
	// �Z�[�u�|�C���g�f�[�^���擾
	const GameData::SavePointData savePointData = GameData::GetInstance()->GetSavePointData();

	// �Z�[�u�|�C���g�f�[�^�̃X�e�[�W�i���o�[�ƌ��݂̃X�e�[�W�i���o�[���قȂ�ꍇ�A�}�b�v�������s��
	if (savePointData.stageNumber != m_mapInfoData.stageNumber)
	{
		// �}�b�v�ړ�����
		StageMove(MapSwitchType::Respawn);

		return std::tuple<bool, Vec2>(true, Vec2());
	}

	// �Z������ϊ��������W��Ԃ�
	return std::tuple<bool, Vec2>(false, FunctionConclusion::CellWithCoordinateToConversion(savePointData.cell, m_mapInfoData.mapChip.chipSize));
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
		m_mapInfoData.filePath.push_back(filePath);


		// �����J�E���g�𑝂₷
		processCount++;
	}	
}

bool ObjectFactory::IsCellCheckOutOfRange(const Cell& cell)
{
	if (cell.x < 0)return true;
	if (cell.x >= m_mapInfoData.mapChip.mapWidth)return true;
	if (cell.y < 0)return true;
	if (cell.y >= m_mapInfoData.mapChip.mapHeight)return true;

	return false;
}

Triangle ObjectFactory::ChipTypeToTriangle(const ChipType& needleDirection, const Square& square)
{

	// ����
	const Vec2 leftTopPos = square.A;
	// ��
	const Vec2 topPos = Vec2(leftTopPos.x + (m_mapInfoData.mapChip.chipSize * 0.5f), leftTopPos.y);
	// �E��
	const Vec2 rightTopPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y);
	// �E
	const Vec2 rightPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y + (m_mapInfoData.mapChip.chipSize * 0.5f));
	// �E��
	const Vec2 rightBottomPos = Vec2(leftTopPos.x + m_mapInfoData.mapChip.chipSize, leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// ��
	const Vec2 bottomPos = Vec2(leftTopPos.x + (m_mapInfoData.mapChip.chipSize * 0.5f), leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// ����
	const Vec2 leftBottomPos = Vec2(leftTopPos.x, leftTopPos.y + m_mapInfoData.mapChip.chipSize);
	// ��
	const Vec2 leftPos = Vec2(leftTopPos.x, leftTopPos.y + (m_mapInfoData.mapChip.chipSize * 0.5f));


	// �O�p�`���
	Triangle triangle = Triangle();

	if (needleDirection == ChipType::TopNeedle)
	{
		// ��
		triangle.pos_1 = topPos;
		// ����
		triangle.pos_2 = leftBottomPos;
		// �E��
		triangle.pos_3 = rightBottomPos;
	}

	if (needleDirection == ChipType::BottomNeedle)
	{
		// ��
		triangle.pos_1 = bottomPos;
		// ����
		triangle.pos_2 = leftTopPos;
		// �E��
		triangle.pos_3 = rightTopPos;
	}

	if (needleDirection == ChipType::LeftNeedle)
	{
		// ��
		triangle.pos_1 = leftPos;
		// �E��
		triangle.pos_2 = rightTopPos;
		// �E��
		triangle.pos_3 = rightBottomPos;
	}

	if (needleDirection == ChipType::RightNeedle)
	{
		// �E
		triangle.pos_1 = rightPos;
		// ����
		triangle.pos_2 = leftTopPos;
		// ����
		triangle.pos_3 = leftBottomPos;
	}

	return triangle;
}

void ObjectFactory::ScreenCheck()
{
	// ����p�̉~���
	Circle collisionMapCircle = Circle();


	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// �~�����R�s�[
			collisionMapCircle = m_mapInfoData.mapCollisionData[x][y].circle;

			// ���W�ɃI�t�Z�b�g�l��������
			collisionMapCircle.centerPos += GameData::GetInstance()->GetCameraPos();

			// �~�̔���
			m_mapInfoData.mapCollisionData[x][y].screenFlag = FunctionConclusion::CircleCollision(collisionMapCircle, m_screenCircle);
		}
	}
}

void ObjectFactory::TestMapDraw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,200);

	Vec2 pos1;
	Vec2 pos2;


	for (int y = 0; y < m_mapInfoData.mapChip.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfoData.mapChip.mapWidth; x++)
		{
			// ��ʓ��łȂ��ꍇ�͕`�悵�Ȃ�
			if(!m_mapInfoData.mapCollisionData[x][y].screenFlag)
			{
				continue;
			}

	

			// ���W�̑��
			pos1 = Vec2(x * m_mapInfoData.mapChip.chipSize, y * m_mapInfoData.mapChip.chipSize);
			pos2 = Vec2(pos1.x + m_mapInfoData.mapChip.chipSize, pos1.y + m_mapInfoData.mapChip.chipSize);

			// �J�������W�𑫂�
			pos1 += GameData::GetInstance()->GetCameraPos();
			pos2 += GameData::GetInstance()->GetCameraPos();


			int color = 0x00ff00;

			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::Ground)
			{
				color = 0xff0000;
			}
			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::Save)
			{
				color = 0xffffff;
			}

			
			// �����̍��W
			const Vec2 stringPos = Vec2(pos1.x + m_mapInfoData.mapChip.chipSize * 0.5f, pos1.y + m_mapInfoData.mapChip.chipSize * 0.5f);
			// �����`��
			DrawFormatString(static_cast<int>(stringPos.x), static_cast<int>(stringPos.y), 0xffffff, "%d", static_cast<int>(m_mapInfoData.mapCollisionData[x][y].chipType));



			// �j�̕`��
			if (m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::RightNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::LeftNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::TopNeedle	||
				m_mapInfoData.mapCollisionData[x][y].chipType == ChipType::BottomNeedle)
			{
				Triangle triangle = ChipTypeToTriangle(m_mapInfoData.mapCollisionData[x][y].chipType, m_mapInfoData.mapCollisionData[x][y].square);

				triangle.pos_1 += GameData::GetInstance()->GetCameraPos();
				triangle.pos_2 += GameData::GetInstance()->GetCameraPos();	
				triangle.pos_3 += GameData::GetInstance()->GetCameraPos();

				// �O�p�`�`��
				DrawTriangleAA(
					triangle.pos_1.x, triangle.pos_1.y,
					triangle.pos_2.x, triangle.pos_2.y,
					triangle.pos_3.x, triangle.pos_3.y, 0xff0000, true);

				continue;
			}

			// �}�b�v�̕`��
			DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, color, false);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}