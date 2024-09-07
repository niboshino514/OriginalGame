#pragma once
#include "ShotBase.h"

class SineCurveShot : public ShotBase
{
public:
	SineCurveShot();
	virtual ~SineCurveShot();
	
	void ShotInit() override;
	void Update() override;

private:

	void SineCurveMove();

private:

	// サインカーブのデータ
	EvoLib::Calculation::SineCurveData<float> m_sineCurveData;
};