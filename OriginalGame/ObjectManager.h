#pragma once

#include "StateMachine.h"
#include <memory>
#include <vector>
#include <string>
#include "Vec2.h"
#include "PlatinumLoader.h"


#include "EvoLib.h"

#include <tuple>


template <class TState> class StateMachine;

class ObjectBase;
class PlatinumLoader;
class Camera;
class Pause;

class ObjectManager : public std::enable_shared_from_this<ObjectManager>
{
public:

	// �X�e�[�W�ړ��^�C�v
	enum class MapSwitchType
	{
		Spawn,			// �X�|�[��
		Respawn,		// ���X�|�[��
		NextStage,		// ���̃X�e�[�W
		PreviouseStage	// �O�̃X�e�[�W
	};


	// �`�b�v�^�C�v
	enum class ChipType
	{
		None,				// �����Ȃ�
		Ground,				// �n��
		NextStage,			// ���̃X�e�[�W
		PreviouseStage,		// �O�̃X�e�[�W
		NextPos,			// ���̃X�e�[�W���W
		PreviousePos,		// �O�̃X�e�[�W���W
		SpawnPos,			// �X�|�[�����W
		Save,				// �Z�[�u

		TopNeedle,			// ��j
		BottomNeedle,		// ���j
		LeftNeedle,			// ���j
		RightNeedle,		// �E�j
		DiedBlock,			// ���S�u���b�N
		
		TopGravity,			// ��d��
		BottomGravity,		// ���d��
		LeftGravity,		// ���d��
		RightGravity,		// �E�d��

		IceBlock,			// �A�C�X�u���b�N
	
		TopConveyor,		// ��R���x�A
		BottomConveyor,		// ���R���x�A
		LeftConveyor,		// ���R���x�A
		RigthConveyor,		// �E�R���x�A

		TransparentBlock,	// �����u���b�N
		
		NotExists			// ���݂��Ȃ�
	};



	// �}�b�v����f�[�^
	struct MapCollisionData
	{
		// �`�b�v�^�C�v
		ChipType chipType = ChipType::None;

		// �}�b�v�l�p�`���
		Square square = Square();

		// �}�b�v�~���
		Circle circle = Circle();

		// ��ʓ��t���O
		bool screenFlag = false;
	};

	// �}�b�v���f�[�^
	struct MapInfoData
	{
		// �}�b�v�f�[�^�̃t�@�C���p�X
		std::vector<std::string> filePath;

		// �}�b�v����f�[�^
		std::vector<std::vector<MapCollisionData>> mapCollisionData;

		// �}�b�v���
		PlatinumLoader::MapChip mapChip;
		
		// �}�b�v�i���o�[
		int mapNumber = 0;
	};


public:

	// �X�e�[�g
	enum class State
	{
		// �ݒ�
		Setting,
		// �ʏ�
		Normal,
		// �|�[�Y���
		Pause,
	};


public:
	ObjectManager();
	virtual ~ObjectManager();

	void Init();
	void Update();
	void Draw();

public:



	/// <summary>
	/// �X�e�[�g�ݒ�
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	void SetState(const State& state);




	/// <summary>
	/// �L�����N�^�[����
	/// </summary>
	void CharacterCreate(const Vec2& pos);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="�}�b�v���"></param>
	void MapCollisionDataCreate(const std::vector<std::vector<int>>& mapData, const MapSwitchType& mapSwitchType);

	/// <summary>
	/// �I�u�W�F�N�g�폜
	/// </summary>
	void ObjectErase();

	/// <summary>
	/// �X�e�[�W�ړ�
	/// </summary>
	void StageMove(const MapSwitchType& mapSwitchType);

	/// <summary>
	/// �}�b�v���f�[�^��Ԃ�
	/// </summary>
	/// <returns>�}�b�v���f�[�^</returns>
	MapInfoData GetMapInfoData() { return m_mapInfoData; }
	
