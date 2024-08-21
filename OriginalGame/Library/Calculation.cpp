#include "Calculation.h"

int EvoLib::Calculation::SineCurve(int currentFrame, int maxFrame, int maxValue, bool isAbs)
{
    // ABS���g�p����ꍇ�A�ő�t���[�����Q�{����
    if (isAbs)
    {
        maxFrame *= 2;
    }

    // ���݂̃t���[�����ő�t���[���Ŋ���
    currentFrame %= maxFrame;

    // �T�C���l�����߂�
    int sinValue = static_cast<int>(sin(DX_PI_F * 2.0f / maxFrame * currentFrame) * maxValue);

    // ABS���g�p����ꍇ�A��Βl�����߂�
    if (isAbs)
    {
        // ��Βl�����߂�
        sinValue = std::abs(sinValue);
    }

    // �T�C���l��Ԃ�
    return sinValue;
}

Vec2 EvoLib::Calculation::VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth)
{
    // ���݂̃t���[���ɍő�t���[�����猻�݂̃t���[�����������l��������
    currentFrame = maxFrame - currentFrame;

    // ���݂̃t���[�����O�ȉ��������ꍇ�A0����
    if (currentFrame <= 0)
    {
        currentFrame = 0;
    }


    // x,y���}�C�i�X���邩���Ȃ���
    int x = GetRand(1);
    int y = GetRand(1);

    // �p�[�Z���e�[�W�����߂�
    const float percentage = static_cast<float>(EvoLib::Convert::PercentageRate(static_cast<float>(maxFrame), static_cast<float>(currentFrame)));

    // ������
    const float decimal = percentage * 0.01f;

    // �U����(�v�Z)
    int afterVibrationWidth = static_cast<int>(vibrationWidth * decimal);


    Vec2 vibrationPos = {};


    // �����_���l�ō��W��ϊ�����
    vibrationPos.x = static_cast<float>(GetRand(afterVibrationWidth));
    vibrationPos.y = static_cast<float>(GetRand(afterVibrationWidth));


    // x or y��0�̎��A�ʒu���}�C�i�X����
    if (x == 0)vibrationPos.x *= -1;
    if (y == 0)vibrationPos.y *= -1;

    // �U���l��Ԃ�
    return vibrationPos;
}

int EvoLib::Calculation::SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop)
{
    // ���݂̔ԍ�����
    int number = currentNumber;

    // �t���[�v���邩�ǂ����ŏ�����ύX����
    if (isInverseLoop)
    {
        // ���݂̑I�����ڂ����ɂ��炷(�t���[�v����)
        number = (currentNumber + (maxNumber - 1)) % maxNumber;
    }
    else
    {
        // ���݂̑I�����ڂ�����ɂ��炷(���[�v����)
        number = (currentNumber + 1) % maxNumber;
    }

    // ���߂��ԍ���Ԃ�
    return number;
}

