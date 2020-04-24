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
private:
	//�����źż�����
	void clearListener(Ref* date);
};

