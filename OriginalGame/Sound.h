#pragma once
#include <string>
#include <vector>
#include <string>
#include <map>

// シングルトンを使用したサウンドクラス
class Sound
{

private:
	Sound() = default;
	virtual ~Sound() = default;

	static Sound* m_pInstance;

public:
	// コピーコンストラクタの禁止
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(Sound&&) = delete;

	static Sound* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new Sound();
		}
		return m_pInstance;
	}
	static void DeleteInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:

	// サウンドタイプ情報
	enum class SoundType
	{
		BGM,// BGM
		SE,	// SE
	};

private:

	// ファイル
	enum class File
	{
		FileName,		// ファイル名
		FileExtension,	// 拡張子
		SoundType,		// サウンドのタイプ
		SoundVolume,	// サウンドの音量
	};

	// サウンドデータ
	struct SoundData
	{
		SoundType soundType = SoundType();	// サウンドタイプ
		float volRate = 0.0f;				// ボリューム調整
		int maxVolume = 0;					// 最大ボリューム
		int soundVolume = 0;				// サウンドボリューム
		int settingSoundVolume = 0;			// 設定したサウンドボリューム
		int handle = -1;					// サウンドハンドル
		std::string filePath;				// ファイルパス
	};

public:
	// 初期処理
	void Init();

	// stingに何も書かれていないかどうかの判定
	bool IsEmpty(const std::string& str);


	/// <summary>
	/// サウンドロード
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="ASyncLoad">非同期処理を行うかどうか</param>
	void Load(const std::vector<std::string>& fileName, const bool& ASyncLoad = false);

	/// <summary>
	/// サウンドアンロード
	/// </summary>
	void UnLoad();

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void Play(const std::string& filename);

	/// <summary>
	/// 画面のフェードに合わせてBGMを消す
	/// </summary>
	/// <param name="fadeValue">これを書く→FadeBrightValue()</param>
	void ScreenFadeBGMStop(const int& fadeValue);

	/// <summary>
	/// サウンド音量設定
	/// </summary>
	/// <param name="soundType">サウンドタイプ</param>
	/// <param name="soundPercentVolume">サウンドパーセントボリューム</param>
	void SetSoundVolume(const SoundType& soundType, const int& soundPercentVolume);

private:
	// 音声データ
	std::map<std::string, SoundData> m_soundData;
};