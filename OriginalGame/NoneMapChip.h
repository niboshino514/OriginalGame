#pragma once
#include "ObjectBase.h"

class NoneMapChip : public ObjectBase
{
public:
	NoneMapChip();
	virtual ~NoneMapChip();

	void Init()override;
	void Update()override;
	void Draw()override;
};

