#pragma once
#include "MapChipBase.h"

class ObstacleMapChip : public MapChipBase
{
public:
	ObstacleMapChip();
	virtual ~ObstacleMapChip();

	void Init()override;
	void Update()override;

};