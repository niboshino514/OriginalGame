#include "TransparentBlockChip.h"
#include <DxLib.h>

#include <filesystem>
#include "EvoLib.h"

TransparentBlockChip::TransparentBlockChip()
{
}

TransparentBlockChip::~TransparentBlockChip()
{
}

void TransparentBlockChip::Init()
{
	// オブジェクトIDを設定
	m_objectID = ObjectID::TransparentBlockChip;
}

void TransparentBlockChip::Update()
{
	// 衝突処理
	Collision();
}

void TransparentBlockChip::Draw()
{
	// ギミックフラグが立っていない場合は描画しない
	if (!m_isGimmick)
	{
		return;
	}


	// オフセット値を取得
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// 描画四角形
	Square drawSquare = m_square;
	drawSquare.A += offset;
	drawSquare.B += offset;
	drawSquare.C += offset;
	drawSquare.D += offset;

	// 透明ブロック描画
	DrawBoxAA(drawSquare.A.x, drawSquare.A.y, drawSquare.C.x, drawSquare.C.y, 0xffffff, TRUE);
}
