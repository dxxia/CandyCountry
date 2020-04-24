#pragma once
#include "cocos2d.h"
#include "Controller.h"

USING_NS_CC;

class Entity;
class ItemBox_C;
class DropController_C : public Controller
{
public:
	DropController_C();
	~DropController_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(DropController_C, ItemBox_C);

	//获取掉落列表
	void findDropList(Vector<Entity*> clearList);

	//生成掉落对象，并加入掉落列表
	void createDropList(Vector<Entity*> clearList);

	//检查指定掉落对象是否可以固定并是否执行固定
	bool checkAndFix(Entity* item, bool isFixed);

	//刷新掉落状态
	void update(float dt);
private:
	//掉落信号监听器
	void dropListener(Ref* date);

	std::vector<Vector<Entity*>> dropList;//掉落列表，每列一个列表
	const float dropSpeed = 500.0;//掉落动画速度
};
