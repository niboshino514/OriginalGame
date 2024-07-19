#pragma once
#include "Vec2.h"
#include <cassert>
#include <math.h>

// セル構造体
struct Cell
{
	int x = 0;	// セルX
	int y = 0;	// セルY
};

// 円構造体
struct Circle
{
	// 中心座標
	Vec2 centerPos = Vec2();

	// 円の半径
	float radius = 0.0f;
};

// 四角形
struct Square
{
	// 左上座標
	Vec2 A = Vec2();

	// 右上座標
	Vec2 B = Vec2();

	// 右下座標
	Vec2 C = Vec2();

	// 左下座標
	Vec2 D = Vec2();
};

// ハートボックス
struct Hurtbox
{
	// 中心座標
	Vec2 centerPos = Vec2();

	// 上座標
	Vec2 topPos = Vec2();

	// 下座標
	Vec2 bottomPos = Vec2();

	// 左座標
	Vec2 leftPos = Vec2();

	// 右座標
	Vec2 rightPos = Vec2();
};

class FunctionConclusion
{
public:


	/// <summary>
	/// 円と四角形との当たり判定
	/// </summary>
	/// <param name="circle">円情報</param>
	/// <param name="square">四角形情報</param>
	/// <returns>円と四角形が当たったかどうかを返す</returns>
	static bool IsAABBCollisionDetection(const Circle& circle, const Square& square);



	/// <summary>
	/// 座標をセル情報に変換する
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="chipSize">チップサイズ</param>
	/// <returns>変換されたセル情報</returns>
	static Cell CoordinateWithCellToConversion(const Vec2& pos, const float& chipSize);


	/// <summary>
	/// セル情報を座標に変換する
	/// </summary>
	/// <param name="cell">セル</param>
	/// <param name="chipSize">チップサイズ</param>
	/// <returns>変換された座標</returns>
	static Vec2 CellWithCoordinateToConversion(const Cell& cell, const float& chipSize);


	/// <summary>
	/// 値が負の数かどうかを調べる
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="value">調べる値</param>
	/// <returns>値が負の数かどうかのフラグ</returns>
	template <typename T>
	static bool IsValueNegativeCount(T& value);



};

template<typename T>
inline bool FunctionConclusion::IsValueNegativeCount(T& value)
{
	// 値が0以下ならば、true
	if (value < 0.0f)
	{
		return true;
	}


	return false;
}
