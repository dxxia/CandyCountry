#include "DropController_C.h"
#include "ItemBox_C.h"
#include "PostDate.h"

DropController_C::DropController_C()
{
	this->scheduleUpdate();

}

DropController_C::~DropController_C()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool DropController_C::init(ItemBox_C* itemBox){
	bindItemBox(itemBox);
	for (int i = 0; i < getItemBox()->getCellNum().x; i++){
		Vector<Entity*> a;
		a.clear();
		dropList.push_back(a);
	}
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DropController_C::dropListener), "drop", NULL);

	return true;
}

void DropController_C::dropListener(Ref* date){
	//处理数据
	Vector<Entity*> clearList(((PostDate*)date)->getVector());
	date->release();
	
	//获取掉落列表
	findDropList(clearList);

	//将生成项加入掉落列表
	createDropList(clearList);

	//设置运动状态为droping
	for (int i = 0; i < getItemBox()->getCellNum().x; i++){
		for (const auto& item : dropList.at(i)){
			item->setActionState(Droping);
		}
	}
}

void DropController_C::findDropList(Vector<Entity*> clearList){
	//复制一份clearList
	Vector<Entity*> clearList_copy(clearList);

	//从左下到右上遍历，获取droplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//找完所有掉落项
			if (clearList_copy.size() == 0){
				return;
			}

			//搜索已消除项
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() != ItemType_CLEAN || !clearList_copy.contains(item)){
				continue;
			}

			//找到其上方的可掉落物体
			int k;
			for (k = 0; (item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getItemType() == ItemType_CLEAN \
				&& clearList_copy.contains(item); k++){
				clearList_copy.eraseObject(item);
			}

			//将上方掉落项压入对应droplist对应组的前段
			for (int q = 0; (item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getActionState() != Droping;  k++){
				if (item->getItemType() != ItemType_CLEAN){
					dropList.at(i).insert(q++, item);
				}
				else{//跳过消除项（生成技能时的残留）
					clearList_copy.eraseObject(item);
				}
			}
		}
	}
}

void DropController_C::createDropList(Vector<Entity*> clearList){
	//复制一份clearList
	Vector<Entity*> clearList_copy(clearList);

	//从左下到右上遍历，获取droplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//找完所有掉落项
			if (clearList_copy.size() == 0){
				return;
			}

			//搜索已消除项
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() != ItemType_CLEAN || !clearList_copy.contains(item)){
				continue;
			}

			//统计当前最少的物体类型，并生成
			ItemType leastType = ((Scanner_C*)getItemBox()->getScanner())->getLeastClearItemType();
			((ClearItem_C*)item)->bindSprite(leastType);

			//确定位置
			Point topPoint;//掉落列表尾端对象的物理坐标
			//若尾端对象在容器上方，则生成位置为其后
			if (dropList.at(i).size() > 0 && dropList.at(i).back()->getPosition().y > (getItemBox()->getCellSize().y * (0.5f + (getItemBox()->getCellNum().y - 1)))){
				topPoint = dropList.at(i).back()->getPosition();
			}//否则，生成位置为容器上方
			else{
				topPoint = Point(getItemBox()->getCellSize().x * (0.5 + (float)i), getItemBox()->getCellSize().y * (0.5 + (float)(getItemBox()->getCellNum().y - 1)));
			}
			item->setPosition(topPoint.x, topPoint.y + getItemBox()->getCellSize().y);

			//将生成掉落项压入droplist对应组的尾端,并从clearList_copy中移除
			dropList.at(i).pushBack(item);
			clearList_copy.eraseObject(item);
		}
	}
}

void DropController_C::update(float dt){

	//更新掉落位置和状态
	bool anyFixed = false;
	//从左到右遍历
	for (int i = 0; i < getItemBox()->getCellNum().x; i++){
		//更新每列
		Vector<Entity*> dropListSingle(dropList.at(i));
		while (dropListSingle.size() != 0){
			//对每列第一个进行检查
			Entity* item = dropListSingle.front();
			dropListSingle.eraseObject(item);
			if (checkAndFix(item, true)){//固定
				anyFixed = true;
				dropList.at(i).eraseObject(item);//从列表中移除
				//若可消除，判断无后续消除则发送消除信号
				Scanner_C* scanner = (Scanner_C*)getItemBox()->getScanner();
				if (scanner->isClearable(item)){
					Point pos = getItemBox()->getItemPos(item);

					//竖直方向可消除
					if (scanner->getSameTowClearItemType(pos.x, pos.y, down) == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1) != NULL\
						&& ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1)->getItemType() == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1)->getActionState() == Droping){
						//上方有后续掉落消除，忽略此次消除
							continue;
					}//水平方向可消除
					else if (scanner->getSameTowClearItemType(pos.x, pos.y, left) == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y) != NULL\
						&& ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y)->getItemType() == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y)->getActionState() == Droping\
						&& checkAndFix(dropList.at(pos.x + 1).front(), false)){
						//右方有后续掉落消除，忽略此次消除
							continue;
					}//均无后续消除，则进行即时消除
					else{
						Vector<Entity*> list;
						list.pushBack(item);
						PostDate* date = PostDate::create();
						date->setVector(list);
						date->retain();
						NotificationCenter::getInstance()->postNotification("clear", date);
					}
				}
			}
			else{//掉落
				item->setPosition(item->getPosition().x, item->getPosition().y - dt * dropSpeed);//修改位置
			}
		}
	}

	//该次更新有物体固定则检查是否掉落完毕，若掉落完毕则检查是否存在消除项并重排
	if (anyFixed){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (dropList.at(i).size() > 0){
				break;
			}
			if (i == getItemBox()->getCellNum().x - 1){//掉落完毕，检查
				((Scanner_C*)getItemBox()->getScanner())->checkAndRelocate();
			}
		}
	}
}

bool DropController_C::checkAndFix(Entity* item, bool isFixed){
	CCASSERT(item != NULL, "DropController_C::checkAndFix(ClearItem_C* item)：掉落项不能为空！");

	//获取掉落项物理坐标
	Point pos = item->getPosition();
	pos.y = pos.y - getItemBox()->getCellSize().y / 2;
	int x = pos.x / getItemBox()->getCellSize().x;
	CCASSERT(x >= 0 && x <= getItemBox()->getCellSize().x, "DropController_C::checkAndFix(ClearItem_C* item)：掉落项x物理坐标非法！");
	int y;
	if (pos.y <= 0){//int(-0.x) == 0
		y = -1;
	}
	else{
		y = pos.y / getItemBox()->getCellSize().y;
	}

	//在地图上方,继续掉落
	if (y >= getItemBox()->getCellNum().y){
		return false;
	}//在地图正常范围
	else if(y >= 0){
		Entity* belowItem = getItemBox()->getItem_lgc(x, y);
		//下方为悬空掉落状态，则继续掉落状态
		if (belowItem->getActionState() == Droping){
			return false;
		}
	}
	//下方固定或者在容器下方，则固定，与当前位置的对象交换逻辑坐标
	if (isFixed){
		y++;
		item->setPosition(getItemBox()->getCellSize().x * ((float)x + 0.5), getItemBox()->getCellSize().y * ((float)y + 0.5));
		item->setActionState(Fixed);
		Entity* beforeItem = getItemBox()->getItem_lgc(x, y);
		Point nowPos = getItemBox()->getItemPos(item);
		getItemBox()->setItem(x, y, item);
		getItemBox()->setItem(nowPos.x, nowPos.y, beforeItem);
	}
	return true;
}