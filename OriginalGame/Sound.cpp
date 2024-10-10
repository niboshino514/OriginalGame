#include "Sound.h"
#include "EvoLib.h"
#include <sstream>
Sound* Sound::m_pInstance = nullptr;

namespace 
{
	// サウンドのファイルパス
	const std::string  kDataFilePaht = "Data/Sound/";

	// サウンドデータのファイルパス
	const std::string kSoundDataFilePath = "Data/Csv/Sound.csv";

	// サウンドの最大値
	const int kSoundMaxValue = 255;

	// サウンド音量設定データファイルパス
	const std::string kSoundVolumeSettingFilePath = "Data/Setting/SoundVolumeSetting.dat";


}

void Sound::LoadSoundVolumeSetting()
{
	// ファイルが存在するかどうかを確認し、存在しない場合は新規作成
	if (!EvoLib::File::IsFileExist(kSoundVolumeSettingFilePath))
	{
		// サウンド音量設定を書き込み
		EvoLib::File::WriteBinaryFile<SoundVolume>(kSoundVolumeSettingFilePath, m_soundPercentVolume);

		return;
	}

	// datファイルの読み込み
	EvoLib::File::ReadBinaryFile<SoundVolume>(kSoundVolumeSettingFilePath, m_soundPercentVolume);
}

void Sound::WriteSoundVolumeSetting()
{
	// サウンド音量設定を書き込み
	EvoLib::File::WriteBinaryFile<SoundVolume>(kSoundVolumeSettingFilePath, m_soundPercentVolume);
}

void Sound::Init()
{


	// サウンドデータの読み込み
	const std::vector<std::vector<std::string>> loadData = 
		EvoLib::File::CsvFileLoading(kSoundDataFilePath, EvoLib::File::LoadType::SkipFirstLine);
	

	// デフォルトデータ
	SoundData defoultData;

	// デフォルト拡張子名
	std::string defoultExtension;;


	// サウンドデータのデフォルト読み込み
	{
		// ファイル名
		const std::string fileName = loadData[0][0];

		// 拡張子名
		defoultExtension = loadData[0][1];


			// ファイル名と拡張子を結合
		defoultData.filePath = kDataFilePaht + fileName + defoultExtension;
		

		// サウンドタイプ
		if(loadData[0][2] == "BGM")
		{
			defoultData.soundType = SoundType::BGM;
		}
		else
		{
			defoultData.soundType = SoundType::SE;
		}

		// ボリューム調整
		defoultData.volRate = std::stof(loadData[0][3]);

		// 最大ボリューム
		defoultData.maxVolume = EvoLib::Convert::ConvertValueToPercent(kSoundMaxValue, defoultData.volRate);

		// 最大ボリュームをボリュームに代入
		defoultData.soundVolume = defoultData.maxVolume;

		defoultData.settingSoundVolume = defoultData.soundVolume;

		// ハンドルの初期化
		defoultData.handle = -1;
	}



	// サウンドデータの読み込み
	{
		// サウンドデータのサイズ
		const int soundDataSize = static_cast<int>(loadData.size());


		for (int i = 1; i < soundDataSize; i++)
		{
			// 一時保存データ
			SoundData tempData;


			
			// ファイル名
			const std::string fileName = loadData[i][0];


			// 拡張子名
			std::string extension = loadData[i][1];

			// 拡張子名が空の場合、デフォルトの拡張子名を代入
			if (IsEmpty(loadData[i][1]))
			{
				extension = defoultExtension;
			}


			// ファイル名と拡張子を結合
			tempData.filePath = kDataFilePaht + fileName + extension;
			
			// サウンドタイプ
			if(!IsEmpty(loadData[i][2]))
			{
				if (loadData[i][2] == "BGM")
				{
					tempData.soundType = SoundType::BGM;
				}
				else
				{
					tempData.soundType = SoundType::SE;
				}
			}
			else
			{
				// デフォルトのサウンドタイプを代入
				tempData.soundType = defoultData.soundType;
			}
			


			// ボリューム調整
			if (loadData[i].size() != 4)
			{
				// デフォルトのボリューム調整を代入
				tempData.volRate = defoultData.volRate;

			}
			else
			{
				if (!IsEmpty(loadData[i][3]))
				{
					tempData.volRate = std::stof(loadData[i][3]);
				}
				else
				{
					// デフォルトのボリューム調整を代入
					tempData.volRate = defoultData.volRate;
				}
			}

	
			
		

			// 最大ボリューム
			tempData.maxVolume = EvoLib::Convert::ConvertValueToPercent(kSoundMaxValue, tempData.volRate);

			// 最大ボリュームをボリュームに代入
			tempData.soundVolume = tempData.maxVolume;


			// ボリュームを設定ボリュームに代入
			if (tempData.soundType == SoundType::SE)
			{
				tempData.settingSoundVolume = EvoLib::Convert::ConvertFromPercentToValue(tempData.soundVolume, static_cast<float>(m_soundPercentVolume.se));
			}
			else
			{
				tempData.settingSoundVolume = EvoLib::Convert::ConvertFromPercentToValue(tempData.soundVolume, static_cast<float>(m_soundPercentVolume.bgm));
			}

			// ハンドルの初期化
			tempData.handle = -1;


			// サウンドデータの追加
			m_soundData[fileName] = tempData;
		}

	}
}

