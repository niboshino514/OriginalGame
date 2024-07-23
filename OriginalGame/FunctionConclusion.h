#pragma once
#include "Vec2.h"
#include <cassert>
#include <math.h>
#include <vector>
#include "PlatinumLoader.h"


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

// 矩形
struct Rect
{
	float top = 0.0f;	// 上
	float bottom = 0.0f;// 下
	float left = 0.0f;	// 左
	float right = 0.0f;	// 右
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


	/// <summary>
	/// 線形補間数を求める
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="vec">移動量</param>
	/// <param name="heghtSize">縦幅</param>
	/// <param name="widthSize">横幅</param>
	/// <returns>線形補間数</returns>
	static int IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, float heghtSize, float widthSize);

	/// <summary>
	/// 線形補間座標を求める
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="vec">移動量</param>
	/// <param name="iinearInterpolationCount">線形補間数</param>
	/// <returns>線形補間座標</returns>
	static std::vector<Vec2> IinearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& iinearInterpolationCount);



	/// <summary>
	/// 受け取った座標を中心とした矩形を計算する
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="size">サイズ</param>
	/// <returns>座標を中心とした矩形</returns>
	static Rect RectangleCalculation(const Vec2& pos, const Vec2& size);

	/// <summary>
	/// マップチップの矩形の移動範囲を返す
	/// </summary>
	/// <param name="rect">矩形情報</param>
	/// <param name="mapInfo">マップ情報</param>
	/// <param name="mapData">マップデータ</param>
	/// <returns>移動範囲</returns>
	static Rect GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapInfo& mapInfo, const PlatinumLoader::MapData& mapData);

};

template<typename T>
inline bool FunctionConclusion::IsValueNegativeCount(T& value)
{
	// 値が0以下ならば、false
	if (value > 0.0f)
	{
		return false;
	}


	return true;
}
