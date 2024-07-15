#include "MapLoader.h"
#include "PlatinumLoader.h"


MapLoader::MapLoader():
	m_currentData(),
	m_pPlatinumLoader(std::make_shared<PlatinumLoader>())
{
}

MapLoader::~MapLoader()
{
}

void MapLoader::Init()
{
}

void MapLoader::Update()
{
}

void MapLoader::Draw()
{
}

void MapLoader::LoadMap(const TCHAR* fmfFilePath, int layerType)
{
}
