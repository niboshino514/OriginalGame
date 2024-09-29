#pragma once
#include "EvoLib.h"
#include "Vec2.h"
#include <tuple>

// �V���O���g��
class GameData
{
private:
	GameData() = default;
	virtual ~GameData() = default;

	static GameData* m_pInstance;

public:

	// �R�s�[�C���X�g���N�^�̋֎~
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;
	GameData(GameData&&) = delete;
	GameData& operator=(GameData&&) = delete;

	static GameData* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new GameData();
		}

		return m_pInstance;
	}

	static void DeleteInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:

	// �W�����v�^�C�v
	enum class JumpType
	{
		Infinite,	// �����W�����v
		Second,	// 2�i�W�����v
	};

	// �W�����v��
	enum class JumpPower
	{
		// �ʏ�
		Normal,
		// ��
		Strong,
		// ��
		Weak,
	};

	// �ړ����x
	enum class MoveSpeed
	{
		Normal,// �ʏ�
		Slow,// �x��
		Fast,// ����
	};

	// �v���C���[�̃X�e�[�^�X
	struct PlayerStatus
	{
		// �d�͕���
		Direction gravityDirection = Direction::Bottom;

		// �W�����v�^�C�v
		JumpType jumpType = JumpType::Second;

		// �W�����v��
		JumpPower jumpPower = JumpPower::Normal;

		// �����t���O
		MoveSpeed moveSpeed = MoveSpeed::Normal;

		// ���S��
		int deathCount = 0;
	};


	// �Z�[�u�|�C���g�f�[�^
	struct SaveData
	{
		// ��������Ԃ̃f�[�^���ǂ���
		bool isInit = true;

		// ����
		Time time = Time();

		// �X�e�[�W�i���o�[
		int stageNumber = 0;

		// �Z�[�u�����Z��
		Cell cell = Cell();

		// �v���C���[�X�e�[�^�X
		PlayerStatus playerStatus = PlayerStatus();
	}; 


	// �V���b�g�̎��
	enum class ShotType
	{
		// �v���C���[�V���b�g
		PlayerShot,
		// �G�l�~�[�V���b�g
		EnemyShot,
		// �T�C���J�[�u�V���b�g
		SineCurveShot,
		// ���˃V���b�g
		ReflectionShot
	};

	// ���˃f�[�^
	struct Reflect
	{
		// ���ˍő吔
		int maxCount = 2;

		// ���ˉ�
		int count = 0;
	};

	// �V���b�g�f�[�^
	struct ShotData
	{
		// �V���b�g�̎��
		ShotType type = ShotType::PlayerShot;

		// ���W
		Vec2 startPos = Vec2(0.0f, 0.0f);

		// �V���b�g�A���O��
		float angle = 0.0f;

		// �V���b�g�X�s�[�h
		float speed = 0.0f;

		// ���˃f�[�^
		Reflect reflect = Reflect();
	};


