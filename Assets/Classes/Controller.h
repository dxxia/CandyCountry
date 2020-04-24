#pragma once
#include "cocos2d.h"
USING_NS_CC;

//��������create�꣬���ڿ�������Ŀ������
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
//���������࣬���������İ󶨹���
class Controller : public Node
{
public:
	Controller();
	~Controller();

	//��ȡĿ����������
	ItemBox* getItemBox();
	//��Ŀ������
	void bindItemBox(ItemBox* itemBox);
private:
	ItemBox* m_itemBox;//Ŀ������
};


