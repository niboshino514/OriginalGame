#include "GameData.h"

// シングルトン
GameData* GameData::m_pInstance = nullptr;

namespace
{
	// セーブデータファイル名
	static const std::string kSaveDataFileName = "Data/Save/SaveData.dat";
}

void GameData::LoadSaveData()
{
	// ファイルが存在するかどうかを確認し、存在しない場合は新規作成
	if (!EvoLib::File::IsFileExist(kSaveDataFileName))
	{
		// セーブデータを書き込み
		EvoLib::File::WriteBinaryFile<SaveData>(kSaveDataFileName, SaveData());

		return;
	}
	
	// datファイルの読み込み
	EvoLib::File::ReadBinaryFile<SaveData>(kSaveDataFileName, m_saveData);
}

void GameData::WriteSaveData()
{
	// セーブデータを書き込み
	EvoLib::File::WriteBinaryFile<SaveData>(kSaveDataFileName, m_saveData);
}

void GameData::SetSavePointData(const int& stageNumber, const Cell& cell, const PlayerStatus& playerStatus)
{
	// セーブポイントデータを設定
	m_saveData.stageNumber = stageNumber;
	m_saveData.cell = cell;
	m_saveData.playerStatus = playerStatus;
}

void GameData::TimeCount()
{
	// ミリ秒
	{
		// ミリ秒を加算
		m_saveData.time.millisecond++;

		// 60カウント以上ならば秒を加算し、ミリ秒をリセット
		if (IsTimeCount60(m_saveData.time.millisecond))
		{
			// 秒を加算
			m_saveData.time.second++;

			// ミリ秒をリセット
			m_saveData.time.millisecond = 0;
		}
	}

	// 秒
	{
		// 60カウント以上ならば分を加算し、秒をリセット
		if (IsTimeCount60(m_saveData.time.second))
		{
			// 分を加算
			m_saveData.time.minute++;

			// 秒をリセット
			m_saveData.time.second = 0;
		}
	}

	// 分
	{
		// 60カウント以上ならば時間を加算し、分をリセット
		if (IsTimeCount60(m_saveData.time.minute))
		{
			// 時間を加算
			m_saveData.time.hour++;

			// 分をリセット
			m_saveData.time.minute = 0;
		}
	}

}

bool GameData::IsTimeCount60(const int& count)
{
	// 60カウント以上ならば、trueを返す
	if (count >= 60)
	{
		return true;
	}

	return false;
}