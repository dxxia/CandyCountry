#pragma once
#include "cocos2d.h"
USING_NS_CC;

//定义多参数create宏，用于控制器绑定目标容器
#define CREATE_FUNC_T(__TYPE__,__TARGET__) \
	static __TYPE__* create(__TARGET__* target) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init(target)) \
{ \
	pRet->autorelease(); \
} \
		else \
{ \
	delete pRet; \
	pRet = NULL; \
} \
	return pRet; \
}

class ItemBox;
//控制器基类，负责容器的绑定工作
class Controller : public Node
{
public:
	Controller();
	~Controller();

	//获取目标容器对象
	ItemBox* getItemBox();
	//绑定目标容器
	void bindItemBox(ItemBox* itemBox);
private:
	ItemBox* m_itemBox;//目标容器
};


