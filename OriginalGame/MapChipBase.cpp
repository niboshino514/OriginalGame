#include "MapChipBase.h"

MapChipBase::MapChipBase()
{
}

MapChipBase::~MapChipBase()
{
}

void MapChipBase::Init()
{
}

void MapChipBase::Update()
{
}

void MapChipBase::Draw()
{
	// ƒZƒ‹•`‰æ
	DrawQuadrangleAA
	(m_square.A.x, m_square.A.y, m_square.B.x, m_square.B.y,
		m_square.C.x, m_square.C.y, m_square.D.x, m_square.D.y, m_color, false);
}
