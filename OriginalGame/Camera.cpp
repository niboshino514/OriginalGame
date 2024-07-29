#include "Camera.h"
#include "game.h"
#include "ObjectFactory.h"
#include "GameData.h"

Camera::Camera():
	m_cameraPos()
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// オフセット値を求める
	OffsetCalculation(GameData::GetInstance()->GetPlayerPos());
}

void Camera::OffsetCalculation(const Vec2& targetPos)
{
	// ターゲットオフセット
	Vec2 offset = Vec2();

	
	

	// マップの縦横幅を代入
	const float mapWidth = (m_pObjectFactory->GetMapInfoData().mapChip.mapWidth * m_pObjectFactory->GetMapInfoData().mapChip.chipSize);
	const float mapHeight = (m_pObjectFactory->GetMapInfoData().mapChip.mapHeight * m_pObjectFactory->GetMapInfoData().mapChip.chipSize);



	// スクロールの計算、プレイヤーが画面中央に表示されるようにスクロールする
	offset.x = (Game::kScreenWidth / 2) - targetPos.x;
	if (offset.x > 0)
	{
		offset.x = 0;
	}
	if (offset.x < -mapWidth + static_cast<float>(Game::kScreenWidth))
	{
		offset.x = -mapWidth + static_cast<float>(Game::kScreenWidth);
	}

	offset.y = (Game::kScreenHeight / 2) - targetPos.y;
	if (offset.y > 0)
	{
		offset.y = 0;
	}
	if (offset.y < -mapHeight + static_cast<float>(Game::kScreenHeight))
	{
		offset.y = -mapHeight + static_cast<float>(Game::kScreenHeight);
	}


	m_cameraPos = offset * 0.2f + m_cameraPos * 0.8f;


	// スクリーンの半分の横幅
	const float screenWidthCenter = Game::kScreenWidth * 0.5f;
	const float screenHeightCenter = Game::kScreenHeight * 0.5f;



	// 補正オフセット値
	Vec2 correctionOffset = m_cameraPos;


	if (Game::kScreenWidth >= mapWidth)
	{
		// 中心マップ値
		const float centerMapValue = correctionOffset.x - screenWidthCenter;

		// 補正オフセット値計算
		correctionOffset.x = (m_cameraPos.x - centerMapValue) - (mapWidth * 0.5f);
	}

	if (Game::kScreenHeight >= mapHeight)
	{
		// 中心マップ値
		const float centerMapValue = correctionOffset.y - screenHeightCenter;

		// 補正オフセット値計算
		correctionOffset.y = (m_cameraPos.y - centerMapValue) - (mapHeight * 0.5f);
	}

	// ゲームデータにカメラ座標を代入
	GameData::GetInstance()->SetCameraPos(correctionOffset);
}
