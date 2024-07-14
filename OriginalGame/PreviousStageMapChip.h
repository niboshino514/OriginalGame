#pragma once
#include "MapChipBase.h"


class PreviousStageMapChip : public MapChipBase
{
public:
	PreviousStageMapChip();
	virtual ~PreviousStageMapChip();

	void Init()override;
	void Update()override;

private:

	/// <summary>
	/// “–‚½‚è”»’è
	/// </summary>
	void Collision();

};