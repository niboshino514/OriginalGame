#pragma once
#include "MapChipBase.h"

class NoramalMapChip : public MapChipBase
{
public:
	NoramalMapChip();
	virtual ~NoramalMapChip();

	void Init()override;
	void Update()override;
};