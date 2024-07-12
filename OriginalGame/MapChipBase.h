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

	// ÉJÉâÅ[
	int m_color;
};

