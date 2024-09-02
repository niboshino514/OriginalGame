#pragma once
#include "MapChipBase.h"


class BossSpawnFlagChip : public MapChipBase
{
public:
	BossSpawnFlagChip();
	virtual ~BossSpawnFlagChip();


	void Init()override;
	void Update()override;
	void Draw()override {};

};