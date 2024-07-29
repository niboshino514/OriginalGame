#pragma once
#include "Vec2.h"
#include <DxLib.h>
#include "FunctionConclusion.h"
#include "ObjectFactory.h"
#include <memory>


class ObjectFactory;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �I�u�W�F�N�gID
	enum class ObjectID
	{
		Player,					// �v���C���[

		NoneMapChip,			// �N���s�}�b�v�`�b�v
		NoramalMapChip,			// �ʏ�}�b�v�`�b�v
		ObstacleMapChip,		// ��Q���}�b�v�`�b�v
		NextStageMapChip,		// ���̃X�e�[�W�ɐi�ރ}�b�v�`�b�v
		PreviousStageMapChip,	// �O�̃X�e�[�W�ɖ߂�}�b�v�`�b�v
	};


	// �`�惉���N(�����N�̐��l������������O�ɕ`�悳���)
	enum class DrawRank
	{
		Rank_1 = 1,
		Rank_2,
		Rank_3,
		Rank_4,
		Rank_5,
		Rank_6,
		RankNum,
	};
	


	/// <summary>
	/// �I�u�W�F�N�g�t�@�N�g���[�N���X���R�s�[
	/// </summary>
	/// <param name="objectFactory">�I�u�W�F�N�g�t�@�N�g���[</param>
	void SetObjectFactory(std::shared_ptr<ObjectFactory>objectFactory) { m_pObjectFactory = objectFactory; }

	/// <summary>
	/// ���݃t���O���
	/// </summary>
	/// <param name="isExlist">���݃t���O</param>
	void SetIsExlist(const bool& isExlist) { m_isExlist = isExlist; }

	/// <summary>
	/// ���݃t���O��Ԃ�
	/// </summary>
	/// <returns>���݃t���O</returns>
	bool GetIsExlist() { return m_isExlist; }

	


	/// <summary>
	/// �~�����
	/// </summary>
	/// <param name="circle">�~���</param>
	void SetCircle(const Circle& circle) { m_circle = circle; }

	/// <summary>
	/// �~����Ԃ�
	/// </summary>
	/// <returns>�~���</returns>
	Circle GetCircle() { return m_circle; }


	/// <summary>
	/// �l�p�`������
	/// </summary>
	/// <param name="square">�l�p�`���</param>
	void SetSquare(const Square& square) { m_square = square; }

	/// <summary>
	/// �l�p�`����Ԃ�
	/// </summary>
	/// <returns>�l�p�`���</returns>
	Square GetSquare() { return m_square; }


	/// <summary>
	/// �I�u�W�F�N�gID��Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�gID</returns>
	ObjectID GetObjectID() { return m_objectID; }

	/// <summary>
	/// �`��D�揇�ʂ�Ԃ�
	/// </summary>
	/// <returns>�`��D�揇��</returns>
	DrawRank GetDrawRank() { return m_drawRank; }

	/// <summary>
	/// �`��D�揇�ʎ擾
	/// </summary>
	/// <param name="drawRank">�`�惉���N</param>
	void SetDrawRank(const DrawRank& drawRank) { m_drawRank = drawRank; }


protected:


	// �I�u�W�F�N�g�̎��
	ObjectID m_objectID;


	// ���݃t���O
	bool m_isExlist;

	// �~���
	Circle m_circle;

	// �l�p�`���
	Square m_square;

	// �`��̗D�揇��
	DrawRank m_drawRank;

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	std::shared_ptr<ObjectFactory>m_pObjectFactory;// �I�u�W�F�N�g�t�@�N�g���[
};