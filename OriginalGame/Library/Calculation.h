#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	// 計算クラス
	class Calculation
	{

	public:

		// 計算(足し算、引き算)
		enum class CalculationType
		{
			Addition,	// 足し算
			Subtraction,// 引き算
		};

		// テンプレート
		template <typename T>
		struct SineCurveData
		{
			// サインカーブフレーム
			int sineCurrentFrame = 0;
			// サインカーブフレームの最大値
			int sineMaxFrame = 0;

			// サインカーブの最大値
			T sineMaxValue = 0;
		};


		// イージングデータ
		struct EasingData
		{
			// 現在のフレーム
			float currentFrame = 0.0f;

			// 移動に掛かる総フレーム
			float totalFrame = 0.0f;

			// 開始座標
			Vec2 startPos = Vec2();

			// 終了座標
			Vec2 endPos = Vec2();
		};

	public:



		/// <summary>
		/// サインカーブ値を求める
		/// </summary>
		/// <typeparam name="T">float,int</typeparam>
		/// <param name="sineCurveData">サインカーブデータ</param>
		/// <param name="isAbs">ABSを使用するかどうか(未使用：-1~1 使用：0~1)</param>
		/// <returns></returns>
		template <typename T>
		static T SineCurve(SineCurveData<T>& sineCurveData, bool isAbs = false);

		/// <summary>
		/// 角度を基準にサインカーブ値を求める
		/// </summary>
		/// <typeparam name="T">float,int</typeparam>
		/// <param name="sineCurveData">サインカーブデータ</param>
		/// <param name="angle">角度</param>
		/// <param name="isAbs">ABSを使用するかどうか(未使用：-1~1 使用：0~1)</param>
		/// <param name="isHorizontally">横方向にサインカーブするかどうか</param>
		/// <returns>角度を基準のサインカーブ値</returns>
		template <typename T>
		static Vec2 SineCurveAngle(SineCurveData<T>& sineCurveData, const float& angle, bool isAbs = false, const bool& isHorizontally = false);


		/// <summary>
		/// 2Dの振動
		/// </summary>
		/// <param name="currentFrame">現在のフレーム</param>
		/// <param name="maxFrame">振動させる最大フレーム</param>
		/// <param name="vibrationWidth">振動幅</param>
		/// <returns>2Dの振動値</returns>
		static Vec2 VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth);


		/// <summary>
		/// ループ番号
		/// </summary>
		/// <param name="minNumber">最小値</param>
		/// <param name="maxNumber">最大値</param>
		/// <param name="currentNumber">現在の値</param>
		/// <param name="isInverseLoop">逆ループするかどうか</param>
		/// <returns>変換された番号</returns>
		static int SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop = false);


		/// <summary>
		/// グラフィックのサイズを取得
		/// </summary>
		/// <param name="graphHandle">グラフィックハンドル</param>
		/// <param name="graphMagnificationRate">グラフィックの拡大率</param>
		/// <returns>グラフィックのサイズ</returns>
		static Vec2 GetGraphSize_EvoLib(std::vector<int>graphHandle, const double& graphMagnificationRate);



		/// <summary>
		/// 画像サイズを基準として、個数分座標を均等に配置する
		/// </summary>
		/// <param name="graphSize">グラフのサイズ</param>
		/// <param name="basePos">基準座標</param>
		/// <param name="num">個数</param>
		/// <param name="graphInterval">グラフィック同士の幅</param>
		/// <param name="isHorizontalSort">横並びかどうか</param>
		/// <returns>座標</returns>
		static std::vector<Vec2>GraphEqualization(const Vec2& graphSize, const Vec2& basePos, const int& num, const Vec2& graphInterval, const bool& isHorizontalSort);

	
	


		/// <summary>
		/// ある座標を基準として、個数分座標を均等に配置する
		/// </summary>
		/// <param name="basePos">基準座標</param>
		/// <param name="num">個数</param>
		/// <param name="interval">間隔</param>
		/// <returns>座標</returns>
		static std::vector<Vec2>PosEqualization(const Vec2& basePos, const int& num, const Vec2& interval);



		/// <summary>
		/// ターゲットの範囲内かどうかの判定
		/// </summary>
		/// <param name="currentValue">現在の値</param>
		/// <param name="targetValue">ターゲットの値</param>
		/// <param name="addSubValue">加減算値</param>
		/// <returns>ターゲットの範囲内かどうか</returns>
		static bool IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue);


		/// <summary>
		/// 角度を加減算して、360度を超えたり、0度以下にならないように計算を行う
		/// </summary>
		/// <param name="currentAngle">現在の角度</param>
		/// <param name="addSubValue">加減算値</param>
		/// <param name="calculation">足し算か引き算かどうか</param>
		/// <param name="isInteger">整数値かどうか</param>
		/// <param name="isAdjustment">調整値を加えるかどうか</param>
		/// <returns>加減算を終えた角度を返す</returns>
		static float AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, EvoLib::Calculation::CalculationType calculation, bool isInteger = true, bool isAdjustment = true);


		/// <summary>
		/// 三平方の定理
		/// </summary>
		/// <param name="poa1">座標1</param>
		/// <param name="pos2">座標2</param>
		/// <returns>直角三角形の一辺の長さ</returns>
		static float ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2);

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
		static int InearInterpolationCount(const Vec2& pos, const Vec2& vec, const Vec2& size);

		/// <summary>
		/// 線形補間座標を求める
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="vec">移動量</param>
		/// <param name="iinearInterpolationCount">線形補間数</param>
		/// <returns>線形補間座標</returns>
		static std::vector<Vec2> InearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& iinearInterpolationCount);

		/// <summary>
		/// 矩形の移動範囲を計算
		/// </summary>
		/// <param name="rect">自身の矩形情報</param>
		/// <param name="maxCell">セルの最大数</param>
		/// <param name="cellSize">セルサイズ</param>
		/// <param name="cellData">二次元セルデータ</param>
		/// <param name="unusedCellNumber">地面セル番号</param>
		/// <returns>移動範囲</returns>
		static Rect CalculateRectangleMovementRange(const Rect& rect, const Cell& maxCell, const float& cellSize, const std::vector<std::vector<int>>& cellData, const std::vector<int> groundCellNumber);


		/// <summary>
		/// セルが範囲内かどうか
		/// </summary>
		/// <param name="cell">調べるセル</param>
		/// <param name="maxCell">最大セル</param>
		/// <param name="minCell">最小セル</param>
		/// <returns>セルが範囲内かどうかを返す</returns>
		static bool IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell);

		/// <summary>
		/// 上限下限値を設定
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="x">数値</param>
		/// <param name="low">下限</param>
		/// <param name="high">上限</param>
		/// <returns></returns>
		template<typename T>
		static T Clamp(T x, T low, T high);


		// ターゲットに向かう移動量を求める
		static Vec2 TargetMoveValue(const Vec2& pos, const Vec2& targetPos, const float& moveSpeed);

		/// <summary>
		/// 衛星軌道(中心点を軸に、角度を参照して回転する座標を計算する)
		/// </summary>
		/// <param name="centerPos">中心座標</param>
		/// <param name="radian">ラジアン</param>
		/// <param name="circleRadius">円の半径</param>
		/// <returns>アングルに対応した座標を返す</returns>
		static Vec2 SatelliteTrajectory(const Vec2& centerPos, const float& radian, const float& circleRadius);

		/// <summary>
		/// 頂点座標を回転させる
		/// </summary>
		/// <param name="rotaCenterPos">回転の中心座標</param>
		/// <param name="radian">回転させるラジアン</param>
		/// <param name="vertexPos">頂点座標</param>
		/// <returns></returns>
		static std::vector<Vec2> VertexCoordinateRotation(const Vec2& rotaCenterPos, const float& radian, const std::vector<Vec2> vertexPos);

		/// <summary>
		/// Atan2(ターゲット方向に向かう、ラジアンを求める)
		/// </summary>
		/// <param name="targetPos">ターゲット座標</param>
		/// <param name="startPos">スタート座標</param>
		/// <returns>ターゲット座標を向くラジアン</returns>
		static float Atan2_Lib(const Vec2& targetPos, const Vec2& startPos);

		/// <summary>
		/// 二点の座標空法線ベクトルを求める
		/// </summary>
		/// <param name="p1">座標1</param>
		/// <param name="p2">座標2</param>
		/// <returns>二点の座標の法線ベクトルを求める</returns>
		static Vec2 NormalVector(const Vec2& p1, const Vec2& p2);

		/// <summary>
		/// 反射ベクトルを求める
		/// </summary>
		/// <param name="vec">移動量</param>
		/// <param name="p1">壁となる座標1</param>
		/// <param name="p2">壁となる座標2</param>
		/// <returns>反射ベクトル</returns>
		static Vec2 ReflectVector(const Vec2& vec, const Vec2& p1, const Vec2& p2);


		/// <summary>
		/// 角度を指定した個数分、均等に分割する
		/// </summary>
		/// <param name="divisionNumber">分割数</param>
		/// <param name="baseAngle">基準とする座標</param>
		/// <returns>分割された角度</returns>
		static std::vector<float> AngleDivision(const int& divisionNumber, const float& baseAngle = 0.0f);


		/// <summary>
		/// イージング移動
		/// </summary>
		/// <param name="easingData">イージングデータ</param>
		/// <returns>現在の座標</returns>
		static Vec2 EasingInOutSine(EasingData& easingData);


	};
	template<typename T>
	inline T Calculation::SineCurve(SineCurveData<T>& sineCurveData, bool isAbs)
	{
		// サインフレームを増やす
		sineCurveData.sineCurrentFrame++;
		// サインフレームが最大値を超えたらリセット
		sineCurveData.sineCurrentFrame %= sineCurveData.sineMaxFrame;

		// サインカーブの値
		T sineValue = {};

		// 絶対値を取るかどうか
		if (isAbs)
		{
			sineValue = static_cast<float>(abs(sin(DX_PI * 2.0f / sineCurveData.sineMaxFrame * sineCurveData.sineCurrentFrame) * sineCurveData.sineMaxValue));
		}
		else
		{
			sineValue = static_cast<float>(sin(DX_PI * 2.0f / sineCurveData.sineMaxFrame * sineCurveData.sineCurrentFrame) * sineCurveData.sineMaxValue);
		}

		// サイン値を返す
		return sineValue;
	}
	template<typename T>
	inline Vec2 Calculation::SineCurveAngle(SineCurveData<T>& sineCurveData, const float& angle, bool isAbs, const bool& isHorizontally)
	{
		// サインフレームを増やす
		sineCurveData.sineCurrentFrame++;
		// サインフレームが最大値を超えたらリセット
		sineCurveData.sineCurrentFrame %= sineCurveData.sineMaxFrame;

		// サインカーブの値
		T sineValue = {};

		// 絶対値を取るかどうか
		if (isAbs)
		{
			sineValue = static_cast<float>(abs(sin(DX_PI * 2.0f / sineCurveData.sineMaxFrame * sineCurveData.sineCurrentFrame) * sineCurveData.sineMaxValue));
		}
		else
		{
			sineValue = static_cast<float>(sin(DX_PI * 2.0f / sineCurveData.sineMaxFrame * sineCurveData.sineCurrentFrame) * sineCurveData.sineMaxValue);
		}



		// 角度をラジアンに変換
		float radian = angle * (DX_PI_F / 180.0f);





		Vec2 rotated = Vec2();

		if(isHorizontally)
		{
			rotated.x = sineValue * cos(radian);
			rotated.y = sineValue * sin(radian);
		}
		else
		{
			rotated.x = sineValue * sin(-radian);
			rotated.y = sineValue * cos(-radian);
		}


		return rotated;
	}
	template<typename T>
	inline bool Calculation::IsValueNegativeCount(T& value)
	{
		// 値が0以下ならば、false
		if (value > 0.0f)
		{
			return false;
		}

		return true;
	}
	template<typename T>
	inline T Calculation::Clamp(T x, T low, T high)
	{
		if (low >= x)
		{
			return low;
		}
		else if (x >= high)
		{
			return high;
		}

		return x;
	}
}