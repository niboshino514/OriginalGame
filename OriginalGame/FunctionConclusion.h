#pragma once
#include "Vec2.h"
#include <cassert>
#include <math.h>
#include <vector>
#include "PlatinumLoader.h"
#include <string>


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


// 交差の構造体
struct Intersection
{
	// 交差したかどうかのフラグ
	bool isFrag = false;

	// 交差座標
	Vec2 pos = Vec2(0.0f, 0.0f);
};

/// <summary>
/// 直線の構造体
/// </summary>
struct Line
{
	float a; // 傾き
	float b; // y軸との交点
};

// 三角形
struct Triangle
{
	// 座標1
	Vec2 pos_1 = Vec2();

	// 座標2
	Vec2 pos_2 = Vec2();

	// 座標3
	Vec2 pos_3 = Vec2();
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
	/// 円の当たり判定
	/// </summary>
	/// <param name="circle1">円1</param>
	/// <param name="cphere2">円2</param>
	/// <returns>円同士が当たったかどうかを返す</returns>
	static bool CircleCollision(const Circle& circle1, const Circle& cphere2);


	/// <summary>
	/// 直線の傾きとY軸との交点を2点から計算する
	/// </summary>
	/// <param name="start">スタート座標</param>
	/// <param name="end">エンド座標</param>
	/// <returns></returns>
	static Line CalculateLine(const Vec2& start, const Vec2& end);

	/// <summary>
	/// 2つの直線交点を計算する
	/// </summary>
	/// <param name="line1">直線1</param>
	/// <param name="line2">直線2</param>
	/// <returns>交差情報(重なっているかどうかのフラグ,重なっている座標)</returns>
	static Intersection CalculateIntersection(const Line& line1, const Line& line2);

	/// <summary>
	/// 三平方の定理
	/// </summary>
	/// <param name="poa1">座標1</param>
	/// <param name="pos2">座標2</param>
	/// <returns>直角三角形の一辺の長さ</returns>
	static float ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2);

	/// <summary>
	/// 四角形情報から中心座標を求める
	/// </summary>
	/// <param name="square">四角形座標</param>
	/// <returns>四角形情報</returns>
	static Vec2 QuadrangularCenter(const Square& square);

	/// <summary>
	/// 四角形の中心点および、中心点からの各頂点距離の最大値を半径とした円の情報を返す
	/// </summary>
	/// <param name="square">四角形情報</param>
	/// <returns>円情報</returns>
	static Circle CalculateQuadrangularCenter(const Square& square);

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
	/// <param name="size">縦横幅</param>
	/// <returns>線形補間数</returns>
	static int IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, const Vec2& size);

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
	static Rect GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapChip& mapInfo, const std::vector<std::vector<int>>& mapData);

	/// <summary>
	/// セルが範囲内かどうか
	/// </summary>
	/// <param name="cell">セル</param>
	/// <param name="maxCell">最大セル</param>
	/// <param name="minCell">最小セル</param>
	/// <returns>セルが範囲内かどうかを返す</returns>
	static bool IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell);

	/// <summary>
	/// 四角形と三角形の当たり判定
	/// </summary>
	/// <param name="square">四角形情報</param>
	/// <param name="triangle">三角形情報</param>
	/// <returns>四角形と三角形の当たり判定</returns>
	static bool CollisionDetectionOfQuadrangleAndTriangle(const Square& square, const Triangle& triangle);

	/// <summary>
	/// 四角形同士が当たっているかどうかを調べる
	/// </summary>
	/// <param name="square1">四角形情報1</param>
	/// <param name="square2">四角形情報2</param>
	/// <returns>四角形同士が当たっているかどうか</returns>
	static bool CollisionDetectionOfQuadrangleAndQuadrangle(const Square& square1, const Square& square2);

	/// <summary>
	/// 線分同士が当たっているかどうかを調べる
	/// </summary>
	/// <param name="start1">スタート座標１</param>
	/// <param name="end1">ゴール座標１</param>
	/// <param name="start2">スタート座標２</param>
	/// <param name="end2">ゴール座標２</param>
	/// <returns>線分同士が当たっているかどうかを調べる</returns>
	static bool CheckLineIntersection(const Vec2& start1, const Vec2& end1, const Vec2& start2, const Vec2& end2);

	/// <summary>
	/// ポリゴンの中にポイント座標が入っているかどうかを調べる
	/// </summary>
	/// <param name="point">座標</param>
	/// <param name="polygon">ポリゴン</param>
	/// <returns>ポリゴンの中にポイント座標が入っているかどうか</returns>
	static bool PointInPolygon(const Vec2& point, const std::vector<Vec2>& polygon);

	/// <summary>
	/// 矩形情報を四角形情報に変換する
	/// </summary>
	/// <param name="rect">矩形情報</param>
	/// <returns>四角形情報</returns>
	static Square RectToSquare(const Rect& rect);


	/// <summary>
	/// エラーメッセージ
	/// </summary>
	/// <param name="errorMessage">エラーメッセージ</param>
	static void ErrorAssertMessage(const std::string& errorMessage);

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
