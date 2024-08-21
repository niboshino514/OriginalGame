#pragma once
#include <memory>


class ObjectManager;

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
	// �N���X�|�C���^ //
	////////////////////

	// �I�u�W�F�N�g�t�@�N�g���[�|�C���^
	std::shared_ptr<ObjectManager>m_pObjectFactory;

};