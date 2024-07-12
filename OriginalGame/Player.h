#pragma once
#include "ObjectBase.h"



class Player : public ObjectBase
{
public:
	Player();
	virtual ~Player();


	void Init()override;
	void Update()override;
	void Draw()override;

private:

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �Փ�
	/// </summary>
	void Collision();

private:
	Vec2 m_vec;

};

