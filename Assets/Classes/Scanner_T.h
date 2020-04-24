#pragma once
#include "cocos2d.h"
#include "Direction.h"
#include "Controller.h"
#include "ItemType_T.h"

USING_NS_CC;


class Entity;

class ItemBox_T;
class Scanner_T : public Controller
{
public:
	Scanner_T();
	~Scanner_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(Scanner_T, ItemBox_T);

	//重新排列物体--points需按从左下到右上的顺序排列
	void relocateItems(Vector<Entity*> items, std::vector<Point> points);

	//初始化物体类型
	void initItems();

	//检查全局若无潜在可消除的对象，则重新排列
	void checkAndRelocate();

	//若指定方向两个可消除物体类型相同则返回该类型，否则返回ClearItemType_END类型
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

	//将一个物体进行类型转换(工厂模式)
	void convertItemType(Entity* item, ItemType type);
private:
};
