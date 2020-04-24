#include "LogicLayer_L.h"
#include "ItemBox_L.h"
#include "AccountScene_L.h"

LogicLayer_L::LogicLayer_L()
{
}

LogicLayer_L::~LogicLayer_L()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool LogicLayer_L::init(){
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//加载物品格子
	m_itemBox = ItemBox_L::create();
	this->addChild(m_itemBox, 1);
	m_itemBox->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

	//游戏结束条件监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_L::stepEnd), "stepEnd", NULL);

	return true;
}


void LogicLayer_L::stepEnd(Ref*){
	//对所有的类型进行扫描，发现没有等级且个数少于3个的进行消除
	bool allNoLevel = true;//全场无等级物体
	bool isClearing = false;//正在消除标志
	Vector<Entity*> levelList;
	for (int k = ClearItemType_BEGIN + 1; k < ClearItemType_END; k++){
		Vector<Entity*> list;
		bool noLevel = true;
		for (int j = 0; j < m_itemBox->getCellNum().y; j++){
			for (int i = 0; i < m_itemBox->getCellNum().x; i++){
				ClearItem_L* item_L = (ClearItem_L*)m_itemBox->getItem_lgc(i, j);
				if (item_L->getItemType() == ItemType(k)){
					if (item_L->getItemLevel() == 0){
						list.pushBack((Entity*)item_L);
					}
					else if(item_L->getItemLevel() > 0){
						levelList.pushBack((Entity*)item_L);
						noLevel = false;
					}
				}
			}
		}
		if (noLevel){
			if (list.size() > 0 && list.size() < 3){
				for (int i = 0; i < list.size(); i++){
					((ClearController_L*)m_itemBox->getClearController())->clearEntity(list.at(i));
				}
				NotificationCenter::getInstance()->postNotification("drop", NULL);
				//有消除时不检查
				isClearing = true;
			}
		}
		else{
			allNoLevel = false;
		}
	}
	//进行游戏结束判断
	if (!isClearing && !((Scanner_L*)m_itemBox->getScanner())->isClearablePotentially()){
		log("no clearable");
		if (allNoLevel){
			gameOver();
		}
		else{
			//若所有有等级的物体均不可移动，则gameover
			for (int k = 0; k < levelList.size(); k++){
				Entity* item = levelList.at(k);
				int i = m_itemBox->getItemPos(item).x;
				int j = m_itemBox->getItemPos(item).y;
				if (((Scanner_L*)m_itemBox->getScanner())->isMoveable(item, m_itemBox->getItem_lgc(i, j + 1))\
					|| ((Scanner_L*)m_itemBox->getScanner())->isMoveable(item, m_itemBox->getItem_lgc(i, j - 1))\
					|| ((Scanner_L*)m_itemBox->getScanner())->isMoveable(item, m_itemBox->getItem_lgc(i - 1, j))\
					|| ((Scanner_L*)m_itemBox->getScanner())->isMoveable(item, m_itemBox->getItem_lgc(i + 1, j)))
				{
					break;
				}
				if (k == levelList.size() - 1){
					gameOver();
				}
			}
		}
	}
}

void LogicLayer_L::update(float dt){

}

void LogicLayer_L::gameOver(){
	int goal = 0;
	//根据消除个数计算得分
	for (int i = 0; i < m_itemBox->getCellNum().x; i++){
		for (int j = 0; j < m_itemBox->getCellNum().y; j++){
			if (m_itemBox->getItem_lgc(i, j)->getItemType() == ItemType_CLEAN){
				goal++;
			}
		}
	}

	Director::getInstance()->replaceScene(AccountScene_L::createScene(goal));
}