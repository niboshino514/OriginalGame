#include "DxLib.h"

#include "game.h"

#include "SceneManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// 最初のシーンの初期化
	SceneManager scene;
	scene.init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		scene.update();
		scene.draw();


		// フレーム終了時間を計測
		const LONGLONG endTime = GetNowHiPerformanceCount();
		const LONGLONG frameTime = endTime - time;

		// フレーム時間をミリ秒単位に変換して表示
		float frameTimeMs = static_cast<float>(frameTime) / 1000.0f;
		DrawFormatString(0, 0, 0xffffff, "処理速度ミリ秒単位=%f", frameTimeMs);


		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	scene.end();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}