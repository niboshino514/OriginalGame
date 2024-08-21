#pragma once

class SceneTitle;

class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	/// <summary>
	/// シーンタイトルのポインタを取得
	/// </summary>
	/// <param name="sceneTitle">シーンタイトルポインタ</param>
	void SetSceneTitle(SceneTitle* sceneTitle) { m_pSceneTitle = sceneTitle; }

	void Init();
	void Update();
	void Draw();

private:

	// シーンタイトルポインタ
	SceneTitle* m_pSceneTitle;
};