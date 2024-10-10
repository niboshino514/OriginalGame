#include "SaveChip.h"

SaveChip::SaveChip()
{
}

SaveChip::~SaveChip()
{
}

void SaveChip::Init()
{
	// �I�u�W�F�N�gID��ݒ�
	m_objectID = ObjectID::SaveChip;
}

void SaveChip::Update()
{
	m_isGimmick = false;

	// �Փˏ���
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

	// ������W
	Vec2 pos = m_square.A + offset;


	// �摜�̕`��
	DrawGraph(pos.x, pos.y, m_graphicHandle[number], TRUE);


}