#include "PlatinumLoader.h"
#include <string>
#include <cassert>

PlatinumLoader::PlatinumLoader():
	m_mapWidth(0),
	m_mapHeight(0),
	m_chipSize(0),
	m_layerCount(0)
{
	// 可変長配列の初期化
	m_mapData.clear();
}

PlatinumLoader::~PlatinumLoader()
{
}

void PlatinumLoader::Load(const TCHAR* filePath)
{
	//FMFヘッダー(Platinumのドキュメントに書いてある)
	struct Header {
		int8_t id[4];			//識別子(FMF_)			1*4バイト
		uint32_t size;			//データサイズ　		4バイト
		uint32_t mapWidth;		//マップの幅			4バイト
		uint32_t mapHeight;		//マップの高さ　		4バイト
		uint8_t chiphWidth;		//チップ(セル一個)の幅					1バイト
		uint8_t chipHeight;		//チップ(セル一個)の高さ				1バイト
		uint8_t layerCount;		//レイヤーの数							1バイト
		uint8_t bitCount;		//１セル当たりのビット数(÷8でバイト数)	1バイト
	};//20バイト

	Header header;
	int handle =  FileRead_open(filePath);
	FileRead_read(&header,sizeof(header), handle);

	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());

	if (strId != "FMF_") {
		
		// ファイル名をテキスト化
		std::string fileName = filePath;
	
		// エラーメッセージ
		std::string errorMsg = "\n///////// README /////////\n\n" +
								fileName +
								" <-This file isn't here.\n\n" +
								 "-END-";
			
		//ワイド文字列に変換
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);

	
		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}

	// レイヤー数を代入
	m_layerCount = header.layerCount;

	// マップの縦幅と横幅を代入
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;

	//レイヤー1個当たりのサイズを計算する
	//マップの幅＊マップの高さ*(チップ1個当たりのバイト数)
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	// チップサイズを代入(チップサイズは縦横同じサイズなのでWidthでもHeightでもどちらでもよい)
	m_chipSize = header.chiphWidth;


	m_mapData.resize(m_layerCount);
	for (auto& layer : m_mapData) {
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);
}

const MapData_t& PlatinumLoader::GetMapData() const
{
	return m_mapData;
}

void PlatinumLoader::LayerCheck(int layerNum)
{
	// レイヤーが存在しない場合エラーメッセージを出す
	if (m_layerCount == layerNum) {


		// エラーメッセージ
		std::string errorMsg = "\n///////// README /////////\n\n"  
								" This fmf file does not have that layer..\n\n" 
								"-END-";

		//ワイド文字列に変換
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);


		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

	}
}

const int PlatinumLoader::GetChipSpriteNo(int layerNum, int chipX, int chipY) const
{
	assert(chipX < m_mapWidth);
	assert(chipY < m_mapHeight);


	auto index = chipX + chipY * m_mapWidth;

	return m_mapData[layerNum][index];
}

void PlatinumLoader::GetMapSize(int& width, int& height)
{
	width = m_mapWidth;
	height = m_mapHeight;
}

const int PlatinumLoader::GetChipSize()
{
	return m_chipSize;
}
