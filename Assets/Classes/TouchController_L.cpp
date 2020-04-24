#include "TouchController_L.h"
#include "ItemBox_L.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"

TouchController_L::TouchController_L()
{
	this->lastTouchedItem = NULL;
	isMoving = false;
}

TouchController_L::~TouchController_L()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}


void TouchController_L::moveOver(Ref*){
	isMoving = false;
}

bool TouchController_L::init(ItemBox_L* itemBox){
	//绑定，注册
	bindItemBox(itemBox);
	this->touchListener();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TouchController_L::moveOver), "moveOver", NULL);

	return true;
}

Entity* TouchController_L::getLastTouchedItem(){
	return this->lastTouchedItem;
}
void TouchController_L::setLastTouchedItem(Entity* item){
	this->lastTouchedItem = item;
}

void TouchController_L::exchange(Entity* lastItem, Entity* nowItem, bool isCheck){

	//执行交换动作
	Point aimPos_last = nowItem->getPosition();
	Point aimPos_now = lastItem->getPosition();
	MoveTo* action_last = MoveTo::create(0.2f, aimPos_last);
	MoveTo* action_now = MoveTo::create(0.2f, aimPos_now);
	Action* moveToFix;

	//动作序列-> 移动-固定-判断是否回弹-回弹-固定
	moveToFix = Sequence::create(action_last, DelayTime::create(0.01),
		CallFunc::create([this, lastItem, nowItem, isCheck](){
		exAndFixCheck(lastItem, nowItem, isCheck); }),
			NULL);

		lastItem->runAction(moveToFix);
		nowItem->runAction(action_now);
}

void TouchController_L::exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck){
	//交换逻辑坐标
	Point nowIndex = this->getItemBox()->getItemPos(nowItem);
	Point lastIndex = this->getItemBox()->getItemPos(lastItem);
	this->getItemBox()->setItem(nowIndex.x, nowIndex.y, lastItem);
	this->getItemBox()->setItem(lastIndex.x, lastIndex.y, nowItem);

	//判断是第一次移动还是回弹
	if (isCheck){
		//要判断是否可消除
		if (((Scanner_L*)getItemBox()->getScanner())->isClearable(lastItem) || ((Scanner_L*)getItemBox()->getScanner())->isClearable(nowItem)){
			//发送消除消息
			Vector<Entity*> list;
			list.pushBack(lastItem);
			list.pushBack(nowItem);
			PostDate* date = PostDate::create();
			date->setVector(list);
			date->retain();
			NotificationCenter::getInstance()->postNotification("clear", date);
		}//不可消除，回弹
		else{
			ClearItem_L* l = (ClearItem_L*)lastItem;
			ClearItem_L* n = (ClearItem_L*)nowItem;
			//若存在技能目标，则发送消除请求,直接触发技能
			if (l->getItemLevel() > 0 ){
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/skillMove.wav");

				l->setItemLevel(l->getItemLevel() - 1);
				isMoving = false;
				NotificationCenter::getInstance()->postNotification("stepEnd", NULL);
			}
			else if (n->getItemLevel() > 0){
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/skillMove.wav");

				n->setItemLevel(n->getItemLevel() - 1);
				isMoving = false;
				NotificationCenter::getInstance()->postNotification("stepEnd", NULL);
			}
			else{
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/moveBack.wav");

				exchange(lastItem, nowItem, false);
			}
		}
	}//回弹结束
	else
	{
		isMoving = false;
	}
}

void TouchController_L::touchListener(){
	auto listener = EventListenerTouchOneByOne::create();

	//注册监听函数
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		//移动中，停止接收触摸动作
		if (isMoving){
			return true;
		}
		//获取触摸处的物体对象
		Point pos = this->getItemBox()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
		Entity* item = NULL;
		int touchedX, touchedY;
		touchedX = pos.x >= 0 ? (pos.x / getItemBox()->getCellSize().x) : -1;
		touchedY = pos.y >= 0 ? (pos.y / getItemBox()->getCellSize().y) : -1;
		if (touchedX < 0 || touchedX > getItemBox()->getCellNum().x || touchedY < 0 || touchedY > getItemBox()->getCellNum().y){
			return true;
		}
		item = this->getItemBox()->getItem_lgc(touchedX, touchedY);


		//若为第一次触摸选取对象，显示选取动作
		if (this->getLastTouchedItem() == NULL){
			//若该物体可以选取，则进行动作判断
			if (item != NULL && item->getActionState() == Fixed){
				item->onTouched();
				//将其存入上次触摸数据
				this->setLastTouchedItem(item);
				return true;
			}
		}
		//为第二次选取，判断是否可交换
		else{
			//可交换，移动
			if (((Scanner_L*)getItemBox()->getScanner())->isMoveable(getLastTouchedItem(), item)){
				isMoving = true;
				Entity* lastItem = this->getLastTouchedItem();
				lastItem->offTouched();
				//将上次触摸数据清空
				setLastTouchedItem(NULL);
				this->exchange(lastItem, item, true);
			}//不可交换，恢复
			else{
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

				this->getLastTouchedItem()->offTouched();
				//将上次触摸数据清空
				this->setLastTouchedItem(NULL);
				return true;
			}

		}
		return true;
	};

	listener->onTouchMoved = [&](Touch* touch, Event* event){
		Entity* lastItem = NULL;
		if ((lastItem = getLastTouchedItem()) != NULL){
			Point pos = getItemBox()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
			Entity* nowItem = getItemBox()->getItem_lgc(pos.x / getItemBox()->getCellSize().x, pos.y / getItemBox()->getCellSize().y);
			if (nowItem != NULL && nowItem != lastItem){
				//可交换，移动
				if (((Scanner_L*)getItemBox()->getScanner())->isMoveable(lastItem, nowItem)){
					isMoving = true;
					lastItem->offTouched();
					//将上次触摸数据清空
					setLastTouchedItem(NULL);
					this->exchange(lastItem, nowItem, true);
				}//不可交换，恢复
				else{
					//播放音效
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

					lastItem->offTouched();
					//将上次触摸数据清空
					this->setLastTouchedItem(NULL);
				}
			}
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this->getItemBox());
}
