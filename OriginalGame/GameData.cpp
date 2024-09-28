#include "GameData.h"

// �V���O���g��
GameData* GameData::m_pInstance = nullptr;

namespace
{
	// �Z�[�u�f�[�^�t�@�C����
	static const std::string kSaveDataFileName = "Data/Save/SaveData.dat";
}

void GameData::LoadSaveData()
{
	// �t�@�C�������݂��邩�ǂ������m�F���A���݂��Ȃ��ꍇ�͐V�K�쐬
	if (!EvoLib::File::IsFileExist(kSaveDataFileName))
	{
		// �Z�[�u�f�[�^����������
		EvoLib::File::WriteBinaryFile<SaveData>(kSaveDataFileName, SaveData());

		return;
	}
	
	// dat�t�@�C���̓ǂݍ���
	EvoLib::File::ReadBinaryFile<SaveData>(kSaveDataFileName, m_saveData);
}

void GameData::WriteSaveData()
{
	// �Z�[�u�f�[�^����������
	EvoLib::File::WriteBinaryFile<SaveData>(kSaveDataFileName, m_saveData);
}

void GameData::SetSavePointData(const int& stageNumber, const Cell& cell, const PlayerStatus& playerStatus)
{
	// �Z�[�u�|�C���g�f�[�^��ݒ�
	m_saveData.stageNumber = stageNumber;
	m_saveData.cell = cell;
	m_saveData.playerStatus = playerStatus;
}

void GameData::TimeCount()
{
	// �~���b
	{
		// �~���b�����Z
		m_saveData.time.millisecond++;

		// 60�J�E���g�ȏ�Ȃ�Εb�����Z���A�~���b�����Z�b�g
		if (IsTimeCount60(m_saveData.time.millisecond))
		{
			// �b�����Z
			m_saveData.time.second++;

			// �~���b�����Z�b�g
			m_saveData.time.millisecond = 0;
		}
	}

	// �b
	{
		// 60�J�E���g�ȏ�Ȃ�Ε������Z���A�b�����Z�b�g
		if (IsTimeCount60(m_saveData.time.second))
		{
			// �������Z
			m_saveData.time.minute++;

			// �b�����Z�b�g
			m_saveData.time.second = 0;
		}
	}

	// ��
	{
		// 60�J�E���g�ȏ�Ȃ�Ύ��Ԃ����Z���A�������Z�b�g
		if (IsTimeCount60(m_saveData.time.minute))
		{
			// ���Ԃ����Z
			m_saveData.time.hour++;

			// �������Z�b�g
			m_saveData.time.minute = 0;
		}
	}

}

bool GameData::IsTimeCount60(const int& count)
{
	// 60�J�E���g�ȏ�Ȃ�΁Atrue��Ԃ�
	if (count >= 60)
	{
		return true;
	}

	return false;
}