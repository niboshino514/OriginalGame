#pragma once
#include "ObjectBase.h"

class MapChipBase : public ObjectBase
{
public:
	MapChipBase();
	virtual ~MapChipBase();

	void Init()override = 0;
	void Update()override = 0;
	void Draw()override = 0;

protected:

	/// <summary>
	/// Õ“Ëˆ—
	/// </summary>
	void Collision();
};