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

	//��ȡָ���������ڵ�������Χ���Ƿ�״̬���ؿ��б�
	Vector<Entity*> getClearList(Entity* item);

	//��������
	void triggerSkill(Entity* item, ItemType type);

	//������󣬽���ת����clean״̬
	void clearEntity(Entity* item);

	//����������󣬣��ڽڵ�item�����ɣ�
	void createSkillItem(Vector<Entity*> clearList, Entity* item);

private:
	//�����źż�����
	void clearListener(Ref* date);
};

