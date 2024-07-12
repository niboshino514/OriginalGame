#pragma once
#include "Vec2.h"

// �Z���\����
struct Cell
{
	int x = 0;	// �Z��X
	int y = 0;	// �Z��Y
};

// �~�\����
struct Circle
{
	// ���S���W
	Vec2 centerPos = Vec2();

	// �~�̔��a
	float radius = 0.0f;
};

// �l�p�`
struct Square
{
	// ������W
	Vec2 A = Vec2();

	// �E����W
	Vec2 B = Vec2();

	// �E�����W
	Vec2 C = Vec2();

	// �������W
	Vec2 D = Vec2();
};


class FunctionConclusion
{
public:


	/// <summary>
	/// �~�Ǝl�p�`�Ƃ̓����蔻��
	/// </summary>
	/// <param name="circle">�~���</param>
	/// <param name="square">�l�p�`���</param>
	/// <returns>�~�Ǝl�p�`�������������ǂ�����Ԃ�</returns>
	static bool IsAABBCollisionDetection(const Circle& circle, const Square& square);

};