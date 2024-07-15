#include "PreviousStageMapChip.h"

PreviousStageMapChip::PreviousStageMapChip()
{
}

PreviousStageMapChip::~PreviousStageMapChip()
{
}

void PreviousStageMapChip::Init()
{
	// オブジェクトID設定
	m_objectID = ObjectID::PreviousStageMapChip;

	// カラー代入
	m_color = 0x8a2be2;
}

void PreviousStageMapChip::Update()
{
	// 当たり判定
	Collision();
}

void PreviousStageMapChip::Collision()
{
	for (auto& object : m_pObjectFactory->GetObjectInfo())
	{
		switch (object->GetObjectID())
		{
		case ObjectID::Player:

			// 当たっていた場合、前のステージに移行する
			if (FunctionConclusion::IsAABBCollisionDetection(object->GetCircle(), m_square))
			{
				m_pObjectFactory->StageMove(ObjectFactory::MapSwitchType::PreviousStage);
				return;
			}

			break;
		default:
			break;
		}
	}
}
