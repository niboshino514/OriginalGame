#include "FPS.h"

int EvoLib::FPS::GetFPS_EvoLib()
{
    // 現在のFPSを取得する
    const float fps = GetFPS();

    // 取得したFPSを返す
    return static_cast<int>(fps);
}
