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

	// ウィンドウ中心座標
	constexpr int kWindowCenterX = kScreenWidth / 2;
	constexpr int kWindowCenterY = kScreenHeight / 2;

	// カラーモード
	constexpr int kColorDepth = 32;		// 32 or 16
};