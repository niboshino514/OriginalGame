#pragma once

namespace Game
{
	// ウインドウモード設定
	constexpr bool kWindowMode = true;
	// ウインドウ名
	const char* const kTitleText = "OriginalGame";

	// ウインドウサイズ
	constexpr int kScreenWidth = 1400;
	constexpr int kScreenHeight = 800;

	// ウインドウサイズ(float型)
	constexpr float kScreenWidth_F = static_cast<float>(kScreenWidth);
	constexpr float kScreenHeight_F = static_cast<float>(kScreenHeight);


	// ウィンドウ中心座標
	constexpr int kWindowCenterX = kScreenWidth / 2;
	constexpr int kWindowCenterY = kScreenHeight / 2;

	// ウィンドウ中心座標(float型)
	constexpr float kWindowCenterX_F = kScreenWidth_F * 0.5f;
	constexpr float kWindowCenterY_F = kScreenHeight_F * 0.5f;

	// カラーモード
	constexpr int kColorDepth = 32;		// 32 or 16
};