#pragma once
#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;

//监听数据封装
class PostDate : public Ref
{
public:
	PostDate();
	~PostDate();

	bool init();
	CREATE_FUNC(PostDate);

	//存入Vector数据
	void setVector(Vector<Entity*> itemLsit);
	//取出Vector数据
	Vector<Entity*> getVector();
private:
	Vector<Entity*> m_itemList;//Vector数据
};
