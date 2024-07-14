#include "NextStageMapChip.h"



NextStageMapChip::NextStageMapChip()
{
}

NextStageMapChip::~NextStageMapChip()
{
}

void NextStageMapChip::Init()
{
	// オブジェクトID設定
	m_objectID = ObjectID::NextStageMapChip;

	// カラー代入
	m_color = 0xe0ffff;
}

void NextStageMapChip::Update()
{
	// 当たり判定
	Collision();
}

void NextStageMapChip::Collision()
{
	for (auto& object : m_pObjectFactory->GetObjectInfo())
	{
		switch (object->GetObjectID())
		{
		case ObjectID::Player:

			// 当たっていた場合、次のステージに移行する
			if (FunctionConclusion::IsAABBCollisionDetection(object->GetCircle(), m_square))
			{
				m_pObjectFactory->StageMove(true);
			
				return;
			}

			break;
		default:
			break;
		}
	}
}
