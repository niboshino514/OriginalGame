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
	/// �n�[�g�{�b�N�X�X�V����
	/// </summary>
	Hurtbox HurtboxSetting(const Vec2& pos);

	


	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �Փ�
	/// </summary>
	void Collision();

private:

	// �ړ���
	Vec2 m_vec;

	// ���W
	Vec2 m_pos;


};

