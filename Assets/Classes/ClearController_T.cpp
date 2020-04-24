#include "ClearController_T.h"
#include "ItemBox_T.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"

ClearController_T::ClearController_T()
{
}

ClearController_T::~ClearController_T()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool ClearController_T::init(ItemBox_T* itemBox){
	this->bindItemBox(itemBox);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ClearController_T::clearListener), "clear", NULL);
	return true;
}

void ClearController_T::clearListener(Ref* date){
	//��������
	Vector<Entity*> clearList;
	Vector<Entity*> list = ((PostDate*)date)->getVector();
	Entity* lastItem = NULL;
	Entity* nowItem = NULL;

	//�жϵ����������ǽ�������
	if (list.size() == 1){
		lastItem = list.at(0);
	}
	else if (list.size() == 2){
		lastItem = list.at(0);
		nowItem = list.at(1);
	}
	else{
		CCASSERT(false, "ClearController_T::clearListener(Ref* date)�����������б��С�Ƿ���");
	}
	date->release();

	//��ȡ��һ������������б�
	Vector<Entity*> list1 = getClearList(lastItem);
	if (list1.size() > 0){
		clearList.pushBack(list1);
		//��ȡ�����б�
		if (createSkillItem(list1, lastItem)){
			//���ɶ�����
			clearList.eraseObject(lastItem);
		}
	}

	//��ȡ�ڶ�������������б�
	Vector<Entity*> list2 = getClearList(nowItem);
	if (list2.size() > 0){
		clearList.pushBack(list2);
		if (createSkillItem(list2, nowItem)){
			clearList.eraseObject(nowItem);
		}
	}

	//��ԭʼ��������д���
	//������breakItem��Ч��
	for (int i = 0; i < clearList.size(); i++){
		Entity* item = clearList.at(i);
		if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END){
			Entity* thisItem = NULL;
			Point pos = getItemBox()->getItemPos(item);
			if ((thisItem = getItemBox()->getItem_lgc(pos.x, pos.y + 1)) != NULL && thisItem->getItemType() == ItemType_Break){
				if (!clearList.contains(thisItem)){
					clearList.pushBack(thisItem);
				}
			}
			if ((thisItem = getItemBox()->getItem_lgc(pos.x, pos.y - 1)) != NULL && thisItem->getItemType() == ItemType_Break){
				if (!clearList.contains(thisItem)){
					clearList.pushBack(thisItem);
				}
			}
			if ((thisItem = getItemBox()->getItem_lgc(pos.x - 1, pos.y)) != NULL && thisItem->getItemType() == ItemType_Break){
				if (!clearList.contains(thisItem)){
					clearList.pushBack(thisItem);
				}
			}
			if ((thisItem = getItemBox()->getItem_lgc(pos.x + 1, pos.y)) != NULL && thisItem->getItemType() == ItemType_Break){
				if (!clearList.contains(thisItem)){
					clearList.pushBack(thisItem);
				}
			}
		}
	}



	//�������ܣ����㼼�ܲ����ķ�Χ�ڵ�Ŀ�꣬�����ظ��ذ����������б���
	for (int i = 0; i < clearList.size(); i++){
		Entity* it = clearList.at(i);
		Vector<Entity*> l = getSkillScope(it);
		for (const auto& it_l : l){
			if (clearList.contains(it_l) == false){
				clearList.pushBack(it_l);
			}
		}
	}

	//���������б�״̬ת����clean
	for (const auto& it : clearList){
		clearEntity(it);
	}

	//ͳ�Ʒ��������͵����ź�
	if (clearList.size() > 0){
		//������Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/clear.wav");

		NotificationCenter::getInstance()->postNotification("addGoal", (Ref*)clearList.size());
		NotificationCenter::getInstance()->postNotification("drop", NULL);
		NotificationCenter::getInstance()->postNotification("stopStaticTime", NULL);
	}
}

