#include "PlatinumLoader.h"
#include <string>
#include <cassert>

PlatinumLoader::PlatinumLoader():
	m_mapWidth(0),
	m_mapHeight(0),
	m_chipSize(0),
	m_layerCount(0)
{
	// �ϒ��z��̏�����
	m_mapData.clear();
}

PlatinumLoader::~PlatinumLoader()
{
}

void PlatinumLoader::Load(const TCHAR* filePath)
{
	//FMF�w�b�_�[(Platinum�̃h�L�������g�ɏ����Ă���)
	struct Header {
		int8_t id[4];			//���ʎq(FMF_)			1*4�o�C�g
		uint32_t size;			//�f�[�^�T�C�Y�@		4�o�C�g
		uint32_t mapWidth;		//�}�b�v�̕�			4�o�C�g
		uint32_t mapHeight;		//�}�b�v�̍����@		4�o�C�g
		uint8_t chiphWidth;		//�`�b�v(�Z�����)�̕�					1�o�C�g
		uint8_t chipHeight;		//�`�b�v(�Z�����)�̍���				1�o�C�g
		uint8_t layerCount;		//���C���[�̐�							1�o�C�g
		uint8_t bitCount;		//�P�Z��������̃r�b�g��(��8�Ńo�C�g��)	1�o�C�g
	};//20�o�C�g

	Header header;
	int handle =  FileRead_open(filePath);
	FileRead_read(&header,sizeof(header), handle);

	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());

	if (strId != "FMF_") {
		
		// �t�@�C�������e�L�X�g��
		std::string fileName = filePath;
	
		// �G���[���b�Z�[�W
		std::string errorMsg = "\n///////// README /////////\n\n" +
								fileName +
								" <-This file isn't here.\n\n" +
								 "-END-";
			
		//���C�h������ɕϊ�
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);

	
		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}

	// ���C���[������
	m_layerCount = header.layerCount;

	// �}�b�v�̏c���Ɖ�������
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;

	//���C���[1������̃T�C�Y���v�Z����
	//�}�b�v�̕����}�b�v�̍���*(�`�b�v1������̃o�C�g��)
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	// �`�b�v�T�C�Y����(�`�b�v�T�C�Y�͏c�������T�C�Y�Ȃ̂�Width�ł�Height�ł��ǂ���ł��悢)
	m_chipSize = header.chiphWidth;


	m_mapData.resize(m_layerCount);
	for (auto& layer : m_mapData) {
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);
}

const MapData_t& PlatinumLoader::GetMapData() const
{
	return m_mapData;
}

void PlatinumLoader::LayerCheck(int layerNum)
{
	// ���C���[�����݂��Ȃ��ꍇ�G���[���b�Z�[�W���o��
	if (m_layerCount == layerNum) {


		// �G���[���b�Z�[�W
		std::string errorMsg = "\n///////// README /////////\n\n"  
								" This fmf file does not have that layer..\n\n" 
								"-END-";

		//���C�h������ɕϊ�
		WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);


		_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

	}
}

const int PlatinumLoader::GetChipSpriteNo(int layerNum, int chipX, int chipY) const
{
	assert(chipX < m_mapWidth);
	assert(chipY < m_mapHeight);


	auto index = chipX + chipY * m_mapWidth;

	return m_mapData[layerNum][index];
}

void PlatinumLoader::GetMapSize(int& width, int& height)
{
	width = m_mapWidth;
	height = m_mapHeight;
}

const int PlatinumLoader::GetChipSize()
{
	return m_chipSize;
}
