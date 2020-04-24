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

	//��ȡ�����б�
	void findDropList();

	//ˢ�µ���״̬
	void update(float dt);

	//ˢ�¹̶�״̬
	void updateToFixed();

	//ÿ���ƶ����������߼��ж�
	void onStepEnd();
private:
	//�����źż�����
	void dropListener(Ref* date);

	Vector<Entity*> dropList;//�����б�
	std::vector<Point> aimPos;//Ŀ���ƶ�λ���б�
	const float dropSpeed = 400.0;//���䶯���ٶ�
};