public:


	/// <summary>
	/// �Z�[�u�f�[�^�̓ǂݍ���
	/// </summary>
	void LoadSaveData();

	/// <summary>
	/// �Z�[�u�f�[�^�̏�������
	/// </summary>
	void WriteSaveData();

	/// <summary>
	/// �Z�[�u�f�[�^������������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>�Z�[�u�f�[�^�̏��������</returns>
	bool IsSaveDataInit() { return m_saveData.isInit; }

	/// <summary>
	/// �Z�[�u�f�[�^��ݒ�
	/// </summary>
	/// <param name="savePointData">�Z�[�u�|�C���g�f�[�^</param>
	void SetSaveData(const SaveData& savePointData) { m_saveData = savePointData; }

	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^��Ԃ�
	/// </summary>
	/// <returns>�Z�[�u�|�C���g�f�[�^</returns>
	SaveData GetSaveData() { return m_saveData; }

	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^��ݒ�
	/// </summary>
	/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
	/// <param name="cell">�Z��</param>
	/// <param name="playerStatus">�v���C���[�X�e�[�^�X</param>
	void SetSavePointData(const int& stageNumber = 0, const Cell& cell = Cell(), const PlayerStatus& playerStatus = PlayerStatus());

	/// <summary>
	/// �v���C���[���W�ݒ�
	/// </summary>
	/// <param name="playerPos">�v���C���[���W</param>
	void SetPlayerPos(const Vec2& playerPos) { m_playerPos = playerPos; }

	/// <summary>
	/// �v���C���[���W��Ԃ�
	/// </summary>
	/// <returns>�v���C���[���W</returns>
	Vec2 GetPlayerPos() { return m_playerPos; }

	/// <summary>
	/// �{�X�G�l�~�[�X�|�[�����W��ݒ�
	/// </summary>
	/// <param name="bossEnemySpawnPos">�{�X�G�l�~�[�X�|�[�����W</param>
	void SetBossEnemySpawnPos(const Vec2& bossEnemySpawnPos) { m_bossEnemySpawnPos = bossEnemySpawnPos; }

	/// <summary>
	/// �{�X�G�l�~�[�X�|�[�����W��Ԃ�
	/// </summary>
	/// <returns>�{�X�G�l�~�[�X�|�[�����W</returns>
	Vec2 GetBossEnemySpawnPos() { return m_bossEnemySpawnPos; }



	/// <summary>
	/// �J�������W��ݒ�
	/// </summary>
	/// <param name="cameraPos">�J�������W</param>
	void SetCameraPos(const Vec2& cameraPos) { m_cameraPos = cameraPos; }

	/// <summary>
	/// �J�������W��Ԃ�
	/// </summary>
	/// <returns>�J�������W</returns>
	Vec2 GetCameraPos() { return m_cameraPos; }


	/// <summary>
	/// �v���C���[�X�e�[�^�X��Ԃ�
	/// </summary>
	/// <returns>�v���C���[�X�e�[�^�X�̎擾</returns>
	PlayerStatus GetPlayerStatus() { return m_saveData.playerStatus; }

	/// <summary>
	/// �v���C���[�X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="playerStatus">�v���C���[�̃X�e�[�^�X��ݒ�</param>
	void SetPlayerStatus(const PlayerStatus& playerStatus) { m_saveData.playerStatus = playerStatus; }

	/// <summary>
	/// �v���C���[�������Ă��邩�ǂ�����ݒ�
	/// </summary>
	/// <param name="isPlayerAlive">�v���C���[�������Ă��邩�ǂ���</param>
	void SetIsPlayerAlive(const bool& isPlayerAlive) { m_isPlayerAlive = isPlayerAlive; }

	/// <summary>
	/// �v���C���[�������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>�v���C���[�������Ă��邩�ǂ���</returns>
	bool GetIsPlayerAlive() { return m_isPlayerAlive; }

	/// <summary>
	/// ���S�񐔂�ݒ�
	/// </summary>
	/// <param name="deathCount">���S��</param>
	void SetDeathCount(const int& deathCount) { m_saveData.playerStatus.deathCount = deathCount; }

	/// <summary>
	/// ���Ԍv��
	/// </summary>
	void TimeCount();

	// 60�J�E���g�������ǂ���
	bool IsTimeCount60(const int& count);

	/// <summary>
	/// ���Ԃ�Ԃ�
	/// </summary>
	/// <returns>����</returns>
	Time GetTime() { return m_saveData.time; }

private:

	// �Z�[�u�|�C���g�f�[�^
	SaveData m_saveData = SaveData();

	// �v���C���[���W
	Vec2 m_playerPos = Vec2();

	// �{�X�G�l�~�[�X�|�[�����W
	Vec2 m_bossEnemySpawnPos = Vec2();

	// �J�������W
	Vec2 m_cameraPos = Vec2();


	// �v���C���[�������Ă��邩�ǂ���
	bool m_isPlayerAlive = true;
};