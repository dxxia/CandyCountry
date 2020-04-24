#pragma once
#include "cocos2d.h"
#include "Controller.h"
#include "ItemType_L.h"

USING_NS_CC;

class Entity;
class ItemBox_L;

class ClearController_L : public Controller
{
public:
	ClearController_L();
	~ClearController_L();

	bool init(ItemBox_L* itemBox);
	CREATE_FUNC_T(ClearController_L, ItemBox_L);

	//获取指定对象所在的消除范围，非法状态返回空列表
	Vector<Entity*> getClearList(Entity* item);

	//触发技能
	void triggerSkill(Entity* item, ItemType type);

	//清理对象，将其转换成clean状态
	void clearEntity(Entity* item);

	//生成特殊对象，（在节点item处生成）
	void createSkillItem(Vector<Entity*> clearList, Entity* item);

private:
	//消除信号监听器
	void clearListener(Ref* date);
};

