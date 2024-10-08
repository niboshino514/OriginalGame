#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	class FPS
	{
	public:

		// DXLibを60FPSで動かす
		static constexpr int Dxlib_60FPS = 16667;
		// FPS
		static constexpr int Fps = 60;


		// FPSの取得
		static int GetFPS_EvoLib();
	};
}