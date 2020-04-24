#include "TouchController_T.h"
#include "ItemBox_T.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"

TouchController_T::TouchController_T()
{
	this->lastTouchedItem = NULL;
	isMoving = false;
}

TouchController_T::~TouchController_T()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}


void TouchController_T::moveOver(Ref*){
	isMoving = false;
}

bool TouchController_T::init(ItemBox_T* itemBox){
	//�󶨣�ע��
	bindItemBox(itemBox);
	this->touchListener();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TouchController_T::moveOver), "moveOver", NULL);

	return true;
}

Entity* TouchController_T::getLastTouchedItem(){
	return this->lastTouchedItem;
}

void TouchController_T::setLastTouchedItem(Entity* item){
	this->lastTouchedItem = item;
}


void TouchController_T::exchange(Entity* lastItem, Entity* nowItem, bool isCheck){

	//ִ�н�������
	Point aimPos_last = nowItem->getPosition();
	Point aimPos_now = lastItem->getPosition();
	MoveTo* action_last = MoveTo::create(0.2f, aimPos_last);
	MoveTo* action_now = MoveTo::create(0.2f, aimPos_now);
	Action* moveToFix;

	//��������-> �ƶ�-�̶�-�ж��Ƿ�ص�-�ص�-�̶�
	moveToFix = Sequence::create(action_last, DelayTime::create(0.01),
		CallFunc::create([this, lastItem, nowItem, isCheck](){
		fixCheck(lastItem, nowItem, isCheck); }),
			NULL);

	lastItem->runAction(moveToFix);
	nowItem->runAction(action_now);
}

void TouchController_T::fixCheck(Entity* lastItem, Entity* nowItem, bool isCheck){
	//�����߼�����
	Point nowIndex = this->getItemBox()->getItemPos(nowItem);
	Point lastIndex = this->getItemBox()->getItemPos(lastItem);
	this->getItemBox()->setItem(nowIndex.x, nowIndex.y, lastItem);
	this->getItemBox()->setItem(lastIndex.x, lastIndex.y, nowItem);

	//�ж��ǵ�һ���ƶ����ǻص�
	if (isCheck){
		//�̶�,�̶���Ҫ�ж��Ƿ������
		if (((Scanner_T*)getItemBox()->getScanner())->isClearable(lastItem) || ((Scanner_T*)getItemBox()->getScanner())->isClearable(nowItem)){
			//����������Ϣ
			Vector<Entity*> list;
			list.pushBack(lastItem);
			list.pushBack(nowItem);
			PostDate* date = PostDate::create();
			date->setVector(list);
			date->retain();
			NotificationCenter::getInstance()->postNotification("clear", date);
			//��������
			//stepEnd
			NotificationCenter::getInstance()->postNotification("updateStep", NULL);

		}//�����������ص�
		else{
			//������Ч
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/moveBack.wav");

			exchange(lastItem, nowItem, false);
		}
	}//�ص�����
	else
	{
		isMoving = false;
	}
}

void TouchController_T::touchListener(){
	auto listener = EventListenerTouchOneByOne::create();

	//ע���������
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		//�����þ����ƶ��У�ֹͣ���մ�������
		if (isMoving){
			return true;
		}
		//��ȡ���������������
		Point pos = this->getItemBox()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
		Entity* item = NULL;
		int touchedX, touchedY;
		touchedX = pos.x >= 0?(pos.x / getItemBox()->getCellSize().x):-1;
		touchedY = pos.y >= 0?(pos.y / getItemBox()->getCellSize().y):-1;
		if (touchedX < 0 || touchedX > getItemBox()->getCellNum().x || touchedY < 0 || touchedY > getItemBox()->getCellNum().y){
			return true;
		}
		item = this->getItemBox()->getItem_lgc(touchedX, touchedY);
		

		//��Ϊ��һ�δ���ѡȡ������ʾѡȡ����
		if (this->getLastTouchedItem() == NULL){
			//�����������ѡȡ������ж����ж�
			if (item != NULL && item->getItemType() != ItemType_NULL && item->getItemType() != ItemType_Break &&item->getActionState() == Fixed){
				item->onTouched();
				//��������ϴδ�������
				this->setLastTouchedItem(item);
				return true;
			}
		}
		//Ϊ�ڶ���ѡȡ���ж��Ƿ�ɽ���
		else{
			//�ɽ������ƶ�
			if (((Scanner_T*)getItemBox()->getScanner())->isMoveable(getLastTouchedItem(), item)){
				isMoving = true;					
				Entity* lastItem = this->getLastTouchedItem();
				this->getLastTouchedItem()->offTouched();
				//���ϴδ����������
				setLastTouchedItem(NULL);
				this->exchange(lastItem, item, true);
			}//���ɽ������ָ�
			else{
				//������Ч
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/notSwap.mp3");

				this->getLastTouchedItem()->offTouched();
				//���ϴδ����������
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
				//�ɽ������ƶ�
				if (((Scanner_T*)getItemBox()->getScanner())->isMoveable(lastItem, nowItem)){
					isMoving = true;
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

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this->getItemBox());
}
