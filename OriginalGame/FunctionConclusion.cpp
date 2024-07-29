#include "FunctionConclusion.h"
#include "Sort.h"

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

	cell.x = static_cast<int>(pos.x / chipSize);
	cell.y = static_cast<int>(pos.y / chipSize);

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

bool FunctionConclusion::CircleCollision(const Circle& circle1, const Circle& cphere2)
{
	// 衝突したかどうか
	bool isCollision = false;


	// 円の衝突判定
	{
		const float dx = circle1.centerPos.x - cphere2.centerPos.x;
		const float dy = circle1.centerPos.y - cphere2.centerPos.y;



		const float dr = (dx * dx) + (dy * dy);	// A²＝B²＋C²

		const float ar = circle1.radius + cphere2.radius;		// 球の大きさ
		const float dl = ar * ar;

		if (dr <= dl)
		{
			// 衝突している
			isCollision = true;
		}
	}

	// 衝突判定を返す
	return isCollision;
}

Line FunctionConclusion::CalculateLine(const Vec2& start, const Vec2& end)
{
	// 直線変数
	Line line = Line();

	{
		// 傾きを求める
		line.a = (end.y - start.y) / (end.x - start.x);
		// Y軸とこ交点を求める
		line.b = start.y - line.a * start.x;
	}

	// 直線情報を返す
	return line;
}

Intersection FunctionConclusion::CalculateIntersection(const Line& line1, const Line& line2)
{
	// 交差の構造体変数
	Intersection intersection;


	// 傾きが同じ場合、交点は存在しない
	if (line1.a == line2.a)
	{
		return intersection;
	}

	// 交点が存在するので、フラグをtrueにする
	intersection.isFrag = true;

	// 交点のx座標を計算
	intersection.pos.x = (line2.b - line1.b) / (line1.a - line2.a);

	// 交点のy座標を計算
	intersection.pos.y = line1.a * intersection.pos.x + line1.b;


	// 交差情報を返す
	return intersection;
}

float FunctionConclusion::ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2)
{
	// 一辺の長さ
	float length = 0.0f;

	// 2乗された、一辺の長さを求める
	length = std::powf(poa1.x - pos2.x, 2.0f)
		+ std::powf(poa1.y - pos2.y, 2.0f);

	// 平方根を計算
	length = std::sqrtf(length);

	// 一辺の長さを返す
	return length;
}

Vec2 FunctionConclusion::QuadrangularCenter(const Square& square)
{
	// 直線を2点から計算
	const Line line1 = CalculateLine(square.A, square.C);
	const Line line2 = CalculateLine(square.B, square.D);

	// 2つの直線交点情報を取得
	const Intersection intersection = CalculateIntersection(line1, line2);

	// 中心点
	Vec2 centerPos = Vec2();

	// 直線が交差してなければ、ここで処理を終了する
	if (!intersection.isFrag)
	{
		return centerPos;
	}

	// 中心点を代入
	centerPos = intersection.pos;

	// 中心座標を返す
	return centerPos;
}

Circle FunctionConclusion::CalculateQuadrangularCenter(const Square& square)
{
	// 直線を2点から計算
	const Line line1 = CalculateLine(square.A, square.C);
	const Line line2 = CalculateLine(square.B, square.D);

	// 2つの直線交点情報を取得
	const Intersection intersection = CalculateIntersection(line1, line2);

	// 円情報
	Circle circle;

	// 直線が交差してなければ、ここで処理を終了する
	if (!intersection.isFrag)
	{
		return circle;
	}

	// 距離
	std::vector<float>range;

	// 値を代入
	range.push_back(ThreeSquareTheorem(square.A, intersection.pos));
	range.push_back(ThreeSquareTheorem(square.B, intersection.pos));
	range.push_back(ThreeSquareTheorem(square.C, intersection.pos));
	range.push_back(ThreeSquareTheorem(square.D, intersection.pos));

	// 数値が小さい順になるよう、ソートを行う
	Sort::SelectionSort(range, static_cast<int>(range.size()));



	// 円の中心点を代入
	circle.centerPos = intersection.pos;
	// 半径を代入
	circle.radius = range.back();

	// 円情報を返す
	return circle;
}

