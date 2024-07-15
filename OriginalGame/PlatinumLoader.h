#pragma once
#include <DxLib.h>
#include <vector>


//�}�b�v�̍L�� * ���C���[���Ȃ̂ŁA2�̃x�N�^�[�����q�ɂ��Ă�
using MapData_t = std::vector < std::vector<unsigned char>>;


/// <summary>
/// Platinum�̃f�[�^�����[�h����
/// </summary>
class PlatinumLoader
{
public:
	PlatinumLoader();
	virtual ~PlatinumLoader();


	/// <summary>
	/// fmf�t�@�C����ǂݍ����mapData_�ɒ��g������
	/// </summary>
	/// <param name="filePath">fmf�t�@�C���̃t�@�C���p�X</param>
	void Load(const TCHAR* filePath);
	
	/// <summary>
	/// mapData_�̒��g�𒼐ڎ擾����
	/// </summary>
	const MapData_t& GetMapData()const;
	
	/// <summary>
	/// ���̃��C���[�����݂���̂��m�F
	/// </summary>
	/// <param name="layerNum"></param>
	void LayerCheck(int layerNum);

	/// <summary>
	/// Load�œǂݍ���MapData_t�̒��g�i�}�b�v�`�b�v�̃f�[�^�j���ʂɎ擾����
	/// </summary>
	/// <param name="layerType">�擾����}�b�v�`�b�v�̃��C���[ID</param>
	/// <param name="chipX">�擾����}�b�v�`�b�v��X���W</param>
	/// <param name="chipY">�擾����}�b�v�`�b�v��Y���W</param>
	/// <returns>�����Ŏw�肵���ꏊ�̃}�b�v�`�b�v�̃X�v���C�g�ԍ�</returns>
	const int GetChipSpriteNo(int layerNum,int chipX,int chipY)const;
	
	/// <summary>
	/// �}�b�v�̏c���Ɖ������擾
	/// </summary>
	/// <param name="width">�}�b�v����</param>
	/// <param name="height">�}�b�v�c��</param>
	void GetMapSize(int& width,int& height);

	/// <summary>
	/// �`�b�v�T�C�Y��Ԃ�
	/// </summary>
	/// <returns>�`�b�v�T�C�Y��Ԃ�</returns>
	const int GetChipSize();

private:

	// �}�b�v�f�[�^
	MapData_t m_mapData;

	// �}�b�v�̉���
	int m_mapWidth = 0;
	// �}�b�v�̏c��
	int m_mapHeight = 0;

	// �`�b�v�T�C�Y
	int m_chipSize = 0;

	// ���C���[��
	int m_layerCount = 0;
};

