#pragma once
#include "cocos2d.h"
#include "Direction.h"
#include "Controller.h"
#include "ItemType_T.h"

USING_NS_CC;


class Entity;

class ItemBox_T;
class Scanner_T : public Controller
{
public:
	Scanner_T();
	~Scanner_T();

	bool init(ItemBox_T* itemBox);
	CREATE_FUNC_T(Scanner_T, ItemBox_T);

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

	//��ȡǱ����������
	void getPotentialItem(Entity* &item1, Entity* &item2);

	//ͳ��ȫ�����ٵ�3���������ͣ����������
	ItemType getLeastClearItemType();

	//��һ�������������ת��(����ģʽ)
	void convertItemType(Entity* item, ItemType type);
private:
};
