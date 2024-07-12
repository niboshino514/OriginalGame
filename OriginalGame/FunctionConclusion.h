#pragma once
#include "Vec2.h"

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

};