std::vector<Vec2> EvoLib::Calculation::GraphicWidthCoordinate(const int& handle, const int& num, const float& graphScale, const Vec2& centerPos, const Vec2& distancePos, bool isSide)
{
    // ���W
    std::vector<Vec2> pos;

    // �O���t�B�b�N�̌����A���W�ϐ��𑝂₷
    pos.resize(num);

    // �ꎞ�I�ȃO���t�B�b�N�̃T�C�Y�i�[�ϐ�
    int sizeX = 0;
    int sizeY = 0;

    // �O���t�B�b�N�T�C�Y���擾
    GetGraphSize(handle, &sizeX, &sizeY);

    // �O���t�B�b�N�̃T�C�Y�i�[�ϐ�
    Vec2 size;

    // �T�C�Y����
    size.x = static_cast<float>(sizeX);
    size.y = static_cast<float>(sizeY);


    // �����O���t�B�b�N�T�C�Y��������
    size *= graphScale;
    size += distancePos;

    // ����W
    Vec2 basePos = {};

    // ��������
    const bool isEvenNumber = (num % 2 == 0);

    // ����������Ńx�[�X���W��ύX����
    if (isEvenNumber)
    {
        basePos = centerPos;

        basePos -= (size * 0.5);
    }
    else
    {
        basePos = centerPos;
    }


    int n = num;

    if (isEvenNumber)
    {
        n--;
    }

    n /= 2;



    // �ꎞ�ۑ����W
    Vec2 savePos;

    // �i���o�[�ϐ�
    int number = 0;

    // �J�E���g�ϐ�
    int count = 0;

    for (int i = 0; i < num; i++)
    {
        // �ꎞ�ۑ����W�Ƀx�[�X���W����
        savePos = basePos;

        // �i���o�[�ϐ�������
        number = 0;

        // i������������ő��������W��ύX����
        if (i % 2 == 0)
        {
            if (isSide)
            {
                savePos.x -= size.x * count;
            }
            else
            {
                savePos.y -= size.y * count;
            }

            number -= count;
        }
        else
        {
            if (isSide)
            {
                savePos.x += size.x * count;
            }
            else
            {
                savePos.y += size.y * count;
            }

            number += count;
        }

        // ���W���
        pos[number + n] = savePos;

        if (i % 2 == 0)
        {
            count++;
        }
    }

    // �l��Ԃ�
    return pos;
}

std::vector<Vec2> EvoLib::Calculation::SortCoordinateEqually(const int& num, const Vec2& centerPos, const Vec2& distancePos, bool isSide)
{
    // ���W
    std::vector<Vec2> pos;

    // ���W�̗v�f�𑝂₷
    pos.resize(num);

    // �T�C�Y�ϐ��ɕ��Ƃ�����W��������
    const Vec2 size = distancePos;

    // ����W
    Vec2 basePos = {};

    // ��������
    const bool isEvenNumber = (num % 2 == 0);

    // ����������Ńx�[�X���W��ύX����
    if (isEvenNumber)
    {
        basePos = centerPos;

        basePos -= (size * 0.5);
    }
    else
    {
        basePos = centerPos;
    }


    int n = num;

    if (isEvenNumber)
    {
        n--;
    }

    n /= 2;



    // �ꎞ�ۑ����W
    Vec2 savePos;

    // �i���o�[�ϐ�
    int number = 0;
    // �J�E���g�ϐ�
    int count = 0;

    for (int i = 0; i < num; i++)
    {
        // �ꎞ�ۑ����W�Ƀx�[�X���W����
        savePos = basePos;

        // �i���o�[�ϐ�������
        number = 0;

        // i������������ňꎞ�ۑ����W�̒l��ύX����
        if (i % 2 == 0)
        {
            if (isSide)
            {
                savePos.x -= size.x * count;
            }
            else
            {
                savePos.y -= size.y * count;
            }

            number -= count;
        }
        else
        {
            if (isSide)
            {
                savePos.x += size.x * count;
            }
            else
            {
                savePos.y += size.y * count;
            }

            number += count;
        }

        // ���W���
        pos[number + n] = savePos;

        if (i % 2 == 0)
        {
            count++;
        }
    }

    // �l��Ԃ�
    return pos;
}

bool EvoLib::Calculation::IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue)
{
    // �l��␳
    if (currentValue <= (targetValue + addSubValue) &&
        currentValue >= (targetValue - addSubValue))
    {
        // true�����Ԃ�
        return true;
    }

    // false�����Ԃ�
    return false;
}

