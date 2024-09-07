#pragma once
#include "MapChipBase.h"

class TransparentBlockChip : public MapChipBase
{
public:
	TransparentBlockChip();
	virtual ~TransparentBlockChip();

	void Init()override;
	void Update()override;
	void Draw()override;

private:
};