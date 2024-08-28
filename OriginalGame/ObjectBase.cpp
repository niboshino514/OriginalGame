#include "ObjectBase.h"

ObjectBase::ObjectBase():
	m_objectID(),
	m_isExlist(true),
	m_isGimmick(false),
	m_circle(),
	m_square(),
	m_drawRank(),
	m_graphicHandle(),
	m_animationDetails(),
	m_animationRange()
{
}

ObjectBase::~ObjectBase()
{
}

int ObjectBase::AnimationNamberUpdate(const AnimationDetails& animationDetails, const AnimationRange& animationRange)
{
	// 加算減算の判定
	static bool isAdd = true;
	// フレームカウント
	static int frameCount = 0;
	// アニメーション番号
	int animationNum = animationDetails.number;


	if (animationNum <= animationRange.minIndex)
	{
		isAdd = true;
	}
	else if (animationNum >= animationRange.maxIndex)
	{
		isAdd = false;
	}

	// フレーム加算
	frameCount++;

	// アニメーション処理
	if (frameCount >= animationDetails.frameSpeed)
	{
		frameCount = 0;

		// 加算減算の判定
		if (isAdd)
		{
			animationNum++;
		}
		else
		{
			animationNum--;
		}
	}

	// アニメーション番号を返す
	return animationNum;
}

