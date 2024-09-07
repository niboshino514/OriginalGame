#pragma once
#include "ShotBase.h"

class NormalShot : public ShotBase
{
public:
	NormalShot();
	virtual ~NormalShot();

	void ShotInit() override;
	void Update() override;

};