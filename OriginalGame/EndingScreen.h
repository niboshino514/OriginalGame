#pragma once
#include <memory>

class SceneEnding;

class EndingScreen
{
public:
	EndingScreen();
	virtual ~EndingScreen();

	/// <summary>
	/// �V�[���G���f�B���O�̃|�C���^���擾
	/// </summary>
	/// <param name="sceneEnding">�V�[���G���f�B���O�|�C���^�[</param>
	void SetSceneEndingPointer(SceneEnding* sceneEnding) { m_pSceneEnding = sceneEnding; }

	void Init();
	void Update();
	void Draw();


private:

	// �V�[���G���f�B���O�|�C���^
	SceneEnding* m_pSceneEnding;
};