float EvoLib::Calculation::AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, EvoLib::Calculation::CalculationType calculation, bool isInteger, bool isAdjustment)
{
    // ���݂̊p�x����
    float angle = currentAngle;

    // �����l
    float adjustmentValue = 0.0f;

    // ���Z�̏ꍇ�A360�x�𒴂��Ȃ��悤�ɂ���
    if (calculation == CalculationType::Addition)
    {
        // �p�x�ɉ����Z�l��������
        angle += addSubValue;

        if (angle >= 360.0f)
        {
            // �A���O����0�x�ɂ���
            angle = 0.0f;

            // �����l�Ɍ��݂̃A���O������360.0f�������������߂�
            adjustmentValue = angle - 360.0f;
        }
    }
    // ���Z�̏ꍇ�A0�x�������Ȃ��悤�ɂ���
    else
    {
        // �p�x�������Z�l�����炷
        angle -= addSubValue;

        if (angle <= 0.0f)
        {
            // �����l�Ɍ��݂̃A���O����������
            adjustmentValue = angle;

            // �A���O����360�x�ɂ���
            angle = 360.0f;
        }
    }

    // �����l���g�p����ꍇ�A�����l�����Z���s��
    if (isAdjustment)
    {
        // �A���O���𒲐��l���A���Z����
        angle += adjustmentValue;
    }

    // �����l�t���O��true�Ȃ�ΐ����l��Ԃ�
    if (isInteger)
    {
        // �����l�֕ϊ�
        const int integerNum = static_cast<int>(angle);

        // ���߂��p�x��Ԃ�
        return static_cast<float>(integerNum);
    }


    // ���߂��p�x��Ԃ�
    return angle;
}

float EvoLib::Calculation::ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2)
{
    // ��ӂ̒���
    float length = 0.0f;

    // 2�悳�ꂽ�A��ӂ̒��������߂�
    length = std::powf(poa1.x - pos2.x, 2.0f)
        + std::powf(poa1.y - pos2.y, 2.0f);

    // ���������v�Z
    length = std::sqrtf(length);

    // ��ӂ̒�����Ԃ�
    return length;
}

