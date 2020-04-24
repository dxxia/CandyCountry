#include "ClearController_L.h"
#include "ItemBox_L.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"

ClearController_L::ClearController_L()
{
}

ClearController_L::~ClearController_L()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool ClearController_L::init(ItemBox_L* itemBox){
	this->bindItemBox(itemBox);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ClearController_L::clearListener), "clear", NULL);
	return true;
}

void ClearController_L::clearListener(Ref* date){
	//处理数据
	Vector<Entity*> clearList;
	Vector<Entity*> list = ((PostDate*)date)->getVector();
	Entity* lastItem = NULL;
	Entity* nowItem = NULL;

	//判断单个消除还是交换消除
	if (list.size() == 1){
		lastItem = list.at(0);
	}
	else if (list.size() == 2){
		lastItem = list.at(0);
		nowItem = list.at(1);
	}
	else{
		CCASSERT(false, "ClearController_L::clearListener(Ref* date)：交换对象列表大小非法！");
	}
	date->release();

	//获取第一个对象的消除列表
	Vector<Entity*> list1 = getClearList(lastItem);
	if (list1.size() > 0){
		clearList.pushBack(list1);
		//获取生成对象
		createSkillItem(list1, lastItem);
		//生成对象保留
		clearList.eraseObject(lastItem);
		
	}

	//获取第二个对象的消除列表
	Vector<Entity*> list2 = getClearList(nowItem);
	if (list2.size() > 0){
		clearList.pushBack(list2);
		createSkillItem(list2, nowItem);
		clearList.eraseObject(nowItem);
	}

	//清理消除列表，状态转换成clean,等级归零
	for (const auto& it : clearList){
		clearEntity(it);
	}

	//发送掉落信号
	if (clearList.size() > 0){
		//播放音效
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/clear.wav");

		NotificationCenter::getInstance()->postNotification("drop", date);
	}
}

Vector<Entity*> ClearController_L::getClearList(Entity* item){
	Vector<Entity*> clearList;

	//无法获取，忽略
	if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END \
		|| item->getActionState() != Fixed){
		return clearList;
	}

	//获取对象逻辑坐标
	ItemType type = ItemType(item->getItemType());
	int x = this->getItemBox()->getItemPos(item).x;
	int y = this->getItemBox()->getItemPos(item).y;

	//竖直方向搜索消除项
	int up = 1;
	while (this->getItemBox()->getItem_lgc(x, y + up) != NULL && this->getItemBox()->getItem_lgc(x, y + up)->getItemType() == type \
		&& this->getItemBox()->getItem_lgc(x, y + up)->getActionState() == Fixed){
		up++;
	}
	up--;
	int down = 1;
	while (this->getItemBox()->getItem_lgc(x, y - down) != NULL && this->getItemBox()->getItem_lgc(x, y - down)->getItemType() == type \
		&& this->getItemBox()->getItem_lgc(x, y - down)->getActionState() == Fixed){
		down++;
	}
	down--;
	if (up + down + 1 >= 3){
		for (int i = 1; i <= up; i++){
			clearList.pushBack(this->getItemBox()->getItem_lgc(x, y + i));
		}
		for (int i = 1; i <= down; i++){
			clearList.pushBack(this->getItemBox()->getItem_lgc(x, y - i));
		}
	}

	//水平方向搜索消除项
	int left = 1;
	while (this->getItemBox()->getItem_lgc(x - left, y) != NULL && this->getItemBox()->getItem_lgc(x - left, y)->getItemType() == type \
		&& this->getItemBox()->getItem_lgc(x - left, y)->getActionState() == Fixed){
		left++;
	}
	left--;
	int right = 1;
	while (this->getItemBox()->getItem_lgc(x + right, y) != NULL && this->getItemBox()->getItem_lgc(x + right, y)->getItemType() == type \
		&& this->getItemBox()->getItem_lgc(x + right, y)->getActionState() == Fixed){
		right++;
	}
	right--;
	if (left + right + 1 >= 3){
		for (int i = 1; i <= left; i++){
			clearList.pushBack(this->getItemBox()->getItem_lgc(x - i, y));
		}
		for (int i = 1; i <= right; i++){
			clearList.pushBack(this->getItemBox()->getItem_lgc(x + i, y));
		}
	}

	//同时包括目标本身
	if (clearList.size() > 0){
		clearList.pushBack(this->getItemBox()->getItem_lgc(x, y));
	}
	return clearList;
}

void ClearController_L::createSkillItem(Vector<Entity*> clearList, Entity* item){
	ClearItem_L* item_L = (ClearItem_L*)item;

	int nowLevel = item_L->getItemLevel() + clearList.size() - 2;
	for (int i = 0; i < clearList.size(); i++){
		ClearItem_L* thisItem_L = (ClearItem_L*)clearList.at(i);
		if (thisItem_L != item){
			nowLevel += thisItem_L->getItemLevel();
		}
	}

	(item_L->setItemLevel(nowLevel));

}

void ClearController_L::triggerSkill(Entity* item, ItemType type){
	ClearItem_L* item_L = (ClearItem_L*)item;
	if (item_L->getItemLevel() <= 0){
		return;
	}

	if (item_L->getItemLevel() == 1){
		//dont't back
		NotificationCenter::getInstance()->postNotification("moveOver", NULL);
	}
	else if(item_L->getItemLevel() == 2){
		//将指定类型物体全变为本类型
		for (int j = 0; j < getItemBox()->getCellNum().y; j++){
			for (int i = 0; i < getItemBox()->getCellNum().x; i++){
				Entity* thisItem = getItemBox()->getItem_lgc(i, j);
				if (thisItem->getItemType() == type){
					((ClearItem_L*)thisItem)->bindSprite(ItemType(item->getItemType()));
					if (((Scanner_L*)getItemBox()->getScanner())->isClearable(thisItem)){
						//发送消除消息
						Vector<Entity*> list;
						list.pushBack(thisItem);
						PostDate* date = PostDate::create();
						date->setVector(list);
						date->retain();
						NotificationCenter::getInstance()->postNotification("clear", date);
					}
				}
			}
		}
	}
	else if (item_L->getItemLevel() == 3){
		//将指定类型物体等级全+1
		for (int j = 0; j < getItemBox()->getCellNum().y; j++){
			for (int i = 0; i < getItemBox()->getCellNum().x; i++){
				ClearItem_L* thisItem = (ClearItem_L*)getItemBox()->getItem_lgc(i, j);
				if (thisItem->getItemType() == type){
					thisItem->setItemLevel(thisItem->getItemLevel() + 1);
				}
			}
		}
		NotificationCenter::getInstance()->postNotification("moveOver", NULL);
	}
	item_L->setItemLevel(0);
}


void ClearController_L::clearEntity(Entity* item){
	((ClearItem_L*)item)->setItemLevel(0);
	((ClearItem_L*)item)->bindSprite(ItemType_CLEAN);
}
