#pragma once
#include "cocos2d.h"
#include "Controller.h"

USING_NS_CC;
 
class Entity;
class ItemBox_C;
class ClearController_C : public Controller
{
public:
	ClearController_C();
	~ClearController_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(ClearController_C, ItemBox_C);

	//获取指定对象所在的消除范围，非法状态返回空列表
	Vector<Entity*> getClearList(Entity* item);

	//获取技能触发的消除范围，非法状态返回空列表
	Vector<Entity*> getSkillScope(Entity* item);

	//清理对象，将其转换成clean状态
	void clearEntity(Entity* item);

	//判断指定对象所在的消除范围是否能生成特殊对象，（在节点item处生成）
	bool createSkillItem(Vector<Entity*> clearList, Entity* item);

	//获取消除的得分
	void getGoal(Vector<Entity*> clearLsit);
private:
	//消除信号监听器
	void clearListener(Ref* date);
};

