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

	//固定目标位置
	void resetPos(Entity* item);

	//绘制单元格背景
	void drawCell();

private:
	//加载布局配置
	void loadMapConfig();

	//当前关卡数
	int tollNum = 1;
};

