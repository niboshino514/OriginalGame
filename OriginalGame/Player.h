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
	/// ˆÚ“®
	/// </summary>
	void Move();

	/// <summary>
	/// Õ“Ë
	/// </summary>
	void Collision();

private:
	Vec2 m_vec;

};

