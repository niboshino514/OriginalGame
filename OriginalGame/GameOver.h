#pragma once


class GameOver
{
public:
	GameOver();
	virtual ~GameOver();

	void Init();
	void Draw();

private:

	// ゲームオーバーグラフィックハンドル
	int m_gameOverGraphHandle;

};