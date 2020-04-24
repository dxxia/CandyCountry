#include "TouchController_C.h"
#include "ItemBox_C.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"

TouchController_C::TouchController_C()
{
	this->lastTouchedItem = NULL;
	isTimeOver = false;
}

TouchController_C::~TouchController_C()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}


void TouchController_C::timeOver(Ref*){
	isTimeOver = true;
}

bool TouchController_C::init(ItemBox_C* itemBox){
	//绑定，注册
	bindItemBox(itemBox);
	this->touchListener();
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(TouchController_C::timeOver),"timeOver",NULL);

	return true;
}

Entity* TouchController_C::getLastTouchedItem(){
	return this->lastTouchedItem;
}
void TouchController_C::setLastTouchedItem(Entity* item){
	this->lastTouchedItem = item;
}

void TouchController_C::exchange(Entity* lastItem, Entity* nowItem, bool isCheck){
	//转换成moving状态
	lastItem->setActionState(Moving);
	nowItem->setActionState(Moving);

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

void TouchController_C::exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck){
	//交换逻辑坐标
	Point nowIndex = this->getItemBox()->getItemPos(nowItem);
	Point lastIndex = this->getItemBox()->getItemPos(lastItem);
	this->getItemBox()->setItem(nowIndex.x, nowIndex.y, lastItem);
	this->getItemBox()->setItem(lastIndex.x, lastIndex.y, nowItem);

	//固定,每次固定都要检查消除状态
	lastItem->setActionState(Fixed);
	nowItem->setActionState(Fixed);
	if (((Scanner_C*)getItemBox()->getScanner())->isClearable(lastItem) || ((Scanner_C*)getItemBox()->getScanner())->isClearable(nowItem)){
		Vector<Entity*> list;
		list.pushBack(lastItem);
		list.pushBack(nowItem);
		PostDate* date = PostDate::create();
		date->setVector(list);
		date->retain();
		NotificationCenter::getInstance()->postNotification("clear", date);
	}//回弹标志，用于区分交换与回弹动作
	else if(isCheck){
		//播放音效
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/moveBack.wav");

		exchange(lastItem, nowItem, false);
	}
}


void TouchController_C::touchListener(){
	auto listener = EventListenerTouchOneByOne::create();

	//注册监听函数
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		//时间结束，停止接收触摸动作
		if (isTimeOver){
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

		//若该物体可以选取，则进行动作判断
		if (item != NULL && item->getActionState() == Fixed){

			//若为第一次触摸选取对象，显示选取动作
			if (this->getLastTouchedItem() == NULL){
				item->onTouched();
				//将其存入上次触摸数据
				this->setLastTouchedItem(item);
				return true;
			}
			//为第二次选取，两次选取的目标不可交换
			else if (!((Scanner_C*)getItemBox()->getScanner())->isMoveable(getLastTouchedItem(), item)){
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

				this->getLastTouchedItem()->offTouched();
				//将上次触摸数据清空
				this->setLastTouchedItem(NULL);
				return true;
			}
			//准备移动
			else{
				this->getLastTouchedItem()->offTouched();
				Entity* lastItem = this->getLastTouchedItem();
				//将上次触摸数据清空
				setLastTouchedItem(NULL);
				this->exchange(lastItem, item, true);
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
				if (((Scanner_C*)getItemBox()->getScanner())->isMoveable(lastItem, nowItem)){
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

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this->getItemBox());
}