int EvoLib::Calculation::InearInterpolationCount(const Vec2& pos, const Vec2& vec, const Vec2& size)
{
    // �ړ��ʂ�0�Ȃ�΁A�����ŏ������I������
    if (vec.x == 0.0f &&
        vec.y == 0.0f)
    {
        return 0;
    }

    // �ŏ��T�C�Y�𒲂ׂ�
    float minSize = size.x;

    if (minSize > size.x)
    {
        minSize = size.y;
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

std::vector<Vec2> EvoLib::Calculation::InearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& inearInterpolationCount)
{
    // ���`��ԍ��W
    std::vector<Vec2>inearInterpolationPos;

    // ���Z���W�v�Z
    Vec2 addVec = Vec2();
    addVec.x = (vec.x / inearInterpolationCount);
    addVec.y = (vec.y / inearInterpolationCount);


    // ���`��ԍ��W���v�Z
    for (int i = 1; i < inearInterpolationCount + 1; i++)
    {
        inearInterpolationPos.push_back(pos + (addVec * static_cast<float>(i)));
    }

    // ���`��ԍ��W��Ԃ�
    return inearInterpolationPos;
}

Rect EvoLib::Calculation::CalculateRectangleMovementRange(const Rect& rect, const Cell& maxCell, const float& cellSize, const std::vector<std::vector<int>>& cellData, const std::vector<int> groundCellNumber)
{
    // ���ʂ̋�`
    Rect result = Rect();

	// �}�b�v�S�̂��ړ��\�ȏ�Ԃŏ�����
	result.top = 0.0f;
	result.bottom = (maxCell.y * cellSize) - (rect.bottom - rect.top) / 2.0f;
	result.left = 0.0f;
	result.right = (maxCell.x * cellSize) - (rect.right - rect.left) / 2.0f;


	// ����Ώۂ̈ʒu���}�b�v�`�b�v�̃C���f�b�N�X�ɕύX
	const int indexMinX = static_cast<int>(rect.left / cellSize);
	const int indexMaxX = static_cast<int>((rect.right - 1) / cellSize);
	const int indexMinY = static_cast<int>(rect.top / cellSize);
	const int indexMaxY = static_cast<int>((rect.bottom - 1) / cellSize);

	// ������`�F�b�N
	for (int y = indexMinY; y >= 0; y--)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (!IsCellRange(Cell(x, y), Cell(maxCell.x, maxCell.x), Cell(0, 0)))
			{
				return Rect();
			}

            if (cellData[x][y] == 0)
            {
				continue;
			}

            bool isContinue = true;

            // �n�ʃZ���ԍ�������ꍇ�A���̃Z���ԍ��͖�������
            for (auto& unused : groundCellNumber)
            {
				if (cellData[x][y] == unused)
				{
                    isContinue = false;
				}
            }

            if (isContinue)
            {
                continue;
            }



			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԉ�Y���W�����
			float temp = static_cast<float>(y * cellSize + cellSize);
			if (temp > result.top)
			{
				result.top = temp;
			}
		}
	}
	// �������`�F�b�N
	for (int y = indexMaxY; y < maxCell.y; y++)
	{
		for (int x = indexMinX; x <= indexMaxX; x++)
		{
			if (!IsCellRange(Cell(x, y), Cell(maxCell.x, maxCell.x), Cell(0, 0)))
			{
				return Rect();
			}

            if (cellData[x][y] == 0)
            {
                continue;
            }

            bool isContinue = true;

            // �n�ʃZ���ԍ�������ꍇ�A���̃Z���ԍ��͖�������
            for (auto& unused : groundCellNumber)
            {
                if (cellData[x][y] == unused)
                {
                    isContinue = false;
                }
            }

            if (isContinue)
            {
                continue;
            }


			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԏ�Y���W�����
			float temp = static_cast<float>(y * cellSize);
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
			if (!IsCellRange(Cell(x, y), maxCell, Cell(0, 0)))
			{
				return Rect();
			}

            if (cellData[x][y] == 0)
            {
                continue;
            }

            bool isContinue = true;

            // �n�ʃZ���ԍ�������ꍇ�A���̃Z���ԍ��͖�������
            for (auto& unused : groundCellNumber)
            {
                if (cellData[x][y] == unused)
                {
                    isContinue = false;
                }
            }

            if (isContinue)
            {
                continue;
            }


			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԉEX���W�����
			float temp = static_cast<float>(x * cellSize + cellSize);
			if (temp > result.left)
			{
				result.left = temp;
			}
		}
	}
	// �E����
	for (int x = indexMaxX; x < maxCell.x; x++)
	{
		for (int y = indexMinY; y <= indexMaxY; y++)
		{
			if (!IsCellRange(Cell(x, y), maxCell, Cell(0, 0)))
			{
				return Rect();
			}


            if (cellData[x][y] == 0)
            {
                continue;
            }

            bool isContinue = true;

            // �n�ʃZ���ԍ�������ꍇ�A���̃Z���ԍ��͖�������
            for (auto& unused : groundCellNumber)
            {
                if (cellData[x][y] == unused)
                {
                    isContinue = false;
                }
            }

            if (isContinue)
            {
                continue;
            }


			// ��ԍŏ��Ɍ��������Ԃ���}�b�v�`�b�v�̈�ԍ�X���W�����
			float temp = static_cast<float>(x * cellSize);
			if (temp < result.right)
			{
				result.right = temp;
			}
		}
	}

	return result;
}

bool EvoLib::Calculation::IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell)
{
    // �Z�����ő�Z���𒴂��Ă��邩�ǂ������m�F����
    if (cell.x >= maxCell.x ||
        cell.y >= maxCell.y)
    {
        // �����Ă�����false��Ԃ�
        return false;
    }

    // �Z�����ŏ��Z����������Ă��邩�ǂ������m�F����
    if (cell.x < minCell.x ||
        cell.y < minCell.y)
    {
        // ������Ă�����false��Ԃ�
        return false;
    }

    // true��Ԃ�
    return true;
}

Vec2 EvoLib::Calculation::TargetMoveValue(const Vec2& pos, const Vec2& targetPos, const float& moveSpeed)
{

    // �^�[�Q�b�g�̍��W���玩�g�̍��W���������l�����߂�
    Vec2 vel = targetPos - pos;

    // ���߂��������m�}���C�Y���邱�Ƃɂ�苗���̒������P�Ƃ݂�
    vel.normalize();

    // �m�[�}���C�Y���ꂽ�����Ɉړ����x��������
    vel *= moveSpeed;

    // �ړ��ʂ�Ԃ�
    return vel;
}
