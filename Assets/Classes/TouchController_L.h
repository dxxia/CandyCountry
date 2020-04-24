#pragma once
#include "Controller.h"
#include "cocos2d.h"


USING_NS_CC;

class Entity;
class ItemBox_L;
//�ƶ���������������������¼�����Ӧ
class TouchController_L : public Controller
{
public:
	TouchController_L();
	~TouchController_L();

	bool init(ItemBox_L* itemBox);
	CREATE_FUNC_T(TouchController_L, ItemBox_L);

	//��ȡ�ϴδ�������
	Entity* getLastTouchedItem();

	//��������
	void exchange(Entity* lastItem, Entity* nowItem, bool isCheck);

	//����λ�ú͹̶����ж��Ƿ������,ѡ���������߻ص�
	void exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck);
private:
	//�����ϴδ�������
	void setLastTouchedItem(Entity* cell);

	//������������
	void touchListener();

	//�ƶ�������������
	void moveOver(Ref*);

	Entity* lastTouchedItem;//�ϴδ����Ķ���
	bool isMoving;//�����ƶ���־����ֹ�ظ�����
};

