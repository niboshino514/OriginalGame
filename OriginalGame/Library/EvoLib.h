#pragma once
#include <DxLib.h>
#include <vector>
#include "Vec2.h"
#include <string>


/// <summary>
/// ����
/// </summary>
enum class Direction
{
	Top,	// ��
	Bottom,	// ��
	Left,	// ��
	Right,	// �E
	DirectionNum,// ������
};

/// <summary>
/// ���̏��
/// </summary>
struct Sphere
{
	// ���W
	VECTOR pos = { 0.0f,0.0f,0.0f };
	// ���a
	float radius = 0.0f;
};

/// <summary>
/// �Z�����
/// </summary>
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

/// <summary>
/// ��`���
/// </summary>
struct Rect
{
	float top = 0.0f;	// ��
	float bottom = 0.0f;// ��
	float left = 0.0f;	// ��
	float right = 0.0f;	// �E
};

/// <summary>
/// �l�p�`���
/// </summary>
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


/// <summary>
/// �����̍\����
/// </summary>
struct Intersection
{
	// �����������ǂ����̃t���O
	bool isFrag = false;

	// �������W
	Vec2 pos = Vec2(0.0f, 0.0f);
};

/// <summary>
/// �������
/// </summary>
struct Line
{
	float a; // �X��
	float b; // y���Ƃ̌�_
};

/// <summary>
/// �O�p�`���
/// </summary>
struct Triangle
{
	// ���W1
	Vec2 pos_1 = Vec2();

	// ���W2
	Vec2 pos_2 = Vec2();

	// ���W3
	Vec2 pos_3 = Vec2();
};

#include "FPS.h"
#include "Convert.h"
#include "Collision.h"
#include "Calculation.h"
#include "Load.h"
#include "Draw.h"
#include "File.h"