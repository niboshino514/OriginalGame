#include "GameData.h"

// シングルトン
GameData* GameData::m_pInstance = nullptr;

namespace
{
	// セーブデータファイル名
	static const std::string kSaveDataFileName = "Data/Save/SaveData.dat";

	// スコアデータファイル名
	static const std::string kScoreDataFileName = "Data/Save/ScoreData.dat";
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

void GameData::InitSaveData()
{
	// セーブデータの初期化
	m_saveData = SaveData();

}

void GameData::LoadScoreData()
{
	// ファイルが存在するかどうかを確認し、存在しない場合は新規作成
	if (!EvoLib::File::IsFileExist(kScoreDataFileName))
	{
		// スコアデータを書き込み
		EvoLib::File::WriteBinaryFile<ScoreData>(kScoreDataFileName, ScoreData());

		return;
	}

	// datファイルの読み込み
	EvoLib::File::ReadBinaryFile<ScoreData>(kScoreDataFileName, m_scoreData);
}

void GameData::WriteScoreData()
{
	// スコアデータを書き込み
	EvoLib::File::WriteBinaryFile<ScoreData>(kScoreDataFileName, m_scoreData);
}

void GameData::SetScoreData()
{
	// スコアデータを設定
	m_scoreData.isClear = true;
	m_scoreData.clearTime = m_saveData.time;
	m_scoreData.deathCount = m_saveData.playerStatus.deathCount;
}

void GameData::SetSavePointData(const int& stageNumber, const Cell& cell, const PlayerStatus& playerStatus)
{
	// セーブポイントデータを設定
	m_saveData.stageNumber = stageNumber;
	m_saveData.cell = cell;
	m_saveData.playerStatus = playerStatus;
}

bool GameData::IsNewRecord()
{
	// 初期化タイム
	Time initTime = Time();

	// スコアデータが初期値の場合、新記録とする
	if (m_scoreData.clearTime.hour == initTime.hour&&
		m_scoreData.clearTime.minute== initTime.minute&&
		m_scoreData.clearTime.second == initTime.second&&
		m_scoreData.clearTime.millisecond == initTime.millisecond)
	{
		return true;
	}
	
	bool isTime = false;

	// 時間の比較
	if (m_saveData.time.hour < m_scoreData.clearTime.hour ||
		(m_saveData.time.hour == m_scoreData.clearTime.hour && m_saveData.time.minute < m_scoreData.clearTime.minute) ||
		(m_saveData.time.hour == m_scoreData.clearTime.hour && m_saveData.time.minute == m_scoreData.clearTime.minute && m_saveData.time.second < m_scoreData.clearTime.second) ||
		(m_saveData.time.hour == m_scoreData.clearTime.hour && m_saveData.time.minute == m_scoreData.clearTime.minute && m_saveData.time.second == m_scoreData.clearTime.second && m_saveData.time.millisecond <= m_scoreData.clearTime.millisecond))
	{
		isTime = true;
	}




	bool isDeathCount = false;

	// セーブデータのデスカウントがスコアデータのデスカウントよりも少ない場合、新記録とする
	if(m_saveData.playerStatus.deathCount <= m_scoreData.deathCount)
	{
		isDeathCount = true;
	}


	if(isTime && isDeathCount)
	{
		return true;
	}


	return false;
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

			// 時間が99以上ならば99に設定
			if(m_saveData.time.hour >= 99)
			{
				m_saveData.time.hour = 99;
			}


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

GameData::Score GameData::CalcScore(const Time& time, const int& deathCount)
{
	// スコアデータ
	GameData::Score score = Score();

	// スコアタイム計算
	{
		
		// 小数点以下
		{
			score.time[7] = time.millisecond % 10;
			score.time[6] = (time.millisecond / 10) % 10;
		}
		
		// 秒
		{
			score.time[5] = time.second % 10;
			score.time[4] = (time.second / 10) % 10;
		}

		// 分
		{
			score.time[3] = time.minute % 10;
			score.time[2] = (time.minute / 10) % 10;
		}

		// 時間
		{
			score.time[1] = time.hour % 10;
			score.time[0] = (time.hour / 10) % 10;
		}
	}

	// スコアデスカウント計算
	{
		// 一の位
		{
			score.deathCount[3] = deathCount % 10;
		}

		// 十の位
		{
			score.deathCount[2] = (deathCount / 10) % 10;
		}

		// 百の位
		{
			score.deathCount[1] = (deathCount / 100) % 10;
		}

		// 千の位
		{
			score.deathCount[0] = (deathCount / 1000) % 10;
		}

	}


	// スコアを返す
	return score;
}

