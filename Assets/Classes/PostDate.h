#pragma once
#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;

//�������ݷ�װ
class PostDate : public Ref
{
public:
	PostDate();
	~PostDate();

	bool init();
	CREATE_FUNC(PostDate);

	//����Vector����
	void setVector(Vector<Entity*> itemLsit);
	//ȡ��Vector����
	Vector<Entity*> getVector();
private:
	Vector<Entity*> m_itemList;//Vector����
};