	/// <summary>
	/// �}�b�v�`�b�v�ԍ���Ԃ�
	/// </summary>
	/// <returns></returns>
	std::vector<std::vector<int>>GetMapChipNumber();

	/// <summary>
	/// �Z�[�u�|�C���g��ݒ肷��
	/// </summary>
	/// <param name="pos">���W</param>
	void SetSavePoint(const Vec2& pos);

	/// <summary>
	/// �Z�[�u�|�C���g�̍��W��Ԃ�
	/// </summary>
	/// <returns>bool : �X�e�[�W��ύX���邩�ǂ����@Vec2 : ���W</returns>
	std::tuple<bool, Vec2> GetSavePointPos();

	/// <summary>
	/// �j�[�h������Ԃ�
	/// </summary>
	/// <param name="needleDirection">�j�[�h���̌���</param>
	/// <param name="square">�l�p�`���</param>
	/// <returns>�j�[�h�����</returns>
	Triangle ChipTypeToTriangle(const ChipType& needleDirection, const Square& square);

	/// <summary>
	/// �X�N���[�������ǂ����𒲂ׂ�
	/// </summary>
	void ScreenCheck();

	/// <summary>
	/// �}�b�v�����蔻��f�[�^��Ԃ�
	/// </summary>
	std::vector<std::vector<MapCollisionData>> GetMapCollisionData() { return m_mapInfoData.mapCollisionData; }

	/// <summary>
	/// �I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g���</returns>
	std::vector<std::shared_ptr<ObjectBase>> GetObjectInfo() { return m_object; }
	
private:

	/// <summary>
	/// �X�e�[�g������
	/// </summary>
	void StateInit();

	// �ݒ�X�e�[�g����
	void StateSettingInit();

	// �m�[�}���X�e�[�g����
	void StateNormalUpdate();
	void StateNormalDraw();

	// �|�[�Y�X�e�[�g����
	void StatePauseUpdate();
	void StatePauseDraw();


private:
	/// <summary>
	/// �}�b�v�֘A�����ݒ�
	/// </summary>
	void InitMap();
	
	/// <summary>
	/// �X�N���[���T�[�N��������
	/// </summary>
	void InitScreenCircle();

	/// <summary>
	/// �}�b�v�f�[�^�t�@�C���p�X�̏����ݒ�
	/// </summary>
	void InitMapDataFilePath();

	/// <summary>
	/// �}�b�v�`�b�v����
	/// </summary>
	/// <param name="mapCollisionData">�}�b�v����f�[�^</param>
	void MapChipCreate(const MapCollisionData& mapCollisionData);

	/// <summary>
	/// �M�~�b�N���Z�b�g����
	/// </summary>
	void GimmickReset();

	/// <summary>
	/// �Z�����͈͊O���ǂ������m�F����
	/// </summary>
	/// <param name="cell">�Z��</param>
	/// <returns>�Z�����͈͊O���ǂ����̃t���O</returns>
	bool IsCellCheckOutOfRange(const Cell& cell);


	/// <summary>
	/// �I�u�W�F�N�g�X�V
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// �I�u�W�F�N�g�`��
	/// </summary>
	void ObjectDraw();


	/// <summary>
	/// �}�b�v�`��
	/// </summary>
	void TestMapDraw();

private:

	// �I�u�W�F�N�g
	std::vector<std::shared_ptr<ObjectBase>>m_object;

	// �}�b�v���f�[�^
	MapInfoData m_mapInfoData;

	// �X�N���[���T�[�N��
	Circle m_screenCircle;

	// �}�b�v�O���t�B�b�N
	std::vector<int>m_testMapGraph;

	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g�}�V��
	StateMachine<State>m_pStateMachine;


	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// �v���`�i�����[�_�[
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;

	// �J����
	std::shared_ptr<Camera>m_pCamera;

	// �|�[�Y
	std::shared_ptr<Pause>m_pPause;
};