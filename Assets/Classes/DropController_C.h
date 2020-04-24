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

	//��ȡ�����б�
	void findDropList(Vector<Entity*> clearList);

	//���ɵ�����󣬲���������б�
	void createDropList(Vector<Entity*> clearList);

	//���ָ����������Ƿ���Թ̶����Ƿ�ִ�й̶�
	bool checkAndFix(Entity* item, bool isFixed);

	//ˢ�µ���״̬
	void update(float dt);
private:
	//�����źż�����
	void dropListener(Ref* date);

	std::vector<Vector<Entity*>> dropList;//�����б�ÿ��һ���б�
	const float dropSpeed = 500.0;//���䶯���ٶ�
};
