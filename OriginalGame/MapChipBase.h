#pragma once
#include "ObjectBase.h"

class MapChipBase : public ObjectBase
{
public:
	MapChipBase();
	virtual ~MapChipBase();

	void Init()override;
	void Update()override;
	void Draw()override;

protected:

	// �J���[
	int m_color;
};

