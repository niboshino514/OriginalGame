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
	PlatinumLoader();
	virtual ~PlatinumLoader();


	/// <summary>
	/// fmfファイルを読み込んでmapData_に中身を入れる
	/// </summary>
	/// <param name="filePath">fmfファイルのファイルパス</param>
	void Load(const TCHAR* filePath);
	
	/// <summary>
	/// mapData_の中身を直接取得する
	/// </summary>
	const MapData_t& GetMapData()const;
	
	/// <summary>
	/// そのレイヤーが存在するのか確認
	/// </summary>
	/// <param name="layerNum"></param>
	void LayerCheck(int layerNum);

	/// <summary>
	/// Loadで読み込んだMapData_tの中身（マップチップのデータ）を個別に取得する
	/// </summary>
	/// <param name="layerType">取得するマップチップのレイヤーID</param>
	/// <param name="chipX">取得するマップチップのX座標</param>
	/// <param name="chipY">取得するマップチップのY座標</param>
	/// <returns>引数で指定した場所のマップチップのスプライト番号</returns>
	const int GetChipSpriteNo(int layerNum,int chipX,int chipY)const;
	
	/// <summary>
	/// マップの縦幅と横幅を取得
	/// </summary>
	/// <param name="width">マップ横幅</param>
	/// <param name="height">マップ縦幅</param>
	void GetMapSize(int& width,int& height);

	/// <summary>
	/// チップサイズを返す
	/// </summary>
	/// <returns>チップサイズを返す</returns>
	const int GetChipSize();

private:

	// マップデータ
	MapData_t m_mapData;

	// マップの横幅
	int m_mapWidth = 0;
	// マップの縦幅
	int m_mapHeight = 0;

	// チップサイズ
	int m_chipSize = 0;

	// レイヤー数
	int m_layerCount = 0;
};

