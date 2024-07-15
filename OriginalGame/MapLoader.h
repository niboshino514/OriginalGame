#pragma once
#include <vector>
#include <memory>


class PlatinumLoader;

class MapLoader
{
public:
	MapLoader();
	virtual ~MapLoader();

	void Init();
	void Update();
	void Draw();


	/// <summary>
	/// マップをロードする
	/// </summary>
	/// <param name="fmfFilePath">マップのFMFデータ</param>
	/// <param name="layerType">レイヤータイプ</param>
	void LoadMap(const TCHAR* fmfFilePath, int layerType);


private:
	// チップ番号の保存
	std::vector<std::vector<int>> m_currentData;

	// クラスポインタ
	std::shared_ptr<PlatinumLoader>m_pPlatinumLoader;
};