#pragma once
#include "cocos2d.h"
#include "ItemBox.h"
#include "ClearItem_C.h"
#include "Scanner_C.h"
#include "TouchController_C.h"
#include "ClearController_C.h"
#include "DropController_C.h"
USING_NS_CC;



class ItemBox_C : public ItemBox
{
public:
	ItemBox_C();
	~ItemBox_C();
	
	bool init();
	CREATE_FUNC(ItemBox_C);

	//根据指定逻辑坐标转换成物理坐标，并获取该物理坐标所属的物体
	Entity* getItem_lgc2phy(int i, int j);

private:
};

