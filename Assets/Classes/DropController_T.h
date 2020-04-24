#pragma once
#include "cocos2d.h"
#include "Controller.h"

USING_NS_CC;

class Entity;
class ItemBox_T;
class DropController_T : public Controller
{
public:
	DropController_T();
	~DropController_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(DropController_T, ItemBox_T);

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
