#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"
#include "PlatinumLoader.h"
#include <string>

class ObjectBase;
class PlatinumLoader;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:

	// �X�e�[�W�ړ��^�C�v
	enum class MapSwitchType
	{
		Spawn,			// �X�|�[��
		NextStage,		// ���̃X�e�[�W
		PreviousStage	// �O�̃X�e�[�W
	};

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
	void MapChipCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType);


	/// <summary>
	/// �I�u�W�F�N�g�폜
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// �X�e�[�W�ړ�
	/// </summary>
	void StageMove(const MapSwitchType& mapSwitchType);

	/// <summary>
	/// �}�b�v�`�b�v�̒��S���W���v�Z
	/// </summary>
	/// <param name="topLeftmapChipPos">�}�b�v�`�b�v�̍�����W</param>
	/// <returns>�}�b�v�`�b�v�̒��S���W</returns>
	Vec2 MapChipCenterPos(const Vec2& topLeftmapChipPos);


	/// <summary>
	/// �I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g���</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }

private:


	/// <summary>
	/// �}�b�v�f�[�^�t�@�C���p�X�̏����ݒ�
	/// </summary>
	void InitMapDataFilePath();


	void TestMapDraw();

private:

	// �I�u�W�F�N�g
	std::list<std::shared_ptr<ObjectBase>>m_object;

	// �}�b�v���
	PlatinumLoader::MapInfo m_mapInfo;

	// �X�e�[�W�i���o�[
	int m_stageNumber;

	// �}�b�v�f�[�^�̃t�@�C���p�X
	std::vector<std::string> m_mapDataFilePath;


	std::vector<std::vector<int>> testMapData;

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// �v���`�i�����[�_�[
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

};