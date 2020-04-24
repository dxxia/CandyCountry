#pragma once
#include "Controller.h"
#include "cocos2d.h"


USING_NS_CC;

class Entity;
class ItemBox_T;
//移动控制器，负责监听触摸事件并响应
class TouchController_T : public Controller
{
public:
	TouchController_T();
	~TouchController_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(TouchController_T, ItemBox_T);

	//获取上次触摸对象
	Entity* getLastTouchedItem();

	//设置上次触摸对象
	void setLastTouchedItem(Entity* cell);

	//交换动作
	void exchange(Entity* lastItem, Entity* nowItem, bool isCheck);

	//交换位置和固定且判断是否可消除选择消除或者回弹
	void fixCheck(Entity* lastItem, Entity* nowItem, bool isCheck);
protected:
	//触摸监听函数
	void touchListener();

	//移动结束监听函数
	void moveOver(Ref*);
private:
	Entity* lastTouchedItem;//上次触摸的对象
	bool isMoving;//正在移动标志，防止重复操作
};

