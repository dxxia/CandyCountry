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
	//��������
	Vector<Entity*> clearList(((PostDate*)date)->getVector());
	date->release();
	
	//��ȡ�����б�
	findDropList(clearList);

	//���������������б�
	createDropList(clearList);

	//�����˶�״̬Ϊdroping
	for (int i = 0; i < getItemBox()->getCellNum().x; i++){
		for (const auto& item : dropList.at(i)){
			item->setActionState(Droping);
		}
	}
}

void DropController_C::findDropList(Vector<Entity*> clearList){
	//����һ��clearList
	Vector<Entity*> clearList_copy(clearList);

	//�����µ����ϱ�������ȡdroplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//�������е�����
			if (clearList_copy.size() == 0){
				return;
			}

			//������������
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() != ItemType_CLEAN || !clearList_copy.contains(item)){
				continue;
			}

			//�ҵ����Ϸ��Ŀɵ�������
			int k;
			for (k = 0; (item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getItemType() == ItemType_CLEAN \
				&& clearList_copy.contains(item); k++){
				clearList_copy.eraseObject(item);
			}

			//���Ϸ�������ѹ���Ӧdroplist��Ӧ���ǰ��
			for (int q = 0; (item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getActionState() != Droping;  k++){
				if (item->getItemType() != ItemType_CLEAN){
					dropList.at(i).insert(q++, item);
				}
				else{//������������ɼ���ʱ�Ĳ�����
					clearList_copy.eraseObject(item);
				}
			}
		}
	}
}

void DropController_C::createDropList(Vector<Entity*> clearList){
	//����һ��clearList
	Vector<Entity*> clearList_copy(clearList);

	//�����µ����ϱ�������ȡdroplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//�������е�����
			if (clearList_copy.size() == 0){
				return;
			}

			//������������
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() != ItemType_CLEAN || !clearList_copy.contains(item)){
				continue;
			}

			//ͳ�Ƶ�ǰ���ٵ��������ͣ�������
			ItemType leastType = ((Scanner_C*)getItemBox()->getScanner())->getLeastClearItemType();
			((ClearItem_C*)item)->bindSprite(leastType);

			//ȷ��λ��
			Point topPoint;//�����б�β�˶������������
			//��β�˶����������Ϸ���������λ��Ϊ���
			if (dropList.at(i).size() > 0 && dropList.at(i).back()->getPosition().y > (getItemBox()->getCellSize().y * (0.5f + (getItemBox()->getCellNum().y - 1)))){
				topPoint = dropList.at(i).back()->getPosition();
			}//��������λ��Ϊ�����Ϸ�
			else{
				topPoint = Point(getItemBox()->getCellSize().x * (0.5 + (float)i), getItemBox()->getCellSize().y * (0.5 + (float)(getItemBox()->getCellNum().y - 1)));
			}
			item->setPosition(topPoint.x, topPoint.y + getItemBox()->getCellSize().y);

			//�����ɵ�����ѹ��droplist��Ӧ���β��,����clearList_copy���Ƴ�
			dropList.at(i).pushBack(item);
			clearList_copy.eraseObject(item);
		}
	}
}

void DropController_C::update(float dt){

	//���µ���λ�ú�״̬
	bool anyFixed = false;
	//�����ұ���
	for (int i = 0; i < getItemBox()->getCellNum().x; i++){
		//����ÿ��
		Vector<Entity*> dropListSingle(dropList.at(i));
		while (dropListSingle.size() != 0){
			//��ÿ�е�һ�����м��
			Entity* item = dropListSingle.front();
			dropListSingle.eraseObject(item);
			if (checkAndFix(item, true)){//�̶�
				anyFixed = true;
				dropList.at(i).eraseObject(item);//���б����Ƴ�
				//�����������ж��޺����������������ź�
				Scanner_C* scanner = (Scanner_C*)getItemBox()->getScanner();
				if (scanner->isClearable(item)){
					Point pos = getItemBox()->getItemPos(item);

					//��ֱ���������
					if (scanner->getSameTowClearItemType(pos.x, pos.y, down) == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1) != NULL\
						&& ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1)->getItemType() == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x, pos.y + 1)->getActionState() == Droping){
						//�Ϸ��к����������������Դ˴�����
							continue;
					}//ˮƽ���������
					else if (scanner->getSameTowClearItemType(pos.x, pos.y, left) == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y) != NULL\
						&& ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y)->getItemType() == item->getItemType() && ((ItemBox_C*)getItemBox())->getItem_lgc2phy(pos.x + 1, pos.y)->getActionState() == Droping\
						&& checkAndFix(dropList.at(pos.x + 1).front(), false)){
						//�ҷ��к����������������Դ˴�����
							continue;
					}//���޺�������������м�ʱ����
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
			else{//����
				item->setPosition(item->getPosition().x, item->getPosition().y - dt * dropSpeed);//�޸�λ��
			}
		}
	}

	//�ôθ���������̶������Ƿ������ϣ���������������Ƿ�������������
	if (anyFixed){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (dropList.at(i).size() > 0){
				break;
			}
			if (i == getItemBox()->getCellNum().x - 1){//������ϣ����
				((Scanner_C*)getItemBox()->getScanner())->checkAndRelocate();
			}
		}
	}
}

bool DropController_C::checkAndFix(Entity* item, bool isFixed){
	CCASSERT(item != NULL, "DropController_C::checkAndFix(ClearItem_C* item)���������Ϊ�գ�");

	//��ȡ��������������
	Point pos = item->getPosition();
	pos.y = pos.y - getItemBox()->getCellSize().y / 2;
	int x = pos.x / getItemBox()->getCellSize().x;
	CCASSERT(x >= 0 && x <= getItemBox()->getCellSize().x, "DropController_C::checkAndFix(ClearItem_C* item)��������x��������Ƿ���");
	int y;
	if (pos.y <= 0){//int(-0.x) == 0
		y = -1;
	}
	else{
		y = pos.y / getItemBox()->getCellSize().y;
	}

	//�ڵ�ͼ�Ϸ�,��������
	if (y >= getItemBox()->getCellNum().y){
		return false;
	}//�ڵ�ͼ������Χ
	else if(y >= 0){
		Entity* belowItem = getItemBox()->getItem_lgc(x, y);
		//�·�Ϊ���յ���״̬�����������״̬
		if (belowItem->getActionState() == Droping){
			return false;
		}
	}
	//�·��̶������������·�����̶����뵱ǰλ�õĶ��󽻻��߼�����
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