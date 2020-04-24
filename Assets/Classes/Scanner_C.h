#pragma once
#include "cocos2d.h"
#include "ItemType_C.h"
#include "Direction.h"
#include "Controller.h"

USING_NS_CC;


class Entity;
class ItemBox_C;
class Scanner_C : public Controller
{
public:
	Scanner_C();
	~Scanner_C();

	bool init(ItemBox_C* itemBox);
	CREATE_FUNC_T(Scanner_C, ItemBox_C);

	//������������
	void relocateItems(Vector<Entity*>);

	//��ʼ����������
	void initItems();

	//���ȫ������Ǳ�ڿ������Ķ�������������
	void checkAndRelocate();

	//��ָ��������������������ͬ�򷵻ظ����ͣ����򷵻�ClearItemType_END����
	ItemType getSameTowClearItemType(int x, int y, Direction direction);

	//�жϸö�������λ���Ƿ��������
	bool isClearable(Entity* item);

	//�ж���������֮���Ƿ���Խ����ƶ�
	bool isMoveable(Entity* lastItem, Entity* nowItem);

	//�ж�ȫ���Ƿ���Ǳ�ڵ�������
	bool isClearablePotentially();

	//��ȡǱ����������
	void getPotentialItem(Entity* &item1, Entity* &item2);

	//ͳ��ȫ�����ٵ�3���������ͣ����������
	ItemType getLeastClearItemType();
private:
};
