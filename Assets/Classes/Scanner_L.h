#pragma once
#include "cocos2d.h"
#include "Direction.h"
#include "Controller.h"
#include "ItemType_L.h"

USING_NS_CC;


class Entity;

class ItemBox_L;
class Scanner_L : public Controller
{
public:
	Scanner_L();
	~Scanner_L();

	bool init(ItemBox_L* itemBox);
	CREATE_FUNC_T(Scanner_L, ItemBox_L);

	//������������--points�谴�����µ����ϵ�˳������
	void relocateItems(Vector<Entity*> items, std::vector<Point> points);

	//��ʼ����������
	void initItems();

	//���ȫ������Ǳ�ڿ������Ķ�������������
	void checkAndRelocate();

	//��ָ��������������������������ͬ�򷵻ظ����ͣ����򷵻�ClearItemType_END����
	ItemType getSameTowClearItemType(int x, int y, Direction direction);

	//�жϸö�������λ���Ƿ��������
	bool isClearable(Entity* item);

	//�ж���������֮���Ƿ���Խ����ƶ�
	bool isMoveable(Entity* lastItem, Entity* nowItem);

	//�ж�ȫ���Ƿ���Ǳ�ڵ�������
	bool isClearablePotentially();

	//ͳ��ȫ�����ٵ�3���������ͣ����������
	ItemType getLeastClearItemType();
private:
};
