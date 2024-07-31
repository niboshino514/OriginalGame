#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	// �Փ˃N���X
	class Collision
	{
	public:

		/// <summary>
		/// ���̏Փ˔���
		/// </summary>
		/// <param name="sphere1">�����1</param>
		/// <param name="sphere2">�����2</param>
		/// <returns>�Փ˂������ǂ�����Ԃ�</returns>
		static bool SphereCollision(const Sphere& sphere1, const Sphere& sphere2);


		/// <summary>
		/// �~�Ǝl�p�`�Ƃ̓����蔻��
		/// </summary>
		/// <param name="circle">�~���</param>
		/// <param name="square">�l�p�`���</param>
		/// <returns>�~�Ǝl�p�`�������������ǂ�����Ԃ�</returns>
		static bool IsAABBCollisionDetection(const Circle& circle, const Square& square);

		/// <summary>
		/// �~���m�̓����蔻��
		/// </summary>
		/// <param name="circle1">�~���1</param>
		/// <param name="circle2">�~���2</param>
		/// <returns>�~���m�̏Փ˔���t���O</returns>
		static bool IsCircleToCircle(const Circle& circle1, const Circle& circle2);

		/// <summary>
		/// �O�p�`�Ǝl�p�`�̓����蔻��
		/// </summary>
		/// <param name="triangle">�O�p�`���</param>
		/// <param name="square">�l�p�`���</param>
		/// <returns>�O�p�`�Ǝl�p�`�̏Փ˔���t���O</returns>
		static bool IsTriangleToSquare(const Triangle& triangle, const Square& square);

		/// <summary>
		/// �������m���������Ă��邩�ǂ����𒲂ׂ�
		/// </summary>
		/// <param name="start1">�X�^�[�g���W�P</param>
		/// <param name="end1">�S�[�����W�P</param>
		/// <param name="start2">�X�^�[�g���W�Q</param>
		/// <param name="end2">�S�[�����W�Q</param>
		/// <returns>�������m���������Ă��邩�ǂ����𒲂ׂ�</returns>
		static bool IsLein(const Vec2& start1, const Vec2& end1, const Vec2& start2, const Vec2& end2);

	};
}