#pragma once
#include "ObjectBase.h"

class TransparentBlockChip : public ObjectBase
{
public:
	TransparentBlockChip();
	virtual ~TransparentBlockChip();

	void Init()override;
	void Update()override;
	void Draw()override;

private:

	/// <summary>
	/// �Փˏ���
	/// </summary>
	void Collision();


private:

};