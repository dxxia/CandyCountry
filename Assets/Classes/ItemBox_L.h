#pragma once
#include "cocos2d.h"
#include "ItemBox.h"
#include "ClearItem_L.h"
#include "Scanner_L.h"
#include "TouchController_L.h"
#include "ClearController_L.h"
#include "DropController_L.h"
USING_NS_CC;

#define CELLBG 0
#define ITEM 5

class ItemBox_L : public ItemBox
{
public:
	ItemBox_L();
	~ItemBox_L();

	bool init();
	CREATE_FUNC(ItemBox_L);

	//�̶�Ŀ��λ��
	void resetPos(Entity* item);

	//���Ƶ�Ԫ�񱳾�
	void drawCell();

private:
};

