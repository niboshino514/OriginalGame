#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	class File
	{

	public:

		// 読み取るタイプ
		enum class LoadType
		{
			Noramal,// 通常
			SkipFirstLine,	// 一行目をスキップする
			SkipOneColumn,	// 各一列目をスキップする
			DoubleSkip,		// 一行目と各一列目をスキップする
		};


	private:

		/// <summary>
		/// 文字列を分割して返す
		static std::vector<std::string> Split(const std::string& input, const char& delimiter);

		/// 文字列を分割して返す
		static std::vector<std::wstring> SplitWString(const std::wstring& input, const wchar_t& delimiter);
		
	public:

		/// <summary>
		/// ファイルが存在するかどうかを確認
		/// </summary>
		/// <param name="name">ファイル名</param>
		/// <returns>そのファイルが存在するかどうかを返す</returns>
		static bool IsFileExist(const std::string& name);

		/// <summary>
		/// CSVファイルを読み込む
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="isLoadFirstLine">最初の一行を読み込むかどうか</param>
		/// <param name="isLoadOneColumn">最初の一列を読み込むかどうか</param>
		/// <returns>読み込んだ文字列を返す</returns>
		static std::vector<std::vector<std::string>> CsvFileLoading(const std::string& filePath, LoadType loadType = LoadType::Noramal);

		/// <summary>
		/// CSVファイルを読み込む(改訂版)
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="isSkipOneArray">最初の一列を飛ばすかどうか</param>
		/// <param name="skipColumnNum">各列何個まで飛ばすかの数</param>
		/// <param name="isDeleteEmptyCell">空のセルを削除するかどうか</param>
		/// <returns>読み込んだ文字列を返す</returns>
		static std::vector<std::vector<std::string>> CsvFileLoading_Revision(const std::string& filePath, const bool& isSkipOneArray = false, const int& skipColumnNum = 0, const bool& isDeleteEmptyCell = false);

		/// <summary>
		/// 簡易CSVファイル書き込み
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <param name="writingText">書き込むテキスト</param>
		static void SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText);

		/// <summary>
		/// バイナリファイルを書き込む(生成も可能)※テキストの場合、char[]型に変換してください
		/// </summary>
		/// <typeparam name="T">struct</typeparam>
		/// <param name="filename">書き込むファイル名</param>
		/// <param name="data">書き込むデータ</param>
		template<typename T>
		static void WriteBinaryFile(const std::string& filename, const T& data);

		/// <summary>
		/// バイナリファイルを読み込む※テキストの場合、char[]型でしか読み込む事ができません
		/// </summary>
		/// <typeparam name="T">struct</typeparam>
		/// <param name="filename">読み込むファイル名</param>
		/// <param name="data">読み込むデータ関数</param>
		template<typename T>
		static void ReadBinaryFile(const std::string& filename, T& data);
	};

	template<typename T>
	inline void File::WriteBinaryFile(const std::string& filename, const T& data)
	{
		std::ofstream outFile(filename, std::ios::binary);
		if (!outFile) {
			std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
			return;
		}
		outFile.write(reinterpret_cast<const char*>(&data), sizeof(T));
		outFile.close();
	}

	template<typename T>
	inline void File::ReadBinaryFile(const std::string& filename, T& data)
	{
		std::ifstream inFile(filename, std::ios::binary);
		if (!inFile) {
			std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
			return;
		}
		inFile.read(reinterpret_cast<char*>(&data), sizeof(T));
		inFile.close();
	}

}