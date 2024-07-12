#include "NoneMapChip.h"

NoneMapChip::NoneMapChip()
{
}

NoneMapChip::~NoneMapChip()
{
}

void NoneMapChip::Init()
{
}

void NoneMapChip::Update()
{
}

void NoneMapChip::Draw()
{
	// ƒZƒ‹•`‰æ
	DrawQuadrangleAA
	(m_square.A.x, m_square.A.y, m_square.B.x, m_square.B.y,
		m_square.C.x, m_square.C.y, m_square.D.x, m_square.D.y, 0x808080, false);
}
