#pragma once
#include "SceneBase.h"
#include <memory>

class OpeningScreen;

class SceneOpening : public SceneBase
{
public:
	SceneOpening();
	virtual ~SceneOpening();


	virtual void Init() override;
	virtual SceneBase* Update() override;
	virtual void Draw() override;

public:



	/// <summary>
	/// シーン変更
	/// </summary>
	void ChangeScene();



private:

	/// <summary>
	/// フェードアウト時、だんだん音が小さくなるサウンド処理
	/// </summary>
	void FadeOutSound();

private:

	////////////////////
	// クラスポインタ //
	////////////////////

	// オープニングスクリーンポインタ
	std::shared_ptr<OpeningScreen> m_pOpeningScreen;

};