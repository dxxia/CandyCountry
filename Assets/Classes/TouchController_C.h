#pragma once
#include "Controller.h"
#include "cocos2d.h"


USING_NS_CC;

class Entity;
class ItemBox_C;
//�ƶ���������������������¼�����Ӧ
class TouchController_C : public Controller
{
public:
	TouchController_C();
	~TouchController_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(TouchController_C, ItemBox_C);

	//��ȡ�ϴδ�������
	Entity* getLastTouchedItem();

	//��������
	void exchange(Entity* lastItem, Entity* nowItem, bool isCheck);

	//����λ�ú͹̶����ж��Ƿ������ѡ���������߻ص�
	void exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck);
private:
	//�����ϴδ�������
	void setLastTouchedItem(Entity* cell);

	//������������
	void touchListener();

	//ʱ������¼���������
	void timeOver(Ref*);

	Entity* lastTouchedItem;//�ϴδ����Ķ���
	bool isTimeOver;//ʱ�������־
};

