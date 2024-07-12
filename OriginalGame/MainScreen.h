#pragma once
#include <memory>


class ObjectFactory;

class MainScreen
{
public:
	MainScreen();
	virtual ~MainScreen();

	void Init();
	void Update();
	void Draw();


private:

	////////////////////
	// クラスポインタ //
	////////////////////

	// オブジェクトファクトリーポインタ
	std::shared_ptr<ObjectFactory>m_pObjectFactory;

};