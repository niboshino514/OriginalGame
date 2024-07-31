#pragma once
#include "Vec2.h"
#include <cassert>
#include <math.h>
#include <vector>
#include "PlatinumLoader.h"
#include <string>


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

// ��`
struct Rect
{
	float top = 0.0f;	// ��
	float bottom = 0.0f;// ��
	float left = 0.0f;	// ��
	float right = 0.0f;	// �E
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


// �����̍\����
struct Intersection
{
	// �����������ǂ����̃t���O
	bool isFrag = false;

	// �������W
	Vec2 pos = Vec2(0.0f, 0.0f);
};

/// <summary>
/// �����̍\����
/// </summary>
struct Line
{
	float a; // �X��
	float b; // y���Ƃ̌�_
};

// �O�p�`
struct Triangle
{
	// ���W1
	Vec2 pos_1 = Vec2();

	// ���W2
	Vec2 pos_2 = Vec2();

	// ���W3
	Vec2 pos_3 = Vec2();
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


	/// <summary>
	/// ���W���Z�����ɕϊ�����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="chipSize">�`�b�v�T�C�Y</param>
	/// <returns>�ϊ����ꂽ�Z�����</returns>
	static Cell CoordinateWithCellToConversion(const Vec2& pos, const float& chipSize);


	/// <summary>
	/// �Z���������W�ɕϊ�����
	/// </summary>
	/// <param name="cell">�Z��</param>
	/// <param name="chipSize">�`�b�v�T�C�Y</param>
	/// <returns>�ϊ����ꂽ���W</returns>
	static Vec2 CellWithCoordinateToConversion(const Cell& cell, const float& chipSize);

	/// <summary>
	/// �~�̓����蔻��
	/// </summary>
	/// <param name="circle1">�~1</param>
	/// <param name="cphere2">�~2</param>
	/// <returns>�~���m�������������ǂ�����Ԃ�</returns>
	static bool CircleCollision(const Circle& circle1, const Circle& cphere2);


	/// <summary>
	/// �����̌X����Y���Ƃ̌�_��2�_����v�Z����
	/// </summary>
	/// <param name="start">�X�^�[�g���W</param>
	/// <param name="end">�G���h���W</param>
	/// <returns></returns>
	static Line CalculateLine(const Vec2& start, const Vec2& end);

	/// <summary>
	/// 2�̒�����_���v�Z����
	/// </summary>
	/// <param name="line1">����1</param>
	/// <param name="line2">����2</param>
	/// <returns>�������(�d�Ȃ��Ă��邩�ǂ����̃t���O,�d�Ȃ��Ă�����W)</returns>
	static Intersection CalculateIntersection(const Line& line1, const Line& line2);

	/// <summary>
	/// �O�����̒藝
	/// </summary>
	/// <param name="poa1">���W1</param>
	/// <param name="pos2">���W2</param>
	/// <returns>���p�O�p�`�̈�ӂ̒���</returns>
	static float ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2);

	/// <summary>
	/// �l�p�`��񂩂璆�S���W�����߂�
	/// </summary>
	/// <param name="square">�l�p�`���W</param>
	/// <returns>�l�p�`���</returns>
	static Vec2 QuadrangularCenter(const Square& square);

	/// <summary>
	/// �l�p�`�̒��S�_����сA���S�_����̊e���_�����̍ő�l�𔼌a�Ƃ����~�̏���Ԃ�
	/// </summary>
	/// <param name="square">�l�p�`���</param>
	/// <returns>�~���</returns>
	static Circle CalculateQuadrangularCenter(const Square& square);

	/// <summary>
	/// �l�����̐����ǂ����𒲂ׂ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="value">���ׂ�l</param>
	/// <returns>�l�����̐����ǂ����̃t���O</returns>
	template <typename T>
	static bool IsValueNegativeCount(T& value);

