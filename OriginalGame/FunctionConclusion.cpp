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

int FunctionConclusion::IinearInterpolationCountCalculation(const Vec2& pos, const Vec2& vec, float heghtSize, float widthSize)
{

	// �ړ��ʂ�0�Ȃ�΁A�����ŏ������I������
	if (vec.x == 0.0f &&
		vec.y == 0.0f)
	{
		return 0;
	}

	// �ŏ��T�C�Y�𒲂ׂ�
	float minSize = heghtSize;

	if (minSize > widthSize)
	{
		minSize = widthSize;
	}

	// �ő�̈ړ��ʂ����߂�
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
	// ���`��ԍ��W
	std::vector<Vec2>iinearInterpolationPos;

	// ���Z���W�v�Z
	Vec2 addVec = Vec2();
	addVec.x = (vec.x / iinearInterpolationCount);
	addVec.y = (vec.y / iinearInterpolationCount);


	// ���`��ԍ��W���v�Z
	for (int i = 1; i < iinearInterpolationCount + 1; i++)
	{
		iinearInterpolationPos.push_back(pos + (addVec * i));
	}

	// ���`��ԍ��W��Ԃ�
	return iinearInterpolationPos;
}

Rect FunctionConclusion::RectangleCalculation(const Vec2& pos, const Vec2& size)
{
	// ��`�ϐ�
	Rect rect = Rect();

	// ��`�����v�Z
	rect.top = pos.y - (size.y * 0.5f);
	rect.bottom = pos.y + (size.y * 0.5f);
	rect.left = pos.x - (size.x * 0.5f);
	rect.right = pos.x + (size.x * 0.5f);

	// �v�Z������`��Ԃ�
	return rect;
}

Rect FunctionConclusion::GetMoveEnableRect(const Rect& rect, const PlatinumLoader::MapInfo& mapInfo, const PlatinumLoader::MapData& mapData)
{
	Rect result;
	// �}�b�v�S�̂��ړ��\�ȏ�Ԃŏ�����
	result.top = 0.0f;
	result.bottom = (mapInfo.mapHeight * mapInfo.chipSize) - (rect.bottom - rect.top);
	result.left = 0.0f;
	result.right = (mapInfo.mapWidth * mapInfo.chipSize) - (rect.right - rect.left) / 2.0f;


	// ����Ώۂ̈ʒu���}�b�v�`�b�v�̃C���f�b�N�X�ɕύX
	const int indexMinX = static_cast<int>(rect.left / mapInfo.chipSize);
	const int indexMaxX = static_cast<int>((rect.right - 1) / mapInfo.chipSize);
	const int indexMinY = static_cast<int>(rect.top / mapInfo.chipSize);
	const int indexMaxY = static_cast<int>((rect.bottom - 1) / mapInfo.chipSize);

	// ������`�F�b�N
	for (int y = indexMinY; y >= 0; y--)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԉ�Y���W�����
			float temp = static_cast<float>(y * mapInfo.chipSize + mapInfo.chipSize);
			if (temp > result.top)
			{
				result.top = temp;
			}
		}
	}
	// �������`�F�b�N
	for (int y = indexMaxY; y < mapInfo.mapHeight; y++)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԏ�Y���W�����
			float temp = static_cast<float>(y * mapInfo.chipSize);
			if (temp < result.bottom)
			{
				result.bottom = temp;
			}
		}
	}
	// ������
	for (int x = indexMinX; x >= 0; x--)
	{
		for (int y = indexMinY; y <= indexMaxY; y++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԉEX���W�����
			float temp = static_cast<float>(x * mapInfo.chipSize + mapInfo.chipSize);
			if (temp > result.left)
			{
				result.left = temp;
			}
		}
	}
	// �E����
	for (int x = indexMaxX; x < mapInfo.mapWidth; x++)
	{
		for (int y = indexMinY; y <= indexMaxY; y++)
		{
			if (mapData.mapData[x][y] == 0)	continue;
			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԍ�X���W�����
			float temp = static_cast<float>(x * mapInfo.chipSize);
			if (temp < result.right)
			{
				result.right = temp;
			}
		}
	}

	return result;
}
