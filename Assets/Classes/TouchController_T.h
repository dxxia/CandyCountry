#pragma once
#include "Controller.h"
#include "cocos2d.h"


USING_NS_CC;

class Entity;
class ItemBox_T;
//�ƶ���������������������¼�����Ӧ
class TouchController_T : public Controller
{
public:
	TouchController_T();
	~TouchController_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(TouchController_T, ItemBox_T);

	//��ȡ�ϴδ�������
	Entity* getLastTouchedItem();

	//�����ϴδ�������
	void setLastTouchedItem(Entity* cell);

	//��������
	void exchange(Entity* lastItem, Entity* nowItem, bool isCheck);

	//����λ�ú͹̶����ж��Ƿ������ѡ���������߻ص�
	void fixCheck(Entity* lastItem, Entity* nowItem, bool isCheck);
protected:
	//������������
	void touchListener();

	//�ƶ�������������
	void moveOver(Ref*);
private:
	Entity* lastTouchedItem;//�ϴδ����Ķ���
	bool isMoving;//�����ƶ���־����ֹ�ظ�����
};

