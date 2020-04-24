#pragma once
#include "cocos2d.h"
#include "Controller.h"

USING_NS_CC;

class Entity;
class ItemBox_L;
class DropController_L : public Controller
{
public:
	DropController_L();
	~DropController_L();

	bool init(ItemBox_L* itemBox);
	CREATE_FUNC_T(DropController_L, ItemBox_L);

	//获取掉落列表
	void findDropList();

	//刷新掉落状态
	void update(float dt);

	//刷新固定状态
	void updateToFixed();

	//每步移动结束进行逻辑判断
	void onStepEnd();
private:
	//掉落信号监听器
	void dropListener(Ref* date);

	Vector<Entity*> dropList;//掉落列表
	std::vector<Point> aimPos;//目标移动位置列表
	const float dropSpeed = 400.0;//掉落动画速度
};
