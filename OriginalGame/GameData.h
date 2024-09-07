#pragma once
#include "EvoLib.h"

#include "Vec2.h"

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
	};


	// �Z�[�u�|�C���g�f�[�^
	struct SavePointData
	{
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
	};


	// �V���b�g�f�[�^
	struct ShotData
	{
		// �V���b�g�̎��
		ShotType type = ShotType::PlayerShot;

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);

		// �V���b�g�A���O��
		float angle = 0.0f;

		// �V���b�g�X�s�[�h
		float speed = 0.0f;
	};


public:


	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^��ݒ�
	/// </summary>
	/// <param name="savePointData">�Z�[�u�|�C���g�f�[�^</param>
	void SetSavePointData(const SavePointData& savePointData) { m_savePointData = savePointData; }

	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^��Ԃ�
	/// </summary>
	/// <returns>�Z�[�u�|�C���g�f�[�^</returns>
	SavePointData GetSavePointData() { return m_savePointData; }

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
	/// �{�X�G�l�~�[���W��ݒ�
	/// </summary>
	/// <param name="enemyBossPos">�{�X�G�l�~�[���W</param>
	void SetBossEnemyPos(const Vec2& bossEnemyPos) { m_bossEnemyPos = bossEnemyPos; }

	/// <summary>
	/// �{�X�G�l�~�[���W��Ԃ�
	/// </summary>
	/// <returns>�{�X�G�l�~�[���W</returns>
	Vec2 GetBossEnemyPos() { return m_bossEnemyPos; }

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
	PlayerStatus GetPlayerStatus() { return m_savePointData.playerStatus; }

	/// <summary>
	/// �v���C���[�X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="playerStatus">�v���C���[�̃X�e�[�^�X��ݒ�</param>
	void SetPlayerStatus(const PlayerStatus& playerStatus) { m_savePointData.playerStatus = playerStatus; }

private:

	// �Z�[�u�|�C���g�f�[�^
	SavePointData m_savePointData = SavePointData();

	// �v���C���[���W
	Vec2 m_playerPos = Vec2();

	// �{�X�G�l�~�[���W
	Vec2 m_bossEnemyPos = Vec2();

	// �{�X�G�l�~�[�X�|�[�����W
	Vec2 m_bossEnemySpawnPos = Vec2();


	// �J�������W
	Vec2 m_cameraPos = Vec2();

};