	/// <summary>
	/// ���`��Ԑ������߂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="vec">�ړ���</param>
	/// <param name="size">�c����</param>
	/// <returns>���`��Ԑ�</returns>
	static int IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, const Vec2& size);

	/// <summary>
	/// ���`��ԍ��W�����߂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="vec">�ړ���</param>
	/// <param name="iinearInterpolationCount">���`��Ԑ�</param>
	/// <returns>���`��ԍ��W</returns>
	static std::vector<Vec2> IinearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& iinearInterpolationCount);

	/// <summary>
	/// �󂯎�������W�𒆐S�Ƃ�����`���v�Z����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="size">�T�C�Y</param>
	/// <returns>���W�𒆐S�Ƃ�����`</returns>
	static Rect RectangleCalculation(const Vec2& pos, const Vec2& size);

	/// <summary>
	/// �}�b�v�`�b�v�̋�`�̈ړ��͈͂�Ԃ�
	/// </summary>
	/// <param name="rect">��`���</param>
	/// <param name="mapInfo">�}�b�v���</param>
	/// <param name="mapData">�}�b�v�f�[�^</param>
	/// <returns>�ړ��͈�</returns>
	static Rect GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapChip& mapInfo, const std::vector<std::vector<int>>& mapData);

	/// <summary>
	/// �Z�����͈͓����ǂ���
	/// </summary>
	/// <param name="cell">�Z��</param>
	/// <param name="maxCell">�ő�Z��</param>
	/// <param name="minCell">�ŏ��Z��</param>
	/// <returns>�Z�����͈͓����ǂ�����Ԃ�</returns>
	static bool IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell);

	/// <summary>
	/// �l�p�`�ƎO�p�`�̓����蔻��
	/// </summary>
	/// <param name="square">�l�p�`���</param>
	/// <param name="triangle">�O�p�`���</param>
	/// <returns>�l�p�`�ƎO�p�`�̓����蔻��</returns>
	static bool CollisionDetectionOfQuadrangleAndTriangle(const Square& square, const Triangle& triangle);

	/// <summary>
	/// �l�p�`���m���������Ă��邩�ǂ����𒲂ׂ�
	/// </summary>
	/// <param name="square1">�l�p�`���1</param>
	/// <param name="square2">�l�p�`���2</param>
	/// <returns>�l�p�`���m���������Ă��邩�ǂ���</returns>
	static bool CollisionDetectionOfQuadrangleAndQuadrangle(const Square& square1, const Square& square2);

	/// <summary>
	/// �������m���������Ă��邩�ǂ����𒲂ׂ�
	/// </summary>
	/// <param name="start1">�X�^�[�g���W�P</param>
	/// <param name="end1">�S�[�����W�P</param>
	/// <param name="start2">�X�^�[�g���W�Q</param>
	/// <param name="end2">�S�[�����W�Q</param>
	/// <returns>�������m���������Ă��邩�ǂ����𒲂ׂ�</returns>
	static bool CheckLineIntersection(const Vec2& start1, const Vec2& end1, const Vec2& start2, const Vec2& end2);

	/// <summary>
	/// �|���S���̒��Ƀ|�C���g���W�������Ă��邩�ǂ����𒲂ׂ�
	/// </summary>
	/// <param name="point">���W</param>
	/// <param name="polygon">�|���S��</param>
	/// <returns>�|���S���̒��Ƀ|�C���g���W�������Ă��邩�ǂ���</returns>
	static bool PointInPolygon(const Vec2& point, const std::vector<Vec2>& polygon);

	/// <summary>
	/// ��`�����l�p�`���ɕϊ�����
	/// </summary>
	/// <param name="rect">��`���</param>
	/// <returns>�l�p�`���</returns>
	static Square RectToSquare(const Rect& rect);


	/// <summary>
	/// �G���[���b�Z�[�W
	/// </summary>
	/// <param name="errorMessage">�G���[���b�Z�[�W</param>
	static void ErrorAssertMessage(const std::string& errorMessage);

};

template<typename T>
inline bool FunctionConclusion::IsValueNegativeCount(T& value)
{
	// �l��0�ȉ��Ȃ�΁Afalse
	if (value > 0.0f)
	{
		return false;
	}


	return true;
}
