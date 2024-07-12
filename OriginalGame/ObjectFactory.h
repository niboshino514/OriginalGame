#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"


// �}�b�v
struct Map
{
	// �}�b�v���
	std::vector<std::vector<int>> mapInfo;
};

class ObjectBase;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:



public:
	ObjectFactory();
	virtual ~ObjectFactory();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// �L�����N�^�[����
	/// </summary>
	void CharacterCreate(const Vec2& pos);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="�}�b�v���"></param>
	void MapChipCreate(const Map& mapData);


	/// <summary>
	/// �I�u�W�F�N�g�폜
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// ���̃X�e�[�W�ֈڍs
	/// </summary>
	void NextStageToMigration();


	/// <summary>
	/// �}�b�v�`�b�v�̒��S���W���v�Z
	/// </summary>
	/// <param name="topLeftmapChipPos">�}�b�v�`�b�v�̍�����W</param>
	/// <returns>�}�b�v�`�b�v�̒��S���W</returns>
	Vec2 MapChipCenterPos(const Vec2& topLeftmapChipPos);


	/// <summary>
	/// �}�b�v�؂�ւ�
	/// </summary>
	void MapSwitch();



	/// <summary>
	/// �I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g���</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


private:

	// �I�u�W�F�N�g
	std::list<std::shared_ptr<ObjectBase>>m_object;

	// �L�����N�^�[�������W���
	std::vector<Vec2>m_characterPos;

	
	// �X�e�[�W�i���o�[
	int m_stageNumber;


	std::vector<Map>m_mapInfo_test;



};