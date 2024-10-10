#pragma once
#include "MapChipBase.h"

class SaveChip : public MapChipBase
{
public:
	SaveChip();
	virtual ~SaveChip();

	void Init()override;
	void Update()override;
	void Draw()override;

private:
};

