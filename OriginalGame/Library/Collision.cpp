#include "Collision.h"

bool EvoLib::Collision::SphereCollision(const Sphere& sphere1, const Sphere& sphere2)
{
    // 衝突したかどうか
    bool isCollision = false;


    // 球の衝突判定
    {
        const float dx = sphere1.pos.x - sphere2.pos.x;
        const float dy = sphere1.pos.y - sphere2.pos.y;
        const float dz = sphere1.pos.z - sphere2.pos.z;


        const float dr = (dx * dx) + (dy * dy) + (dz * dz);	// A²＝B²＋C²＋D²

        const float ar = sphere1.radius + sphere2.radius;		// 球の大きさ
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

bool EvoLib::Collision::IsAABBCollisionDetection(const Circle& circle, const Square& square)
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

bool EvoLib::Collision::IsCircleToCircle(const Circle& circle1, const Circle& circle2)
{
	// 衝突したかどうか
	bool isCollision = false;


	// 円の衝突判定
	{
		const float dx = circle1.centerPos.x - circle2.centerPos.x;
		const float dy = circle1.centerPos.y - circle2.centerPos.y;

		const float dr = (dx * dx) + (dy * dy);	// A²＝B²＋C²

		const float ar = circle1.radius + circle2.radius;		// 球の大きさ
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

bool EvoLib::Collision::IsTriangleToSquare(const Triangle& triangle, const Square& square)
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

	//// 四角形と三角形の当たり判定を行う
	//for (int i = 0; i < static_cast<int>(squareStart.size()); i++)
	//{
	//	for (int j = 0; j < static_cast<int>(triangleStart.size()); j++)
	//	{
	//		// 四角形と三角形のライン同士の当たり判定を行う
	//		if (CheckLineIntersection(squareStart[i], squareEnd[i], triangleStart[j], triangleEnd[j]))
	//		{
	//			return true;
	//		}
	//	}
	//}

	//// 三角形の頂点が四角形の内部にあるかの判定
	//for (const auto& vertex : triangleStart)
	//{
	//	if (PointInPolygon(vertex, squareStart))
	//	{
	//		return true;
	//	}
	//}

	//// 四角形の頂点が三角形の内部にあるかの判定
	//for (const auto& vertex : squareStart)
	//{
	//	if (PointInPolygon(vertex, triangleStart))
	//	{
	//		return true;
	//	}
	//}

	return false;
}
