#include "ShotBase.h"
#include "EvoLib.h"

namespace
{
	// ショットのスピード
	constexpr float kShotSpeed = 10.0f;
}

namespace Size
{
	// ショット半径サイズ
	constexpr float kShotRadius = 5.0f;
}


ShotBase::ShotBase():
	m_isHitGround(false),
	m_moveRect()
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Init()
{
	// 地面判定初期化
	GroundCollisionInit();


	// ショットの初期化
	ShotInit();
}

void ShotBase::Update()
{
	// ショットの移動
	m_pos.x += kShotSpeed;

	// 当たり判定
	GroundCollision();
}

void ShotBase::Draw()
{
	// オフセット値を取得
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// ショットの描画
	DrawCircleAA(m_pos.x + offset.x, m_pos.y + offset.y, Size::kShotRadius, 5, GetColor(255, 255, 255), TRUE);
}

void ShotBase::GroundCollisionInit()
{



	// マップチップのサイズを取得
	float mapChipSize = m_pObjectManager->GetMapInfoData().mapChip.chipSize;

	// マップチップの最大セルを取得
	Cell maxCell = Cell(m_pObjectManager->GetMapInfoData().mapChip.mapWidth, m_pObjectManager->GetMapInfoData().mapChip.mapHeight);


	// 地面セル番号
	std::vector<int>groundCellNumber;
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::Ground));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::IceBlock));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TopConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::BottomConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::LeftConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::RigthConveyor));
	groundCellNumber.push_back(static_cast<int>(ObjectManager::ChipType::TransparentBlock));


	// 円の直径
	const Vec2 diameter = Vec2(Size::kShotRadius * 2.0f, Size::kShotRadius * 2.0f);

	// ショット矩形
	const Rect shotRect = EvoLib::Convert::PosToRect(m_pos, diameter);

	// 移動可能範囲の矩形を取得
	m_moveRect = EvoLib::Calculation::CalculateRectangleMovementRange
	(shotRect, maxCell, mapChipSize, m_pObjectManager->GetMapChipNumber(), groundCellNumber);

}

void ShotBase::GroundCollision()
{

#if true
	// オフセット値を取得
	const Vec2 offset = GameData::GetInstance()->GetCameraPos();

	// 移動可能範囲の矩形を取得
	Rect drawMoveRect = Rect();

	drawMoveRect.left = m_moveRect.left + offset.x;
	drawMoveRect.right = m_moveRect.right + offset.x;
	drawMoveRect.bottom = m_moveRect.bottom + offset.y;
	drawMoveRect.top = m_moveRect.top + offset.y;

	// 移動できる場所を描画
	DrawBoxAA(drawMoveRect.left, drawMoveRect.top, drawMoveRect.right, drawMoveRect.bottom,
		0x0000ff, true);
#endif


	// 法線ベクトル
	Vec2 normal = Vec2();


	const Vec2 leftTop = Vec2(m_moveRect.left, m_moveRect.top);
	const Vec2 rightTop = Vec2(m_moveRect.right, m_moveRect.top);
	const Vec2 leftBottom = Vec2(m_moveRect.left, m_moveRect.bottom);
	const Vec2 rightBottom = Vec2(m_moveRect.right, m_moveRect.bottom);

	Vec2 temp[2] = { Vec2(), Vec2() };

	// 移動後の座標
	Vec2 afterPos = m_pos + m_vec;


	// 左端に当たった場合
	if (afterPos.x <= m_moveRect.left)
	{
		temp[0] = leftTop;
		temp[1] = leftBottom;

	}
	else if(afterPos.x >= m_moveRect.right)
	{
		temp[0] = rightTop;
		temp[1] = rightBottom;
	}
	else if(afterPos.y <= m_moveRect.top)
	{
		temp[0] = leftTop;
		temp[1] = rightTop;
	}
	else if(afterPos.y >= m_moveRect.bottom)
	{
		temp[0] = leftBottom;
		temp[1] = rightBottom;
	}
	else
	{
		return;
	}


	// 壁に当たった
	//m_isHitGround = true;

	// 反射ベクトルを計算
	const Vec2 reflection = EvoLib::Calculation::ReflectVector(m_vec, temp[0], temp[1]);

	// 反射ベクトルをラジアン変換
	float rad = atan2(reflection.y, reflection.x);

	// ラジアンを角度に変換
	m_shotData.angle = EvoLib::Convert::ConvertRadianToAngle(rad);




}

void ShotBase::Move()
{
	
	// ラジアンを求める
	const float rad = EvoLib::Convert::ConvertAngleToRadian(m_shotData.angle);

	// ラジアンからベクトルを求める
	m_vec.x = cos(rad);
	m_vec.y = sin(rad);

	// ベクトルの正規化
	m_vec = m_vec.normalize();

	// ベクトルにスピードを掛ける
	m_vec *= m_shotData.speed;

	// ベース座標に移動量を加算
	m_basePos += m_vec;

	// 座標を更新
	m_pos = m_basePos;
}
