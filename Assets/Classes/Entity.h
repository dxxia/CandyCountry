#pragma once
#include "cocos2d.h"
USING_NS_CC;

//enum ItemType;
//enum ActionState;
//物体基类
class Entity : public Node
{
public:
	Entity();
	~Entity();

	//绑定精灵
	virtual void bindSprite(Sprite* sprite);

	//设置物体类型
	void setItemType(int type);

	//获取物体类型
	int getItemType();

	//设置运动状态
	void setActionState(int actionState);

	//获取运动状态
	int getActionState();

	//被点击动作
	virtual void onTouched() = 0;

	//点击恢复动作
	virtual void offTouched() = 0;
protected:
	Sprite* m_sprite;//精灵
	int m_itemType;//物体类型
	int m_actionState;//运动类型
};

