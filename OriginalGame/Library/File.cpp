#include "File.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <array>

std::vector<std::string> EvoLib::File::Split(const std::string& input, const char& delimiter)
{
    std::istringstream stream(input);	// string��getline�Ŏg����f�[�^�ɕϊ��H
    std::string field;					// ��������������1�����i�[����H
    std::vector<std::string> result;	// ������̕�����̔z��

    while (getline(stream, field, delimiter))
    {
        result.push_back(field);
    }

    // ���������������Ԃ�
    return result;
}

bool EvoLib::File::IsFileExist(const std::string& name)
{
    return std::filesystem::is_regular_file(name);
}

std::vector<std::vector<std::string>> EvoLib::File::CsvFileLoading(const std::string& filePath, bool isLoadFirstLine)
{
    // �t�@�C�����̓ǂݍ���
    std::ifstream ifs(filePath);
    std::string line;


    // �s�J�E���g
    int lineCount = 0;

    // �ǂݍ��񂾃e�L�X�g
    std::vector<std::vector<std::string>>loadText;



    while (getline(ifs, line))
    {
        // �s�J�E���g�����Z
        lineCount++;

        // ��s�ڂ̏������΂�
        if (lineCount <= 1 && !isLoadFirstLine)continue;

        // csv�f�[�^1�s��','�ŕ����̕�����ɕ���
        std::vector<std::string>strvec = File::Split(line, ',');

        if (static_cast<int>(strvec.size()) == 0)
        {
            break;
        }
        loadText.push_back(strvec);
    }

    return loadText;
}

std::vector<std::vector<std::string>> EvoLib::File::CsvFileLoading_Revision(const std::string& filePath, LoadType loadType)
{
    // �t�@�C�����̓ǂݍ���
    std::ifstream ifs(filePath);
    std::string line;


    // �s�J�E���g
    int lineCount = 0;

    // �ǂݍ��񂾃e�L�X�g
    std::vector<std::vector<std::string>>loadText;

    while (getline(ifs, line))
    {
        // �s�J�E���g�����Z
        lineCount++;


        // ��s�ڂ̏������΂�
        if (lineCount <= 1 && loadType == File::LoadType::SkipFirstLine ||
            lineCount <= 1 && loadType == File::LoadType::DoubleSkip)
        {
            continue;
        }



        // csv�f�[�^1�s��','�ŕ����̕�����ɕ���
        std::vector<std::string>	strvec = File::Split(line, ',');

        if (static_cast<int>(strvec.size()) == 0)
        {
            break;
        }

        // ���ڂ��X�L�b�v����
        if (loadType == File::LoadType::SkipOneColumn ||
            loadType == File::LoadType::DoubleSkip)
        {
            strvec.erase(strvec.begin());
        }

        loadText.push_back(strvec);
    }



    return loadText;
}

void EvoLib::File::SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText)
{
    std::ofstream writing_file;
    writing_file.open(fileName, std::ios::out);

    writing_file << writingText << std::endl;

    writing_file.close();
}
