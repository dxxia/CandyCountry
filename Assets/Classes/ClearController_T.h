#pragma once
#include "cocos2d.h"
#include "Controller.h"

USING_NS_CC;

class Entity;
class ItemBox_T;
class ClearController_T : public Controller
{
public:
	ClearController_T();
	~ClearController_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(ClearController_T, ItemBox_T);

	//��ȡָ���������ڵ�������Χ���Ƿ�״̬���ؿ��б�
	Vector<Entity*> getClearList(Entity* item);

	//��ȡ���ܴ�����������Χ���Ƿ�״̬���ؿ��б�
	Vector<Entity*> getSkillScope(Entity* item);

	//������󣬽���ת����clean״̬
	void clearEntity(Entity* item);

	//�ж�ָ���������ڵ�������Χ�Ƿ�������������󣬣��ڽڵ�item�����ɣ�
	bool createSkillItem(Vector<Entity*> clearList, Entity* item);

	//��ȡ�����ĵ÷�
	void getGoal(Vector<Entity*> clearLsit);
protected:
	//�����źż�����
	void clearListener(Ref* date);
};

