#pragma once
#include "FunctionConclusion.h"
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



	// �Z�[�u�|�C���g�f�[�^
	struct SavePointData
	{
		// �X�e�[�W�i���o�[
		int stageNumber = 0;

		// �Z�[�u�����Z��
		Cell cell = Cell();
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
	/// �J�������W��ݒ�
	/// </summary>
	/// <param name="cameraPos">�J�������W</param>
	void SetCameraPos(const Vec2& cameraPos) { m_cameraPos = cameraPos; }

	/// <summary>
	/// �J�������W��Ԃ�
	/// </summary>
	/// <returns>�J�������W</returns>
	Vec2 GetCameraPos() { return m_cameraPos; }

private:

	// �Z�[�u�|�C���g�f�[�^
	SavePointData m_savePointData = SavePointData();

	// �v���C���[���W
	Vec2 m_playerPos = Vec2();

	// �J�������W
	Vec2 m_cameraPos = Vec2();
};