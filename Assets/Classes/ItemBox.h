#pragma once
#include "cocos2d.h"
#include "Controller.h"
#include "Entity.h"
USING_NS_CC;


class ItemBox : public Node
{
public:
	ItemBox();
	~ItemBox();

	//根据指定逻辑坐标获取该逻辑坐标所属的物体，越界返回空
	Entity* getItem_lgc(int i, int j);

	//获取指定对象的逻辑坐标
	Point getItemPos(Entity* item);

	//设置指定行列的物体
	void setItem(int x, int y, Entity* item);

	//获取绑定的移动控制器
	Controller* getTouchController();

	//获取绑定的扫描器
	Controller* getScanner();

	//获取绑定的消除控制器
	Controller* getClearController();

	//获取绑定的掉落控制器
	Controller* getDropController();

	//获取行列值
	Point getCellNum();

	//获取单元格大小
	Point getCellSize();
protected:

	Controller* m_touchController;//移动控制器
	Controller* m_scanner;//扫描器
	Controller* m_clearController;//消除控制器
	Controller* m_dropController;//掉落控制器
	Point cellNum;//行列值
	Point cellSize;//单元格大小
	Entity*** items;//物体容器
private:
};

