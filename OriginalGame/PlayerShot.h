#pragma once
#include "ShotBase.h"

class PlayerShot : public ShotBase
{
public:
	PlayerShot();
	virtual ~PlayerShot();

	void Init() override;
	void Update() override;


};