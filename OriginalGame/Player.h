#pragma once
#include "ObjectBase.h"





class Player : public ObjectBase
{
public:

	// �W�����v���
	struct JumpInfo
	{
		// �W�����v���Ă��邩�ǂ���
		bool isJump = false;

		// �������x
		float fallSpeed = 0.0f;

		// �W�����v��
		int jumpCount = 0;
	};


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
	/// �W�����v����
	/// </summary>
	void Jump();


	/// <summary>
	/// �Փ�
	/// </summary>
	void Collision();

private:


	/// <summary>
	/// �n�ʂ̏Փ˔���
	/// </summary>
	void GroundCollision();

	/// <summary>
	/// �}�b�v�ړ��`�b�v�̓����蔻��
	/// </summary>
	void MapMoveChipCollision();

private:

	// �ړ���
	Vec2 m_vec;

	// ���W
	Vec2 m_pos;

	// ��`
	Rect m_rect;

	// �ړ���`
	Rect m_moveRect;


	////////////////////
	/// �W�����v�֘A ///
	////////////////////
	
	// �W�����v���
	JumpInfo m_jumpInfo;

};

