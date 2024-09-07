#pragma once

#include <cmath>

// 2�����x�N�g��
class Vec2
{
public:
	float x;
	float y;

public:
	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vec2(float posX, float posY)
	{
		x = posX;
		y = posY;
	}
	// Vec2 = (Vec2 += Vec2)
	Vec2 operator+=(const Vec2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}
	// Vec2 = Vec2 + Vec2
	Vec2 operator+(const Vec2& vec) const
	{
		Vec2 temp{ x + vec.x , y + vec.y };
		return temp;
	}

	// Vec2 = (Vec2 -= Vec2)
	Vec2 operator-=(const Vec2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	// Vec2 = Vec2 - Vec2
	Vec2 operator-(const Vec2& vec) const
	{
		Vec2 temp{ x - vec.x , y - vec.y };
		return temp;
	}

	// Vec2 = (Vec2 *= float)
	Vec2 operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}
	// Vec2 = Vec2 * float
	Vec2 operator*(float scale) const
	{
		Vec2 temp{ x * scale , y * scale };
		return temp;
	}

	// Vec2 = (Vec2 /= float)
	Vec2 operator/=(float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}
	// Vec2 = Vec2 / float
	Vec2 operator/(float scale) const
	{
		Vec2 temp{ x / scale , y / scale };
		return temp;
	}



	// �x�N�g���̒������v�Z���郁�\�b�h
	float length() const
	{
		return std::sqrt(x * x + y * y);
	}



	// �x�N�g���̐��K�����s�����\�b�h
	Vec2 normalize() const
	{
		// �x�N�g���̒������v�Z
		float len = std::sqrt(x * x + y * y);

		// ������0�̏ꍇ�͐��K���ł��Ȃ��̂ł��̂܂ܕԂ�
		if (len == 0)
		{
			return *this;
		}

		return Vec2(x / len, y / len);
	}
};