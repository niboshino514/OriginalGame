#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Vec2.h"
#include "PlatinumLoader.h"
#include <string>
#include "FunctionConclusion.h"


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
		PreviouseStage	// �O�̃X�e�[�W
	};


	// �}�b�v�`�b�v�^�C�v
	enum class MapChipType
	{
		None,			// �����Ȃ�
		Ground,			// �n��
		NextStage,		// ���̃X�e�[�W
		PreviouseStage,	// �O�̃X�e�[�W
		NextPos,		// ���̃X�e�[�W���W
		PreviousePos,	// �O�̃X�e�[�W���W
		SpawnPos,		// �X�|�[�����W
		NotExists		// ���݂��Ȃ�
	};

	// �n�[�g�{�b�N�X�̌���
	enum class HurtboxDrection
	{
		Top,	// ��
		Bottom,	// ��
		Left,	// ��
		Right,	// �E
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
	/// �I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g���</returns>
	std::vector<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


	/// <summary>
	/// ���݂̃}�b�v�f�[�^��Ԃ�
	/// </summary>
	/// <returns></returns>
	PlatinumLoader::MapData GetCurrentMapData() { return PlatinumLoader::MapData(m_currentMapData); }


	/// <summary>
	/// �}�b�v����Ԃ�
	/// </summary>
	/// <returns>�}�b�v���</returns>
	PlatinumLoader::MapInfo GetMapInfo() { return m_mapInfo; }
	

	/// <summary>
	/// �}�b�v�`�b�v����Ԃ�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <returns>�}�b�v�`�b�v���</returns>
	MapChipType GetMapChipType(const Vec2& pos);

	
private:


	/// <summary>
	/// �}�b�v�f�[�^�t�@�C���p�X�̏����ݒ�
	/// </summary>
	void InitMapDataFilePath();


	/// <summary>
	/// �Z�����͈͊O���ǂ������m�F����
	/// </summary>
	/// <param name="cell">�Z��</param>
	/// <returns>�Z�����͈͊O���ǂ����̃t���O</returns>
	bool IsCellCheckOutOfRange(const Cell& cell);


	/// <summary>
	/// �}�b�v�`��
	/// </summary>
	void TestMapDraw();

private:



	// �I�u�W�F�N�g
	std::vector<std::shared_ptr<ObjectBase>>m_object;

	// �}�b�v���
	PlatinumLoader::MapInfo m_mapInfo;

	// �X�e�[�W�i���o�[
	int m_stageNumber;

	// �}�b�v�f�[�^�̃t�@�C���p�X
	std::vector<std::string> m_mapDataFilePath;

	// ���݂̃}�b�v�f�[�^
	std::vector<std::vector<int>> m_currentMapData;



	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// �v���`�i�����[�_�[
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

};