#include "GameOver.h"
#include "EvoLib.h"
#include "GameData.h"
#include "Vec2.h"
#include "game.h"

namespace
{
	// ゲームオーバーグラフィックファイル名
	static const char* const kGameOverFileName = "Data/Graphic/GameMain/GameOver.png";

}


GameOver::GameOver():
	m_gameOverGraphHandle(-1)
{
}

GameOver::~GameOver()
{
	// グラフィックハンドル解放
	DeleteGraph(m_gameOverGraphHandle);
}

void GameOver::Init()
{
	// ゲームオーバーグラフィック読み込み
	m_gameOverGraphHandle = EvoLib::Load::LoadGraph_EvoLib(kGameOverFileName);

}

void GameOver::Draw()
{
	// プレイヤーが死亡していない場合、処理を行わない
	if(GameData::GetInstance()->GetIsPlayerAlive())
	{
		return;
	}


	// 画面全体を薄く塗りつぶす
	{
		// 透明度を半分に設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

		// 画面全体を黒で塗りつぶす
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, TRUE);

		// 透明度を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	
	// 画面中央にゲームオーバーグラフィックを描画
	{
		// ゲームオーバーグラフィックのサイズを取得
		int w, h;
		GetGraphSize(m_gameOverGraphHandle, &w, &h);

		// ゲームオーバーグラフィックを画面中央に描画
		DrawGraph(Game::kScreenWidth / 2 - w / 2, Game::kScreenHeight / 2 - h / 2, m_gameOverGraphHandle, TRUE);
	}
}
