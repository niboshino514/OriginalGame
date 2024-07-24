#pragma once
#include <DxLib.h>
#include <vector>


//マップの広さ * レイヤー数なので、2つのベクターを入れ子にしてる
using MapData_t = std::vector < std::vector<unsigned char>>;


/// <summary>
/// Platinumのデータをロードする
/// </summary>
class PlatinumLoader
{
public:
	// マップ情報
	struct MapInfo
	{
		// マップの横幅
		int mapWidth = 0;
		// マップの縦幅
		int mapHeight = 0;
		// チップサイズ
		float chipSize = 0;
	};

	// マップデータ
	struct MapData
	{
		// マップデータ
		std::vector<std::vector<int>> mapData;
	};

public:
	PlatinumLoader();
	virtual ~PlatinumLoader();

public:

	/// <summary>
	/// fmfファイルを読み込んでmapData_に中身を入れる
	/// </summary>
	/// <param name="filePath">fmfファイルのファイルパス</param>
	void Load(const TCHAR* filePath);
	

	/// <summary>
	/// レイヤーごとのマップデータを取得する
	/// </summary>
	/// <param name="layerNum">レイヤー番号</param>
	/// <returns>マップデータを返す</returns>
	std::vector<std::vector<int>> GetMapLayerData(const int& layerNum);

	/// <summary>
	/// すべてのマップデータを取得する
	/// </summary>
	/// <returns></returns>
	std::vector<MapData> GetMapAllData();

	/// <summary>
	/// マップ情報を返す
	/// </summary>
	/// <returns>マップ情報(マップの横幅,　マップの縦幅,　マップチップのサイズ)</returns>
	MapInfo GetMapInfo() { return m_mapInfo; }



private:

	/// <summary>
	/// ファイルが存在するかどうかを確認(アサート)
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void FileExistsConfirmation(const TCHAR* filePath);


	/// <summary>
	/// そのレイヤーが存在するのか確認
	/// </summary>
	/// <param name="layerNum">レイヤー</param>
	void LayerCheck(const int& layerNum);


private:

	// プラチナムデータ
	MapData_t m_platinumData;

	// マップ情報
	MapInfo m_mapInfo;

	// レイヤー数
	int m_layerMaxNum = 0;
};