int FunctionConclusion::IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, const Vec2& size)
{

	// 移動量が0ならば、ここで処理を終了する
	if (vec.x == 0.0f &&
		vec.y == 0.0f)
	{
		return 0;
	}

	// 最小サイズを調べる
	float minSize = size.x;

	if (minSize > size.x)
	{
		minSize = size.y;
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
		iinearInterpolationPos.push_back(pos + (addVec * static_cast<float>(i)));
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

Rect FunctionConclusion::GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapChip& mapInfo, const std::vector<std::vector<int>>& mapData)
{
	Rect result = Rect();

	// マップ全体を移動可能な状態で初期化
	result.top = 0.0f;
	result.bottom = (mapInfo.mapHeight * mapInfo.chipSize) - (rect.bottom - rect.top) / 2.0f;
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
			if (!IsCellRange(Cell(x, y), Cell(mapInfo.mapWidth, mapInfo.mapWidth), Cell(0, 0)))
			{
				return Rect();
			}

			if (mapData[x][y] == 0 ||
				mapData[x][y] != 1)
			{
				continue;
			}

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
			if (!IsCellRange(Cell(x, y), Cell(mapInfo.mapWidth, mapInfo.mapWidth), Cell(0, 0)))
			{
				return Rect();
			}

			if (mapData[x][y] == 0 ||
				mapData[x][y] != 1)
			{
				continue;
			}

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
			if (!IsCellRange(Cell(x, y), Cell(mapInfo.mapWidth, mapInfo.mapWidth), Cell(0, 0)))
			{
				return Rect();
			}

			if (mapData[x][y] == 0 ||
				mapData[x][y] != 1)
			{
				continue;
			}

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
			if (!IsCellRange(Cell(x, y), Cell(mapInfo.mapWidth, mapInfo.mapWidth), Cell(0, 0)))
			{
				return Rect();
			}

			if (mapData[x][y] == 0 ||
				mapData[x][y] != 1)
			{
				continue;
			}

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

bool FunctionConclusion::IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell)
{

	// セルが最大セルを超えているかどうかを確認する
	if (cell.x >= maxCell.x ||
		cell.y >= maxCell.y)
	{
		// 超えていたらfalseを返す
		return false;
	}

	// セルが最小セルを下回っているかどうかを確認する
	if (cell.x < minCell.x ||
		cell.y < minCell.y)
	{
		// 下回っていたらfalseを返す
		return false;
	}

	// trueを返す
	return true;
}

bool FunctionConclusion::CollisionDetectionOfQuadrangleAndTriangle(const Square& square, const Triangle& triangle)
{
	// 四角形情報
	std::vector<Vec2> squareStart;
	std::vector<Vec2> squareEnd;
	{
		squareStart.push_back(square.A);
		squareEnd.push_back(square.B);

		squareStart.push_back(square.B);
		squareEnd.push_back(square.C);

		squareStart.push_back(square.C);
		squareEnd.push_back(square.D);

		squareStart.push_back(square.D);
		squareEnd.push_back(square.A);
	}

	std::vector<Vec2> triangleStart;
	std::vector<Vec2> triangleEnd;
	{
		triangleStart.push_back(triangle.pos_1);
		triangleEnd.push_back(triangle.pos_2);

		triangleStart.push_back(triangle.pos_2);
		triangleEnd.push_back(triangle.pos_3);

		triangleStart.push_back(triangle.pos_3);
		triangleEnd.push_back(triangle.pos_1);
	}

	// 四角形と三角形の当たり判定を行う
	for (int i = 0; i < static_cast<int>(squareStart.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(triangleStart.size()); j++)
		{
			// 四角形と三角形のライン同士の当たり判定を行う
			if (CheckLineIntersection(squareStart[i], squareEnd[i], triangleStart[j], triangleEnd[j]))
			{
				return true;
			}
		}
	}

	// 三角形の頂点が四角形の内部にあるかの判定
	for (const auto& vertex : triangleStart)
	{
		if (PointInPolygon(vertex, squareStart))
		{
			return true;
		}
	}

	// 四角形の頂点が三角形の内部にあるかの判定
	for (const auto& vertex : squareStart)
	{
		if (PointInPolygon(vertex, triangleStart))
		{
			return true;
		}
	}

	return false;
}

bool FunctionConclusion::CollisionDetectionOfQuadrangleAndQuadrangle(const Square& square1, const Square& square2)
{
	// 四角形情報1
	std::vector<Vec2> square1Start;
	std::vector<Vec2> square1End;
	{
		square1Start.push_back(square1.A);
		square1End.push_back(square1.B);

		square1Start.push_back(square1.B);
		square1End.push_back(square1.C);

		square1Start.push_back(square1.C);
		square1End.push_back(square1.D);

		square1Start.push_back(square1.D);
		square1End.push_back(square1.A);
	}

	// 四角形情報2
	std::vector<Vec2> square2Start;
	std::vector<Vec2> square2End;
	{
		square2Start.push_back(square2.A);
		square2End.push_back(square2.B);

		square2Start.push_back(square2.B);
		square2End.push_back(square2.C);

		square2Start.push_back(square2.C);
		square2End.push_back(square2.D);

		square2Start.push_back(square2.D);
		square2End.push_back(square2.A);
	}

	// 四角形と四角形の当たり判定を行う
	for (int i = 0; i < static_cast<int>(square1Start.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(square2Start.size()); j++)
		{
			// 四角形と四角形のライン同士の当たり判定を行う
			if (CheckLineIntersection(square1Start[i], square1End[i], square2Start[j], square2End[j]))
			{
				return true;
			}
		}
	}

	// 四角形1の頂点が四角形2の内部にあるかの判定
	for (const auto& vertex : square1Start)
	{
		if (PointInPolygon(vertex, square2Start))
		{
			return true;
		}
	}

	// 四角形2の頂点が四角形1の内部にあるかの判定
	for (const auto& vertex : square2Start)
	{
		if (PointInPolygon(vertex, square1Start))
		{
			return true;
		}
	}


	return false;
}

bool FunctionConclusion::CheckLineIntersection(const Vec2& start1, const Vec2& end1, const Vec2& start2, const Vec2& end2)
{
	float s1_x = end1.x - start1.x, s1_y = end1.y - start1.y;
	float s2_x = end2.x - start2.x, s2_y = end2.y - start2.y;

	float s, t;

	s = (-s1_y * (start1.x - start2.x) + s1_x * (start1.y - start2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (start1.y - start2.y) - s2_y * (start1.x - start2.x)) / (-s2_x * s1_y + s1_x * s2_y);

	return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

bool FunctionConclusion::PointInPolygon(const Vec2& point, const std::vector<Vec2>& polygon)
{
	bool inside = false;

	const int numVertices = static_cast<int>(polygon.size());

	for (int i = 0, j = numVertices - 1; i < numVertices; j = i++) {
		// ポリゴンの辺を定義する2点
		const Vec2& vertex1 = polygon[i];
		const Vec2& vertex2 = polygon[j];

		// 点の水平線がこの辺と交差するかどうかをチェック
		bool intersect = ((vertex1.y > point.y) != (vertex2.y > point.y)) &&
			(point.x < (vertex2.x - vertex1.x) * (point.y - vertex1.y) / (vertex2.y - vertex1.y) + vertex1.x);

		if (intersect) {
			inside = !inside;
		}
	}
	return inside;
}

Square FunctionConclusion::RectToSquare(const Rect& rect)
{
	// 四角形情報
	Square square = Square();

	// 四角形情報を計算
	{
		square.A = Vec2(rect.left, rect.top);
		square.B = Vec2(rect.right, rect.top);
		square.C = Vec2(rect.right, rect.bottom);
		square.D = Vec2(rect.left, rect.bottom);
	}

	// 四角形情報を返す
	return square;
}
