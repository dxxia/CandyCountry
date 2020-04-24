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
	//�󶨣�ע��
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
	//ת����moving״̬
	lastItem->setActionState(Moving);
	nowItem->setActionState(Moving);

	//ִ�н�������
	Point aimPos_last = nowItem->getPosition();
	Point aimPos_now = lastItem->getPosition();
	MoveTo* action_last = MoveTo::create(0.2f, aimPos_last);
	MoveTo* action_now = MoveTo::create(0.2f, aimPos_now);
	Action* moveToFix;

	//��������-> �ƶ�-�̶�-�ж��Ƿ�ص�-�ص�-�̶�
	moveToFix = Sequence::create(action_last, DelayTime::create(0.01),
			CallFunc::create([this, lastItem, nowItem, isCheck](){
				exAndFixCheck(lastItem, nowItem, isCheck); }), 
			NULL);

	lastItem->runAction(moveToFix);
	nowItem->runAction(action_now);
}

void TouchController_C::exAndFixCheck(Entity* lastItem, Entity* nowItem, bool isCheck){
	//�����߼�����
	Point nowIndex = this->getItemBox()->getItemPos(nowItem);
	Point lastIndex = this->getItemBox()->getItemPos(lastItem);
	this->getItemBox()->setItem(nowIndex.x, nowIndex.y, lastItem);
	this->getItemBox()->setItem(lastIndex.x, lastIndex.y, nowItem);

	//�̶�,ÿ�ι̶���Ҫ�������״̬
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
	}//�ص���־���������ֽ�����ص�����
	else if(isCheck){
		//������Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/moveBack.wav");

		exchange(lastItem, nowItem, false);
	}
}


void TouchController_C::touchListener(){
	auto listener = EventListenerTouchOneByOne::create();

	//ע���������
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		//ʱ�������ֹͣ���մ�������
		if (isTimeOver){
			return true;
		}

		//��ȡ���������������
		Point pos = this->getItemBox()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
		Entity* item = NULL;
		int touchedX, touchedY;
		touchedX = pos.x >= 0 ? (pos.x / getItemBox()->getCellSize().x) : -1;
		touchedY = pos.y >= 0 ? (pos.y / getItemBox()->getCellSize().y) : -1;
		if (touchedX < 0 || touchedX > getItemBox()->getCellNum().x || touchedY < 0 || touchedY > getItemBox()->getCellNum().y){
			return true;
		}
		item = this->getItemBox()->getItem_lgc(touchedX, touchedY);

		//�����������ѡȡ������ж����ж�
		if (item != NULL && item->getActionState() == Fixed){

			//��Ϊ��һ�δ���ѡȡ������ʾѡȡ����
			if (this->getLastTouchedItem() == NULL){
				item->onTouched();
				//��������ϴδ�������
				this->setLastTouchedItem(item);
				return true;
			}
			//Ϊ�ڶ���ѡȡ������ѡȡ��Ŀ�겻�ɽ���
			else if (!((Scanner_C*)getItemBox()->getScanner())->isMoveable(getLastTouchedItem(), item)){
				//������Ч
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

				this->getLastTouchedItem()->offTouched();
				//���ϴδ����������
				this->setLastTouchedItem(NULL);
				return true;
			}
			//׼���ƶ�
			else{
				this->getLastTouchedItem()->offTouched();
				Entity* lastItem = this->getLastTouchedItem();
				//���ϴδ����������
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
				//�ɽ������ƶ�
				if (((Scanner_C*)getItemBox()->getScanner())->isMoveable(lastItem, nowItem)){
					lastItem->offTouched();
					//���ϴδ����������
					setLastTouchedItem(NULL);
					this->exchange(lastItem, nowItem, true);
				}//���ɽ������ָ�
				else{
					//������Ч
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

					lastItem->offTouched();
					//���ϴδ����������
					this->setLastTouchedItem(NULL);
				}
			}
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this->getItemBox());
}
