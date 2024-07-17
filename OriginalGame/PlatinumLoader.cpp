#include "PlatinumLoader.h"
#include <string>
#include <cassert>
#include <filesystem>

PlatinumLoader::PlatinumLoader() :
	m_platinumData(),
	m_mapInfo(),
	m_layerMaxNum(0)
{
}

PlatinumLoader::~PlatinumLoader()
{
}

void PlatinumLoader::Load(const TCHAR* filePath)
{
	// ファイルが存在するかどうかを確認する
	FileExistsConfirmation(filePath);


	// FMFヘッダー(Platinumのドキュメントに書いてある)
	struct Header {
		int8_t id[4];			// 識別子(FMF_)								1*4バイト
		uint32_t size;			// データサイズ　							4バイト
		uint32_t mapWidth;		// マップの幅								4バイト
		uint32_t mapHeight;		// マップの高さ　							4バイト
		uint8_t chiphWidth;		// チップ(セル一個)の幅						1バイト
		uint8_t chpHeight;		// チップ(セル一個)の高さ					1バイト
		uint8_t layerCount;		// レイヤーの数								1バイト
		uint8_t bitCount;		// １セル当たりのビット数(÷8でバイト数)	1バイト
	};// 20バイト

	// ヘッダー変数
	Header header;

	const int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);

	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());



	// レイヤー数を代入
	m_layerMaxNum = header.layerCount;

	// マップの縦幅と横幅を代入
	m_mapInfo.mapHeight = header.mapHeight;
	m_mapInfo.mapWidth = header.mapWidth;

	//レイヤー1個当たりのサイズを計算する
	//マップの幅＊マップの高さ*(チップ1個当たりのバイト数)
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	// チップサイズを代入(チップサイズは縦横同じサイズなのでWidthでもHeightでもどちらでもよい)
	m_mapInfo.chipSize = header.chiphWidth;


	m_platinumData.resize(m_layerMaxNum);
	for (auto& layer : m_platinumData) {
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);
}



std::vector<std::vector<int>> PlatinumLoader::GetMapLayerData(const int& layerNum)
{
	// レイヤーが存在するかを確認する
	LayerCheck(layerNum);


	// 二次元の要素数を持った二次元配列
	std::vector<std::vector<int>>mapData(m_mapInfo.mapWidth, std::vector<int>(m_mapInfo.mapHeight));

	for (int y = 0; y < m_mapInfo.mapHeight; y++)
	{
		for (int x = 0; x < m_mapInfo.mapWidth; x++)
		{
			auto index = x + y * m_mapInfo.mapWidth;

			mapData[x][y] = m_platinumData[layerNum][index];
		}
	}

	// マップデータを返す
	return mapData;
}

std::vector<PlatinumLoader::MapData> PlatinumLoader::GetMapAllData()
{
	// マップデータ格納変数
	std::vector<MapData> mapData;

	// すべてのレイヤーのデータを格納する
	for (int i = 0; i < m_layerMaxNum; i++)
	{
		mapData.push_back(MapData(GetMapLayerData(i)));
	}

	// マップデータを返す
	return mapData;
}

void PlatinumLoader::FileExistsConfirmation(const TCHAR* filePath)
{
#if _DEBUG

	if (!std::filesystem::is_regular_file(filePath))
	{

		// ファイル名をテキスト化
		std::string fileName = filePath;

		// エラーメッセージ
		std::string errorMsg =
			"\n///////// README /////////\n\n" +
			fileName + " <-This file isn't here.\n\n" +
			"-END-";

		// ワイド文字列に変換
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);


		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}
#endif
}

void PlatinumLoader::LayerCheck(const int& layerNum)
{
#if _DEBUG

	// レイヤーが存在しない場合エラーメッセージを出す
	if (m_layerMaxNum == layerNum)
	{
		// エラーメッセージ
		std::string errorMsg =
			"\n///////// README /////////\n\n"
			" This fmf file does not have that layer..\n\n"
			"-END-";

		//ワイド文字列に変換
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);


		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}
#endif
}