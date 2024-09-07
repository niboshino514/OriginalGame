#pragma once

#include <cmath>

// 2次元ベクトル
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


	// 2点間の方向ベクトルを計算
	Vec2 directionVector(const Vec2& p1, const Vec2& p2)
	{
		return { p2.x - p1.x, p2.y - p1.y };
	}

	// 右向き（時計回り）の法線ベクトルを計算
	Vec2 rightNormal(const Vec2& v)
	{
		return { -v.y, v.x };
	}

	// 左向き（反時計回り）の法線ベクトルを計算
	Vec2 leftNormal(const Vec2& v)
	{
		return { v.y, -v.x };
	}


	// ベクトルの内積を計算
	float dot(const Vec2& v1, const Vec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	// ベクトルのスカラー倍
	Vec2 scalarMultiply(const Vec2& v, float scalar)
	{
		return { v.x * scalar, v.y * scalar };
	}


	/// <summary>
	/// 反射ベクトルを計算
	/// </summary>
	/// <param name="v">移動ベクトル</param>
	/// <param name="n">法線ベクトル</param>
	/// <returns></returns>
	Vec2 reflect(const Vec2& v, const Vec2& n)
	{
		float dotProduct = dot(v, n);
		Vec2 scalarPart = scalarMultiply(n, 2 * dotProduct);
		return (v - scalarPart);
	}





	// 長さの取得
	float length()
	{
		return sqrtf((x * x) + (y * y));
	}

	// 正規化 Vec2 = Vec2.normalize()
	Vec2 normalize()
	{
		float len = length();
		if (len == 0)
		{
			return *this;
		}
		return (*this) / len;
	}
};