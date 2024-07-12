#include "NoramalMapChip.h"

NoramalMapChip::NoramalMapChip()
{
}

NoramalMapChip::~NoramalMapChip()
{
}

void NoramalMapChip::Init()
{
	// オブジェクトID設定
	m_objectID = ObjectID::NoramalMapChip;
}

void NoramalMapChip::Update()
{
}

void NoramalMapChip::Draw()
{
	// セル描画
	DrawQuadrangleAA
	(m_square.A.x, m_square.A.y, m_square.B.x, m_square.B.y,
	 m_square.C.x, m_square.C.y, m_square.D.x, m_square.D.y, 0x00ff00, false);
}