Vector<Entity*> ClearController_T::getClearList(Entity* item){
	Vector<Entity*> clearList;

	//�޷���ȡ������
	if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END \
		|| item->getActionState() != Fixed){
		return clearList;
	}

	//��ȡ�����߼�����
	ItemType type = ItemType(item->getItemType());
	int x = this->getItemBox()->getItemPos(item).x;
	int y = this->getItemBox()->getItemPos(item).y;

	//��ֱ��������������
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

	//ˮƽ��������������
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

	//ͬʱ����Ŀ�걾��
	if (clearList.size() > 0){
		clearList.pushBack(this->getItemBox()->getItem_lgc(x, y));
	}
	return clearList;
}

bool ClearController_T::createSkillItem(Vector<Entity*> clearList, Entity* item){
	//�����������������3����������Ŀ��
	if (clearList.size() > 3){
		//��Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/createSkill.wav");

		((ClearItem_T*)item)->setStateType(StateType_super);
		return true;
	}
	else{
		return false;
	}
}

Vector<Entity*> ClearController_T::getSkillScope(Entity* item){
	Vector<Entity*> clearList;

	//�ж��������
	if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END ){
		
		Point pos = getItemBox()->getItemPos(item);
		Entity* thisItem = NULL;

		if (((ClearItem_T*)item)->getStateType() == StateType_super){
			//����Ч��Ϊ��ʮ��ȫ������
			//��Ч
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/skill.mp3");
			//����
			Size visibleSize = Director::getInstance()->getVisibleSize();

			Sprite* show_x = Sprite::create("show_x.png");
			show_x->setPosition(visibleSize.width * 0.5 - convertToWorldSpace(getPosition()).x, item->getPosition().y);
			show_x->setScaleY(0.1);
			this->addChild(show_x);
			show_x->setGlobalZOrder(10);
			ActionInterval* fi_x = ScaleTo::create(0.05, 1.0, 1.0);
			ActionInterval* fo_x = FadeOut::create(0.2);
			CallFunc* cf_x = CallFunc::create([show_x](){
				show_x->removeFromParent();
			});
			auto action_x = Sequence::create(fi_x, fo_x, cf_x, NULL);
			show_x->runAction(action_x);

			Sprite* show_y = Sprite::create("show_y.png");
			show_y->setPosition(item->getPosition().x, visibleSize.height * 0.5 - convertToWorldSpace(getPosition()).y);
			show_y->setScaleX(0.1);
			this->addChild(show_y);
			show_y->setGlobalZOrder(10);
			ActionInterval* fi_y = ScaleTo::create(0.05, 1.0, 1.0);
			ActionInterval* fo_y = FadeOut::create(0.2);
			CallFunc* cf_y = CallFunc::create([show_y](){
				show_y->removeFromParent();
			});
			auto action_y = Sequence::create(fi_y, fo_y, cf_y, NULL);
			show_y->runAction(action_y);
			
			//����
			for (int i = 0; i < getItemBox()->getCellNum().x; i++){
				thisItem = getItemBox()->getItem_lgc(i, pos.y);
				//��������Ϊ�������������break����
				if ((thisItem->getItemType() > ClearItemType_BEGIN && thisItem->getItemType() < ClearItemType_END || thisItem->getItemType() == ItemType_Break)\
					&& thisItem->getActionState() != Droping){
					clearList.pushBack(getItemBox()->getItem_lgc(i, pos.y));
				}
			}
			for (int i = 0; i < getItemBox()->getCellNum().y; i++){
				thisItem = getItemBox()->getItem_lgc(pos.x, i);
				if ((thisItem->getItemType() > ClearItemType_BEGIN && thisItem->getItemType() < ClearItemType_END || thisItem->getItemType() == ItemType_Break) \
					&& thisItem->getActionState() != Droping){
					clearList.pushBack(getItemBox()->getItem_lgc(pos.x, i));
				}
			}
		}
	
	}

	return clearList;
}

void ClearController_T::clearEntity(Entity* item){
	//����Ч��״̬
	if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END){
		((ClearItem_T*)item)->setStateType(StateType_normal);
		((ClearItem_T*)item)->bindSprite(ItemType_CLEAN);
	}
	else{//��Ϊ����Ŀ����ת����������
		((Scanner_T*)getItemBox()->getScanner())->convertItemType(item, ItemType_CLEAN);
	}
}

void ClearController_T::getGoal(Vector<Entity*> clearList){
	NotificationCenter::getInstance()->postNotification("addGoal", (Ref*)clearList.size());
}