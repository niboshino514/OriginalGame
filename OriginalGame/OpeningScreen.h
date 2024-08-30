#pragma once
#include <memory>

class SceneOpening;
class MessageWindow;

class OpeningScreen : public std::enable_shared_from_this<OpeningScreen>
{
public:
	OpeningScreen();
	virtual ~OpeningScreen();

	/// <summary>
	/// シーンオープニングポインタを設定
	/// </summary>
	/// <param name="pSceneOpening">シーンオープニングポインタ</param>
	void SetSceneOpeningPointer(SceneOpening* pSceneOpening) { m_pSceneOpening = pSceneOpening; }

	void Init();
	void Update();
	void Draw();	

private:

	////////////////////
	// クラスポインタ //
	////////////////////

	// シーンオープニングポインタ
	SceneOpening* m_pSceneOpening;
	
	// メッセージウィンドウ
	std::shared_ptr<MessageWindow> m_pMessageWindow;

};

