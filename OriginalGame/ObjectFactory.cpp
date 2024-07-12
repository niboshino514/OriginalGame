#include "ObjectFactory.h"
#include "FunctionConclusion.h"
#include "Player.h"

#include "NoramalMapChip.h"
#include "NoneMapChip.h"
#include "ObstacleMapChip.h"
#include "NextStageMapChip.h"

namespace
{

	
	// �Ֆʏ��
	const std::vector<std::vector<int>> kMap_1 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	};

	// �Ֆʏ��
	const std::vector<std::vector<int>> kMap_2 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 2, 1, 1, 1, 2},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{0, 1, 1, 1, 1, 1, 2, 1, 2, 2},
	};


	


	// �`�b�v�T�C�Y
	constexpr float kChipSize = 64.0f;

	std::vector<Map>kTest
	{
		Map(kMap_1),
		Map(kMap_2)
	};

	
	

	Map test = Map(kMap_1);

}

namespace
{

	// �v���C���[�~���
	const Circle kPlayerCircle = Circle(Vec2(100, 100), 10.0f);


}


ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::Init()
{
	// �L�����N�^�[����
	CharacterCreate();

	// �}�b�v����
	MapChipCreate(kMap_1);
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

void ObjectFactory::CharacterCreate()
{
	// �L�����N�^�[����
	m_object.push_back(std::make_shared<Player>());

	// �|�C���^�𑗂�
	m_object.back()->SetObjectFactory(shared_from_this());

	// ���W������
	m_object.back()->SetCircle(kPlayerCircle);

	// �����ݒ�
	m_object.back()->Init();
}

void ObjectFactory::MapChipCreate(const std::vector<std::vector<int>>& mapData)
{
	// �}�b�v�c�������
	const int cellWidth = static_cast<int>(mapData[0].size());
	const int cellHeight = static_cast<int>(mapData.size());


	// �l�p�`���
	Square square = Square();


	// ���ׂẴZ��������
	for (int y = 0; y < cellHeight; y++) 
	{
		for (int x = 0; x < cellWidth; x++)
		{
			// �l�p�`�����
			square.A = Vec2(x * kChipSize, y * kChipSize);
			square.B = Vec2(square.A.x + kChipSize, square.A.y);
			square.C = Vec2(square.A.x + kChipSize, square.A.y + kChipSize);
			square.D = Vec2(square.A.x, square.A.y + kChipSize);

			switch (mapData[y][x])
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
			default:
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

void ObjectFactory::NextStageToMigration()
{
	for (auto& object : m_object)
	{
		if (object->GetObjectID() == ObjectBase::ObjectID::Player)
		{
			continue;
		}


		object->SetIsExlist(false);
	}


	// ���̃X�e�[�W�̃}�b�v
	MapChipCreate(kMap_2);
}