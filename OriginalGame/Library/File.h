#pragma once
#include "EvoLib.h"

namespace EvoLib
{
	class File
	{

	public:

		// �ǂݎ��^�C�v
		enum class LoadType
		{
			Noramal,// �ʏ�
			SkipFirstLine,	// ��s�ڂ��X�L�b�v����
			SkipOneColumn,	// �e���ڂ��X�L�b�v����
			DoubleSkip,		// ��s�ڂƊe���ڂ��X�L�b�v����
		};


	private:

		/// <summary>
		/// ������𕪊����ĕԂ�
		static std::vector<std::string> Split(const std::string& input, const char& delimiter);
	public:

		/// <summary>
		/// �t�@�C�������݂��邩�ǂ������m�F
		/// </summary>
		/// <param name="name">�t�@�C����</param>
		/// <returns>���̃t�@�C�������݂��邩�ǂ�����Ԃ�</returns>
		static bool IsFileExist(const std::string& name);


		/// <summary>
		/// CSV�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="isLoadFirstLine">�ŏ��̈�s��ǂݍ��ނ��ǂ���</param>
		/// <returns>�ǂݍ��񂾕������Ԃ�</returns>
		static std::vector<std::vector<std::string>> CsvFileLoading(const std::string& filePath, bool isLoadFirstLine = true);

		/// <summary>
		/// CSV�t�@�C����ǂݍ���(������)
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="isLoadFirstLine">�ŏ��̈�s��ǂݍ��ނ��ǂ���</param>
		/// <param name="isLoadOneColumn">�ŏ��̈���ǂݍ��ނ��ǂ���</param>
		/// <returns>�ǂݍ��񂾕������Ԃ�</returns>
		static std::vector<std::vector<std::string>> CsvFileLoading_Revision(const std::string& filePath, LoadType loadType = LoadType::Noramal);

		/// <summary>
		/// �Ȉ�CSV�t�@�C����������
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="writingText">�������ރe�L�X�g</param>
		static void SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText);
	};
}