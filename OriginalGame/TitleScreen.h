#pragma once

class SceneTitle;

class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	/// <summary>
	/// �V�[���^�C�g���̃|�C���^���擾
	/// </summary>
	/// <param name="sceneTitle">�V�[���^�C�g���|�C���^</param>
	void SetSceneTitle(SceneTitle* sceneTitle) { m_pSceneTitle = sceneTitle; }

	void Init();
	void Update();
	void Draw();

private:

	// �V�[���^�C�g���|�C���^
	SceneTitle* m_pSceneTitle;
};