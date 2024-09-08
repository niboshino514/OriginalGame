#pragma once
#include "ShotBase.h"

class ReflectionShot : public ShotBase
{
public:
	ReflectionShot();
	~ReflectionShot();

	// ショットの初期化
	void ShotInit() override;

	// ショットの更新
	void ShotUpdate() override;
};