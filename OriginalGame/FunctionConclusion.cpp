#include "FunctionConclusion.h"

bool FunctionConclusion::IsAABBCollisionDetection(const Circle& circle, const Square& square)
{
	// 長さ
	float line = 0;

	// 上判定
	{
		if (circle.centerPos.x <= square.A.x)
		{
			line = std::powf(square.A.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.A.x - circle.centerPos.x, 2.0f);
		}
		else if (square.B.x <= circle.centerPos.x)
		{
			line = std::powf(square.B.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.B.x - circle.centerPos.x, 2.0f);
		}
		else
		{
			line = std::powf(square.A.y - circle.centerPos.y, 2.0f);
		}

		line = std::sqrtf(line);

		// 四角形の上に衝突
		if (circle.radius > line)
		{
			return true;
		}
	}

	// 下判定
	{
		if (circle.centerPos.x <= square.D.x)
		{
			line = std::powf(square.D.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.D.x - circle.centerPos.x, 2.0f);
		}
		else if (square.C.x <= circle.centerPos.x)
		{
			line = std::powf(square.C.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.C.x - circle.centerPos.x, 2.0f);
		}
		else
		{
			line = std::powf(square.D.y - circle.centerPos.y, 2.0f);
		}

		line = std::sqrtf(line);

		// 四角形の下に衝突
		if (circle.radius > line)
		{
			return true;
		}
	}

	// 右判定
	{
		if (circle.centerPos.y <= square.B.y)
		{
			line = std::powf(square.B.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.B.x - circle.centerPos.x, 2.0f);
		}
		else if (square.C.y <= circle.centerPos.y)
		{
			line = std::powf(square.C.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.C.x - circle.centerPos.x, 2.0f);
		}
		else
		{
			line = std::powf(square.C.x - circle.centerPos.x, 2.0f);
		}

		line = std::sqrtf(line);

		// 四角形の右に衝突
		if (circle.radius > line)
		{
			return true;
		}
	}

	// 左判定
	{
		if (circle.centerPos.y <= square.A.y)
		{
			line = std::powf(square.A.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.A.x - circle.centerPos.x, 2.0f);
		}
		else if (square.D.y <= circle.centerPos.y)
		{
			line = std::powf(square.D.y - circle.centerPos.y, 2.0f)
				+ std::powf(square.D.x - circle.centerPos.x, 2.0f);
		}
		else
		{
			line = std::powf(square.D.x - circle.centerPos.x, 2.0f);
		}

		line = std::sqrtf(line);

		// 四角形の左に衝突
		if (circle.radius > line)
		{
			return true;
		}
	}

	// どこにも衝突していない
	return false;
}


Cell FunctionConclusion::CoordinateWithCellToConversion(const Vec2& pos, const float& chipSize)
{
	// 座標変数
	Cell cell = Cell();

	cell.x = pos.x / chipSize;
	cell.y = pos.y / chipSize;

	// セルを返す
	return cell;
}

Vec2 FunctionConclusion::CellWithCoordinateToConversion(const Cell& cell, const float& chipSize)
{
	// 座標変数
	Vec2 pos = Vec2();

	pos.x = (cell.x * chipSize) + (chipSize * 0.5f);
	pos.y = (cell.y * chipSize) + (chipSize * 0.5f);

	// 座標を返す
	return pos;
}

int FunctionConclusion::IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, float heghtSize, float widthSize)
{

	// 移動量が0ならば、ここで処理を終了する
	if (vec.x == 0.0f &&
		vec.y == 0.0f)
	{
		return 0;
	}

	// 最小サイズを調べる
	float minSize = heghtSize;

	if (minSize > widthSize)
	{
		minSize = widthSize;
	}

	// 最大の移動量を求める
	float maxVec = std::abs(vec.x);

	if (maxVec < std::abs(vec.y))
	{
		maxVec = std::abs(vec.y);
	}


	const int iinearInterpolationCount =
		static_cast<int>(maxVec / minSize);

	return iinearInterpolationCount;
}

std::vector<Vec2> FunctionConclusion::IinearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& iinearInterpolationCount)
{
	// 線形補間座標
	std::vector<Vec2>iinearInterpolationPos;

	// 加算座標計算
	Vec2 addVec = Vec2();
	addVec.x = (vec.x / iinearInterpolationCount);
	addVec.y = (vec.y / iinearInterpolationCount);


	// 線形補間座標を計算
	for (int i = 1; i < iinearInterpolationCount + 1; i++)
	{
		iinearInterpolationPos.push_back(pos + (addVec * i));
	}

	// 線形補間座標を返す
	return iinearInterpolationPos;
}

Rect FunctionConclusion::RectangleCalculation(const Vec2& pos, const Vec2& size)
{
	// 矩形変数
	Rect rect = Rect();

	// 矩形情報を計算
	rect.top = pos.y - (size.y * 0.5f);
	rect.bottom = pos.y + (size.y * 0.5f);
	rect.left = pos.x - (size.x * 0.5f);
	rect.right = pos.x + (size.x * 0.5f);

	// 計算した矩形を返す
	return rect;
}

Rect FunctionConclusion::GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapInfo& mapInfo, const PlatinumLoader::MapData& mapData)
{
	Rect result;
	// マップ全体を移動可能な状態で初期化
	result.top = 0.0f;
	result.bottom = (mapInfo.mapHeight * mapInfo.chipSize) - (rect.bottom - rect.top);
	result.left = 0.0f;
	result.right = (mapInfo.mapWidth * mapInfo.chipSize) - (rect.right - rect.left) / 2.0f;


	// 判定対象の位置をマップチップのインデックスに変更
	const int indexMinX = static_cast<int>(rect.left / mapInfo.chipSize);
	const int indexMaxX = static_cast<int>((rect.right - 1) / mapInfo.chipSize);
	const int indexMinY = static_cast<int>(rect.top / mapInfo.chipSize);
	const int indexMaxY = static_cast<int>((rect.bottom - 1) / mapInfo.chipSize);

	// 上方向チェック
	for (int y = indexMinY; y >= 0; y--)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// 一番最初に見つかったぶつかるマップチップの一番下Y座標を取る
			float temp = static_cast<float>(y * mapInfo.chipSize + mapInfo.chipSize);
			if (temp > result.top)
			{
				result.top = temp;
			}
		}
	}
	// 下方向チェック
	for (int y = indexMaxY; y < mapInfo.mapHeight; y++)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// 一番最初に見つかったぶつかるマップチップの一番上Y座標を取る
			float temp = static_cast<float>(y * mapInfo.chipSize);
			if (temp < result.bottom)
			{
				result.bottom = temp;
			}
		}
	}
	// 左方向
	for (int x = indexMinX; x >= 0; x--)
	{
		for (int y = indexMinY; y <= indexMaxY; y++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// 一番最初に見つかったぶつかるマップチップの一番右X座標を取る
			float temp = static_cast<float>(x * mapInfo.chipSize + mapInfo.chipSize);
			if (temp > result.left)
			{
				result.left = temp;
			}
		}
	}
	// 右方向
	for (int x = indexMaxX; x < mapInfo.mapWidth; x++)
	{
		for (int y = indexMinY; y <= indexMaxY; y++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// 一番最初に見つかったぶつかるマップチップの一番左X座標を取る
			float temp = static_cast<float>(x * mapInfo.chipSize);
			if (temp < result.right)
			{
				result.right = temp;
			}
		}
	}

	return result;
}
