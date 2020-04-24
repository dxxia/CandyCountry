#pragma once
#include "cocos2d.h"
#include "ItemType_C.h"
#include "Direction.h"
#include "Controller.h"

USING_NS_CC;


class Entity;
class ItemBox_C;
class Scanner_C : public Controller
{
public:
	Scanner_C();
	~Scanner_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(Scanner_C, ItemBox_C);

	//重新排列物体
	void relocateItems(Vector<Entity*>);

	//初始化物体类型
	void initItems();

	//检查全局若无潜在可消除的对象，则重新排列
	void checkAndRelocate();

	//若指定方向两个物体类型相同则返回该类型，否则返回ClearItemType_END类型
	ItemType getSameTowClearItemType(int x, int y, Direction direction);

	//判断该对象所处位置是否可以消除
	bool isClearable(Entity* item);

	//判断两个对象之间是否可以交换移动
	bool isMoveable(Entity* lastItem, Entity* nowItem);

	//判断全局是否有潜在的消除项
	bool isClearablePotentially();

	//获取潜在消除对象
	void getPotentialItem(Entity* &item1, Entity* &item2);

	//统计全局最少的3个物体类型，并随机返回
	ItemType getLeastClearItemType();
private:
};
