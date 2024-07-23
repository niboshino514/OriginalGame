#pragma once
#include "Vec2.h"
#include <cassert>
#include <math.h>
#include <vector>
#include "PlatinumLoader.h"


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

// �n�[�g�{�b�N�X
struct Hurtbox
{
	// ���S���W
	Vec2 centerPos = Vec2();

	// ����W
	Vec2 topPos = Vec2();

	// �����W
	Vec2 bottomPos = Vec2();

	// �����W
	Vec2 leftPos = Vec2();

	// �E���W
	Vec2 rightPos = Vec2();
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
	/// <param name="heghtSize">�c��</param>
	/// <param name="widthSize">����</param>
	/// <returns>���`��Ԑ�</returns>
	static int IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, float heghtSize, float widthSize);

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
	static Rect GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapInfo& mapInfo, const PlatinumLoader::MapData& mapData);

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
