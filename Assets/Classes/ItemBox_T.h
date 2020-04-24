#pragma once
#include "cocos2d.h"
#include "ItemBox.h"
#include "ClearItem_T.h"
#include "BreakItem_T.h"
#include "DropItem_T.h"
#include "Scanner_T.h"
#include "TouchController_T.h"
#include "ClearController_T.h"
#include "DropController_T.h"
USING_NS_CC;

#define CELLBG 0
#define ITEM 5

class ItemBox_T : public ItemBox
{
public:
	ItemBox_T(int tollNum);
	~ItemBox_T();

	bool init();
	static ItemBox_T* create(int tollNum);

	//�̶�Ŀ��λ��
	void resetPos(Entity* item);

	//���Ƶ�Ԫ�񱳾�
	void drawCell();

private:
	//���ز�������
	void loadMapConfig();

	//��ǰ�ؿ���
	int tollNum = 1;
};

