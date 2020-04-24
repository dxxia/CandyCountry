#pragma once
#include "Controller.h"
#include "cocos2d.h"


USING_NS_CC;

class Entity;
class ItemBox_C;
//移动控制器，负责监听触摸事件并响应
class TouchController_C : public Controller
{
public:
	TouchController_C();
	~TouchController_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(TouchController_C, ItemBox_C);

	//获取上次触摸对象
	Entity* getLastTouchedItem();

	//交换动作
	void exchange(Entity* lastItem, Entity* nowItem, bool isCheck);

	//交换位置和固定且判断是否可消除选择消除或者回弹
	void exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck);
private:
	//设置上次触摸对象
	void setLastTouchedItem(Entity* cell);

	//触摸监听函数
	void touchListener();

	//时间结束事件监听函数
	void timeOver(Ref*);

	Entity* lastTouchedItem;//上次触摸的对象
	bool isTimeOver;//时间结束标志
};

