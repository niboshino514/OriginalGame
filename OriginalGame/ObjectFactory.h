#pragma once
#include <list>
#include <memory>
#include <vector>



// �}�b�v
struct Map
{
	// �}�b�v���
	std::vector<std::vector<int>> mapInfo;

	

};

class ObjectBase;

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:



public:
	ObjectFactory();
	virtual ~ObjectFactory();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// �L�����N�^�[����
	/// </summary>
	void CharacterCreate();

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="�}�b�v���"></param>
	void MapChipCreate(const std::vector<std::vector<int>>& mapData);



	/// <summary>
	/// �I�u�W�F�N�g�폜
	/// </summary>
	void ObjectErase();


	/// <summary>
	/// ���̃X�e�[�W�ֈڍs
	/// </summary>
	void NextStageToMigration();


	/// <summary>
	/// �I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g���</returns>
	std::list<std::shared_ptr<ObjectBase>>GetObjectInfo() { return m_object; }


private:

	// �I�u�W�F�N�g
	std::list<std::shared_ptr<ObjectBase>>m_object;

};