bool Sound::IsEmpty(const std::string& str)
{
	if(str == "")
	{
		return true;
	}

	return false;
}

void Sound::Load(const std::vector<std::string>& fileName, const bool& ASyncLoad)
{
	// 非同期処理を行うかどうか
	if (ASyncLoad)
	{
		// リソースの読み込み
		SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい
	}

	// サウンドの読み込み
	for (auto& file : fileName)
	{

		if(!EvoLib::File::IsFileExist(m_soundData[file].filePath))
		{
			EvoLib::Assert::ErrorMessageBox(m_soundData[file].filePath + "が存在しないようです。");
		}


		// ハンドルを代入する
		m_soundData[file].handle = LoadSoundMem(m_soundData[file].filePath.c_str());

		// 音量を代入する
		m_soundData[file].soundVolume = m_soundData[file].settingSoundVolume;

		// 音量設定
		ChangeVolumeSoundMem(m_soundData[file].soundVolume, m_soundData[file].handle);
	}

	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void Sound::UnLoad()
{
	// すべてのハンドルを削除
	InitSoundMem();
}

void Sound::Play(const std::string& filename)
{
	// サウンドのプレイタイプ
	int playType = 0;

	if (m_soundData[filename].soundType == SoundType::BGM)
	{
		playType = DX_PLAYTYPE_LOOP;
	}
	else
	{
		playType = DX_PLAYTYPE_BACK;
	}

	// 音量設定
	ChangeVolumeSoundMem(m_soundData[filename].soundVolume, m_soundData[filename].handle);

	// サウンドを鳴らす
	PlaySoundMem(m_soundData[filename].handle, playType);
}

void Sound::ScreenFadeBGMStop(const int& fadeValue)
{
	// フェードの最大値
	const int fadeMaxValue = 255;

	// フェード値の計算
	const int fadeValueReverse = fadeMaxValue - fadeValue;

	// フェード値を百分率で計算
	const int fadePercent = EvoLib::Convert::PercentageRate(static_cast<float>(fadeMaxValue), static_cast<float>(fadeValueReverse));

	// サウンドの読み込み
	for (auto& data : m_soundData)
	{
		// BGMでなかった場合、コンティニューする
		if(data.second.soundType != SoundType::BGM)
		{
			continue;
		}

		// サウンドボリュームを求める
		data.second.soundVolume = EvoLib::Convert::ConvertFromPercentToValue(data.second.settingSoundVolume, static_cast<float>(fadePercent));

		// 音量設定
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}

void Sound::SetSoundVolume(const SoundType& soundType, const int& soundPercentVolume)
{
	
	// サウンドの音量を代入する
	switch (soundType)
	{
	case Sound::SoundType::BGM:

		// サウンドの音量を代入する
		m_soundPercentVolume.bgm = soundPercentVolume;

		break;
	case Sound::SoundType::SE:

		// サウンドの音量を代入する
		m_soundPercentVolume.se = soundPercentVolume;

		break;
	default:
		// サウンドの音量を代入する
		m_soundPercentVolume.bgm = soundPercentVolume;
		break;
	}


	// サウンドの読み込み
	for (auto& data : m_soundData)
	{
		// サウンドタイプが異なる場合、continueする
		if (data.second.soundType != soundType)
		{
			continue;
		}

		// サウンド音量を調べる
		const int soundVolume = EvoLib::Convert::ConvertFromPercentToValue(data.second.maxVolume, static_cast<float>(soundPercentVolume));

		// 設定ボリュームに調べたサウンドボリュームを代入する
		data.second.settingSoundVolume = soundVolume;

		// ボリュームに設定ボリュームを代入する
		data.second.soundVolume = data.second.settingSoundVolume;

		// 音量設定
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}
