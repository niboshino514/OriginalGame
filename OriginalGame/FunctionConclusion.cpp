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