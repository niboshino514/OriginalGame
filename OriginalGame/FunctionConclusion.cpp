#include "FunctionConclusion.h"

bool FunctionConclusion::IsAABBCollisionDetection(const Circle& circle, const Square& square)
{
	// ����
	float line = 0;

	// �㔻��
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

		// �l�p�`�̏�ɏՓ�
		if (circle.radius > line)
		{
			return true;
		}
	}

	// ������
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

		// �l�p�`�̉��ɏՓ�
		if (circle.radius > line)
		{
			return true;
		}
	}

	// �E����
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

		// �l�p�`�̉E�ɏՓ�
		if (circle.radius > line)
		{
			return true;
		}
	}

	// ������
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

		// �l�p�`�̍��ɏՓ�
		if (circle.radius > line)
		{
			return true;
		}
	}

	// �ǂ��ɂ��Փ˂��Ă��Ȃ�
	return false;
}


Cell FunctionConclusion::CoordinateWithCellToConversion(const Vec2& pos, const float& chipSize)
{
	// ���W�ϐ�
	Cell cell = Cell();

	cell.x = pos.x / chipSize;
	cell.y = pos.y / chipSize;

	// �Z����Ԃ�
	return cell;
}

Vec2 FunctionConclusion::CellWithCoordinateToConversion(const Cell& cell, const float& chipSize)
{
	// ���W�ϐ�
	Vec2 pos = Vec2();

	pos.x = (cell.x * chipSize) + (chipSize * 0.5f);
	pos.y = (cell.y * chipSize) + (chipSize * 0.5f);

	// ���W��Ԃ�
	return pos;
}