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
	m_isHitGround(false)
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Init()
{
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

void ShotBase::GroundCollision()
{
	// ショットの円
	m_circle = { m_pos, Size::kShotRadius };


	// 地面セル番号
	std::vector<ObjectManager::ChipType>groundCellNumber;
	groundCellNumber.push_back(ObjectManager::ChipType::Ground);
	groundCellNumber.push_back(ObjectManager::ChipType::IceBlock);
	groundCellNumber.push_back(ObjectManager::ChipType::TopConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::BottomConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::LeftConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::RigthConveyor);
	groundCellNumber.push_back(ObjectManager::ChipType::TransparentBlock);

	

	// マップ衝突判定データ
	const std::vector<std::vector<ObjectManager::MapCollisionData>> mapCollData =
		m_pObjectManager->GetMapInfoData().mapCollisionData;


	// マップチップ
	const auto& mapChip = m_pObjectManager->GetMapInfoData().mapChip;

	
	for (int y = 0; y < mapChip.mapHeight; y++)
	{
		for(int x = 0; x < mapChip.mapWidth; x++)
		{
			
			// 地面との当たり判定
			for (int i = 0; i < static_cast<int>(groundCellNumber.size()); i++)
			{
				if (mapCollData[x][y].chipType == groundCellNumber[i])
				{
					// 円と四角形の当たり判定
					if (EvoLib::Collision::IsAABBCollisionDetection(m_circle, mapCollData[x][y].square))
					{
						// 地面に当たったフラグを立てる
						m_isHitGround = true;
					}
				}
			}
		}
	}
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

	// 弾が右方向に進む
	m_pos += m_vec;
}
