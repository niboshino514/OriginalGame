#include "DxLib.h"
#include "game.h"
#include "SceneManager.h"
#include <crtdbg.h>
#include "Sound.h"
#include "crtdbg.h"
#include "Controller.h"
#include "GameData.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// メモリをすべて解放しているかどうかを調べる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// リークが発生しているメモリ確保回数を引数にすることで、
    // その個所にデバッグブレークをかけてくれる
//	_CrtSetBreakAlloc(756);

	// 画面モードの設定
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// 出力ログtxtを出さないようにする
	SetOutApplicationLogValidFlag(false);

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


	// サウンドロード
	Sound::GetInstance()->Init();

	// セーブロード
	GameData::GetInstance()->LoadSaveData();	// セーブデータの読み込み
	GameData::GetInstance()->LoadScoreData();	// スコアデータの読み込み


	// 最初のシーンの初期化
	SceneManager scene;
	scene.Init();





	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		scene.Update();
		scene.Draw();


		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	// シーン終了
	scene.End();

	// セーブ書き込み
	GameData::GetInstance()->WriteSaveData();	// セーブデータの書き込み

	// シングルトン解放
	{
		// サウンドシングルトン解放
		Sound::GetInstance()->DeleteInstance();

		// コントローラー解放
		Controller::GetInstance()->DeleteInstance();

		// ゲームデータ解放
		GameData::GetInstance()->DeleteInstance();
	}

	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}