#include "SaveChip.h"

SaveChip::SaveChip()
{
}

SaveChip::~SaveChip()
{
}

void SaveChip::Init()
{
	// オブジェクトIDを設定
	m_objectID = ObjectID::SaveChip;
}

void SaveChip::Update()
{
	m_isGimmick = false;

	// 衝突処理
	Collision();
}

void SaveChip::Draw()
{
	int number = 0;

	if(m_isGimmick)
	{
		number = 1;
	}



	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// 左上座標
	Vec2 pos = m_square.A + offset;


	// 画像の描画
	DrawGraph(pos.x, pos.y, m_graphicHandle[number], TRUE);


}