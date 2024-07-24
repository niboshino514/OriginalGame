#pragma once
#include "FunctionConclusion.h"

class GameData
{
public:

	// �Z�[�u�|�C���g�f�[�^
	struct SavePointData
	{
		// �X�e�[�W�i���o�[
		int stageNumber = 0;

		// �Z�[�u�����Z��
		Cell cell = Cell();
	}; 


public:
	GameData();
	virtual ~GameData();

	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^���擾����
	/// </summary>
	/// <param name="savePointData">�Z�[�u�|�C���g�f�[�^</param>
	void SetSavePointData(const SavePointData& savePointData) { m_savePointData = savePointData; }

	/// <summary>
	/// �Z�[�u�|�C���g�f�[�^��Ԃ�
	/// </summary>
	/// <returns>�Z�[�u�|�C���g�f�[�^</returns>
	SavePointData GetSavePointData() { return m_savePointData; }

private:

	// �Z�[�u�|�C���g�f�[�^
	SavePointData m_savePointData;
};