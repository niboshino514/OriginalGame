#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	// �v�Z�N���X
	class Calculation
	{

	public:

		// �v�Z(�����Z�A�����Z)
		enum class CalculationType
		{
			Addition,	// �����Z
			Subtraction,// �����Z
		};


	public:

		/// <summary>
		/// �T�C���J�[�u�l�����߂�
		/// </summary>
		/// <param name="currentFrame">���݂̃t���[��</param>
		/// <param name="maxFrame">�ő�t���[��</param>
		/// <param name="maxValue">�T�C���J�[�u�̍ő�l</param>
		/// <param name="isAbs">abs���g�p���邩�ǂ���</param>
		/// <returns>�T�C���J�[�u�l</returns>
		static int SineCurve(int currentFrame, int maxFrame, int maxValue, bool isAbs = false);


		/// <summary>
		/// 2D�̐U��
		/// </summary>
		/// <param name="currentFrame">���݂̃t���[��</param>
		/// <param name="maxFrame">�U��������ő�t���[��</param>
		/// <param name="vibrationWidth">�U����</param>
		/// <returns>2D�̐U���l</returns>
		static Vec2 VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth);


		/// <summary>
		/// ���[�v�ԍ�
		/// </summary>
		/// <param name="minNumber">�ŏ��l</param>
		/// <param name="maxNumber">�ő�l</param>
		/// <param name="currentNumber">���݂̒l</param>
		/// <param name="isInverseLoop">�t���[�v���邩�ǂ���</param>
		/// <returns>�ϊ����ꂽ�ԍ�</returns>
		static int SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop = false);


		/// <summary>
		/// �O���t�B�b�N�̃T�C�Y���擾
		/// </summary>
		/// <param name="graphHandle">�O���t�B�b�N�n���h��</param>
		/// <param name="graphMagnificationRate">�O���t�B�b�N�̊g�嗦</param>
		/// <returns>�O���t�B�b�N�̃T�C�Y</returns>
		static Vec2 GetGraphSize_EvoLib(std::vector<int>graphHandle, const double& graphMagnificationRate);



		/// <summary>
		/// �摜�T�C�Y����Ƃ��āA�������W���ϓ��ɔz�u����
		/// </summary>
		/// <param name="graphSize">�O���t�̃T�C�Y</param>
		/// <param name="basePos">����W</param>
		/// <param name="num">��</param>
		/// <param name="graphInterval">�O���t�B�b�N���m�̕�</param>
		/// <param name="isHorizontalSort">�����т��ǂ���</param>
		/// <returns>���W</returns>
		static std::vector<Vec2>GraphEqualization(const Vec2& graphSize, const Vec2& basePos, const int& num, const Vec2& graphInterval, const bool& isHorizontalSort);

	
	


		/// <summary>
		/// ������W����Ƃ��āA�������W���ϓ��ɔz�u����
		/// </summary>
		/// <param name="basePos">����W</param>
		/// <param name="num">��</param>
		/// <param name="interval">�Ԋu</param>
		/// <returns>���W</returns>
		static std::vector<Vec2>PosEqualization(const Vec2& basePos, const int& num, const Vec2& interval);



		/// <summary>
		/// �^�[�Q�b�g�͈͓̔����ǂ����̔���
		/// </summary>
		/// <param name="currentValue">���݂̒l</param>
		/// <param name="targetValue">�^�[�Q�b�g�̒l</param>
		/// <param name="addSubValue">�����Z�l</param>
		/// <returns>�^�[�Q�b�g�͈͓̔����ǂ���</returns>
		static bool IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue);


		/// <summary>
		/// �p�x�������Z���āA360�x�𒴂�����A0�x�ȉ��ɂȂ�Ȃ��悤�Ɍv�Z���s��
		/// </summary>
		/// <param name="currentAngle">���݂̊p�x</param>
		/// <param name="addSubValue">�����Z�l</param>
		/// <param name="calculation">�����Z�������Z���ǂ���</param>
		/// <param name="isInteger">�����l���ǂ���</param>
		/// <param name="isAdjustment">�����l�������邩�ǂ���</param>
		/// <returns>�����Z���I�����p�x��Ԃ�</returns>
		static float AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, EvoLib::Calculation::CalculationType calculation, bool isInteger = true, bool isAdjustment = true);


		/// <summary>
		/// �O�����̒藝
		/// </summary>
		/// <param name="poa1">���W1</param>
		/// <param name="pos2">���W2</param>
		/// <returns>���p�O�p�`�̈�ӂ̒���</returns>
		static float ThreeSquareTheorem(const Vec2& poa1, const Vec2& pos2);

		/// <summary>
		/// �l�����̐����ǂ����𒲂ׂ�
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="value">���ׂ�l</param>
		/// <returns>�l�����̐����ǂ����̃t���O</returns>
		template <typename T>
		static bool IsValueNegativeCount(T& value);

		/// <summary>
		/// ���`��Ԑ������߂�
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="vec">�ړ���</param>
		/// <param name="size">�c����</param>
		/// <returns>���`��Ԑ�</returns>
		static int InearInterpolationCount(const Vec2& pos, const Vec2& vec, const Vec2& size);

		/// <summary>
		/// ���`��ԍ��W�����߂�
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="vec">�ړ���</param>
		/// <param name="iinearInterpolationCount">���`��Ԑ�</param>
		/// <returns>���`��ԍ��W</returns>
		static std::vector<Vec2> InearInterpolationPos(const Vec2& pos, const Vec2& vec, const int& iinearInterpolationCount);

		/// <summary>
		/// ��`�̈ړ��͈͂��v�Z
		/// </summary>
		/// <param name="rect">���g�̋�`���</param>
		/// <param name="maxCell">�Z���̍ő吔</param>
		/// <param name="cellSize">�Z���T�C�Y</param>
		/// <param name="cellData">�񎟌��Z���f�[�^</param>
		/// <param name="unusedCellNumber">�n�ʃZ���ԍ�</param>
		/// <returns>�ړ��͈�</returns>
		static Rect CalculateRectangleMovementRange(const Rect& rect, const Cell& maxCell, const float& cellSize, const std::vector<std::vector<int>>& cellData, const std::vector<int> groundCellNumber);


		/// <summary>
		/// �Z�����͈͓����ǂ���
		/// </summary>
		/// <param name="cell">���ׂ�Z��</param>
		/// <param name="maxCell">�ő�Z��</param>
		/// <param name="minCell">�ŏ��Z��</param>
		/// <returns>�Z�����͈͓����ǂ�����Ԃ�</returns>
		static bool IsCellRange(const Cell& cell, const Cell& maxCell, const Cell& minCell);

		/// <summary>
		/// ��������l��ݒ�
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="x">���l</param>
		/// <param name="low">����</param>
		/// <param name="high">���</param>
		/// <returns></returns>
		template<typename T>
		static T Clamp(T x, T low, T high);


		// �^�[�Q�b�g�Ɍ������ړ��ʂ����߂�
		static Vec2 TargetMoveValue(const Vec2& pos, const Vec2& targetPos, const float& moveSpeed);

		/// <summary>
		/// �q���O��(���S�_�����ɁA�p�x���Q�Ƃ��ĉ�]������W���v�Z����)
		/// </summary>
		/// <param name="centerPos">���S���W</param>
		/// <param name="radian">���W�A��</param>
		/// <param name="circleRadius">�~�̔��a</param>
		/// <returns>�A���O���ɑΉ��������W��Ԃ�</returns>
		static Vec2 SatelliteTrajectory(const Vec2& centerPos, const float& radian, const float& circleRadius);

		/// <summary>
		/// ���_���W����]������
		/// </summary>
		/// <param name="rotaCenterPos">��]�̒��S���W</param>
		/// <param name="radian">��]�����郉�W�A��</param>
		/// <param name="vertexPos">���_���W</param>
		/// <returns></returns>
		static std::vector<Vec2> VertexCoordinateRotation(const Vec2& rotaCenterPos, const float& radian, const std::vector<Vec2> vertexPos);

		/// <summary>
		/// Atan2(�^�[�Q�b�g�����Ɍ������A���W�A�������߂�)
		/// </summary>
		/// <param name="targetPos">�^�[�Q�b�g���W</param>
		/// <param name="startPos">�X�^�[�g���W</param>
		/// <returns>�^�[�Q�b�g���W���������W�A��</returns>
		static float Atan2_Lib(const Vec2& targetPos, const Vec2& startPos);


	};
	template<typename T>
	inline bool Calculation::IsValueNegativeCount(T& value)
	{
		// �l��0�ȉ��Ȃ�΁Afalse
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