#pragma once
#include "ObjectBase.h"

class NoramalMapChip : public ObjectBase
{
public:
	NoramalMapChip();
	virtual ~NoramalMapChip();

	void Init()override;
	void Update()override;
	void Draw()override;
};