#include "Scanner_T.h"
#include "ItemBox_T.h"

Scanner_T::Scanner_T()
{
}

Scanner_T::~Scanner_T()
{
}

bool Scanner_T::init(ItemBox_T* itemBox){
	this->bindItemBox(itemBox);
	return true;
}

ItemType Scanner_T::getSameTowClearItemType(int x, int y, Direction direction){
	ItemType type = ClearItemType_END;
	Entity* item = NULL;

	//�жϲ�ͬ����
	switch (direction)
	{
	case up:
		break;
	case down:
		item = this->getItemBox()->getItem_lgc(x, y - 1);
		if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END ||item->getActionState() == Droping){
			break;
		}
		type = ItemType(item->getItemType());
		item = this->getItemBox()->getItem_lgc(x, y - 2);
		if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END || item->getActionState() == Droping){
			break;
		}
		if (type == item->getItemType()){
			return type;
		}
		else{
			break;
		}
	case left:
		item = this->getItemBox()->getItem_lgc(x - 1, y);
		if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END || item->getActionState() == Droping){
			break;
		}
		type = ItemType(item->getItemType());
		item = this->getItemBox()->getItem_lgc(x - 2, y);
		if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END || item->getActionState() == Droping){
			break;
		}
		if (type == item->getItemType()){
			return type;
		}
		else{
			break;
		}
	case right:
		break;
	default:
		break;
	}

	return ClearItemType_END;
}

bool Scanner_T::isClearablePotentially(){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//����ÿ�����󣬲�����ģ���ƶ�Ѱ��Ǳ�ڿ�������
			Entity* anotherItem = NULL;
			Entity* item = getItemBox()->getItem_lgc(i, j);

			//ģ������
			if ((anotherItem = getItemBox()->getItem_lgc(i - 1, j)) != NULL && isMoveable(item, anotherItem)){
				getItemBox()->setItem(i - 1, j, item);
				getItemBox()->setItem(i, j, anotherItem);
				if (isClearable(item) || isClearable(anotherItem)){
					getItemBox()->setItem(i, j, item);
					getItemBox()->setItem(i - 1, j, anotherItem);
					return true;
				}
				getItemBox()->setItem(i, j, item);
				getItemBox()->setItem(i - 1, j, anotherItem);
			}

			//ģ������
			if ((anotherItem = getItemBox()->getItem_lgc(i, j - 1)) != NULL && isMoveable(item, anotherItem)){
				getItemBox()->setItem(i, j - 1, item);
				getItemBox()->setItem(i, j, anotherItem);
				if (isClearable(item) || isClearable(anotherItem)){
					getItemBox()->setItem(i, j, item);
					getItemBox()->setItem(i, j - 1, anotherItem);
					return true;
				}
				getItemBox()->setItem(i, j, item);
				getItemBox()->setItem(i, j - 1, anotherItem);
			}
		}
	}
	return false;
}

void Scanner_T::getPotentialItem(Entity* &item1, Entity* &item2){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//����ÿ�����󣬲�����ģ���ƶ�Ѱ��Ǳ�ڿ�������
			Entity* anotherItem = NULL;
			Entity* item = getItemBox()->getItem_lgc(i, j);

			//ģ������
			if ((anotherItem = getItemBox()->getItem_lgc(i - 1, j)) != NULL && isMoveable(item, anotherItem)){
				getItemBox()->setItem(i - 1, j, item);
				getItemBox()->setItem(i, j, anotherItem);
				if (isClearable(item) || isClearable(anotherItem)){
					getItemBox()->setItem(i, j, item);
					getItemBox()->setItem(i - 1, j, anotherItem);
					item1 = item;
					item2 = anotherItem;
					return;
				}
				getItemBox()->setItem(i, j, item);
				getItemBox()->setItem(i - 1, j, anotherItem);
			}

			//ģ������
			if ((anotherItem = getItemBox()->getItem_lgc(i, j - 1)) != NULL && isMoveable(item, anotherItem)){
				getItemBox()->setItem(i, j - 1, item);
				getItemBox()->setItem(i, j, anotherItem);
				if (isClearable(item) || isClearable(anotherItem)){
					getItemBox()->setItem(i, j, item);
					getItemBox()->setItem(i, j - 1, anotherItem);
					item1 = item;
					item2 = anotherItem;
					return;
				}
				getItemBox()->setItem(i, j, item);
				getItemBox()->setItem(i, j - 1, anotherItem);
			}
		}
	}
}

void Scanner_T::relocateItems(Vector<Entity*> items, std::vector<Point> points){
	//��ȡϵͳʱ��,�����������
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //��ת��Ϊ΢�� 
	srand(rand_seed);

	CCASSERT(items.size() == points.size(), "Scanner_T::relocateItems(Vector<Entity_T*> items)�����·���Ķ�����Ŀ�Ƿ���");
	//�������������������������
	bool isRepeat;
	do{
		Vector<Entity*> items_copy(items);
		isRepeat = false;
		//��������������������
		for (const auto& point : points){

			//���˴�ѭ���ڴ�λ�ò����ڲ������������У����������ѭ������ֹ��������ѭ����
			int k;
			for (k = 0; k < items_copy.size(); k++){
				if (items_copy.at(k)->getItemType() != getSameTowClearItemType(point.x, point.y, down) && items_copy.at(k)->getItemType() != getSameTowClearItemType(point.x, point.y, left)){
					break;
				}
			}
			if (k == items_copy.size()){
				isRepeat = true;
				break;
			}

			//��ʼ���,�õ�����������
			Entity* item = NULL;
			do{
				item = items_copy.at(rand() % items_copy.size());
			} while (item->getItemType() == getSameTowClearItemType(point.x, point.y, down) || item->getItemType() == getSameTowClearItemType(point.x, point.y, left));

			//��֮ǰ���߼���������彻���߼�����,������������,����copy���Ƴ�
			Entity* beforeItem = getItemBox()->getItem_lgc(point.x, point.y);
			Point pos = getItemBox()->getItemPos(item);
			getItemBox()->setItem(point.x, point.y, item);
			getItemBox()->setItem(pos.x, pos.y, beforeItem);
			item->setPosition(getItemBox()->getCellSize().x * (point.x + 0.5), getItemBox()->getCellSize().y * (point.y + 0.5));
			items_copy.eraseObject(item);
		}
		//����Ǳ�����������������
		if (!isRepeat && !isClearablePotentially()){
			isRepeat = true;
		}
	} while (isRepeat);//�������
}

bool Scanner_T::isClearable(Entity* item){
	//�����������޷�����
	if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END \
		|| item->getActionState() != Fixed){
		return false;
	}

	//��ȡ�߼�����
	ItemType type = ItemType(item->getItemType());
	int x = this->getItemBox()->getItemPos(item).x;
	int y = this->getItemBox()->getItemPos(item).y;

	//��ֱ�����ж�
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
		return true;
	}

	//ˮƽ�����ж�
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
		return true;
	}

	return false;
}

bool Scanner_T::isMoveable(Entity* lastItem, Entity* nowItem){
	CCASSERT(lastItem != NULL && nowItem != NULL, "TouchController_T::isMoveable(Entity_T* lastItem, Entity_T* nowItem)���ж�Ŀ�겻��Ϊ�գ�");

	//���Ͳ�ΪItemType_NULL
	if (lastItem->getItemType() == ItemType_NULL || nowItem->getItemType() == ItemType_NULL){
		return false;
	}

	//���Ͳ�ΪItemType_Clean
	if (lastItem->getItemType() == ItemType_CLEAN || nowItem->getItemType() == ItemType_CLEAN){
		return false;
	}

	//���Ͳ�ΪItemType_Break
	if (lastItem->getItemType() == ItemType_Break || nowItem->getItemType() == ItemType_Break){
		return false;
	}

	//���ӦΪ1
	if (abs(getItemBox()->getItemPos(lastItem).x - getItemBox()->getItemPos(nowItem).x) + abs(getItemBox()->getItemPos(lastItem).y - getItemBox()->getItemPos(nowItem).y) != 1){
		return false;
	}

	//������һ������Ϊclearitem
	if ((lastItem->getItemType() <= ClearItemType_BEGIN || lastItem->getItemType() >= ClearItemType_END)\
		&& (nowItem->getItemType() <= ClearItemType_BEGIN || nowItem->getItemType() >= ClearItemType_END)){
		return false;
	}

	//�˶�����ӦΪFixed
	if (lastItem->getActionState() != Fixed || nowItem->getActionState() != Fixed){
		return false;
	}

	//�������Ͳ�Ӧ��ͬ
	if (lastItem->getItemType() == nowItem->getItemType()){
		return false;
	}

	return true;
}

void Scanner_T::initItems(){
	Vector<Entity*> items;
	std::vector<Point> points;

	//��ȡϵͳʱ��,�����������
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //��ת��Ϊ΢�� 
	srand(rand_seed);

	//�������items
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (getItemBox()->getItem_lgc(i, j)->getItemType() == ClearItemType_END){
				Entity* item = getItemBox()->getItem_lgc(i, j);
				((ClearItem_T*)item)->bindSprite(ItemType(rand() % (ClearItemType_END - ClearItemType_BEGIN - 1) + ClearItemType_BEGIN + 1));
				items.pushBack(item);
				points.push_back(Point(i, j));
			}
		}
	}

	//��items�������
	relocateItems(items, points);
}

void Scanner_T::checkAndRelocate(){
	Vector<Entity*> items;
	std::vector<Point> points;

	//����Ǳ��������������
	if (isClearablePotentially()){
		return;
	}

	//�ռ�����
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			//��ClearItem������
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END){
				items.pushBack(item);
				points.push_back(Point(i, j));
			}
		}
	}

	//��������
	relocateItems(items, points);
}

ItemType Scanner_T::getLeastClearItemType(){
	//����һ�������洢�������͵�����������
	const int maxNum = 100;
	int allType[ClearItemType_END - ClearItemType_BEGIN - 1 + 1];//��һλ��Ϊ�������ֵ
	for (int i = 0; i < ClearItemType_END - ClearItemType_BEGIN - 1; i++){
		allType[i] = 0;
	}
	allType[ClearItemType_END - ClearItemType_BEGIN - 1] = maxNum;

	//��ȡ�������͵�����
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			ItemType type = ItemType(getItemBox()->getItem_lgc(i, j)->getItemType());
			if (type > ClearItemType_BEGIN && type < ClearItemType_END){
				allType[(int)type - (int)ClearItemType_BEGIN - 1]++;
			}
		}
	}

	//��ȡ���ٸ�����3������
	int leastIndex[3] = { ClearItemType_END - ClearItemType_BEGIN - 1, ClearItemType_END - ClearItemType_BEGIN - 1, ClearItemType_END - ClearItemType_BEGIN - 1 };
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < ClearItemType_END - ClearItemType_BEGIN - 1; j++){
			if (i == 0){
				if (allType[j] < allType[leastIndex[i]]){
					leastIndex[i] = j;
				}
			}
			else{
				if (allType[j] < allType[leastIndex[i]] && allType[j] >= allType[leastIndex[i - 1]] && j != leastIndex[i - 1]){
					leastIndex[i] = j;
				}
			}
		}
	}

	//��ȡϵͳʱ��,�����������
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_usec);    //΢�� 
	srand(rand_seed);

	//�����ȡ3����֮һ
	int r = (int)rand() % 3;

	//�±�ת�������ͷ���ֵ
	ItemType type = ItemType(leastIndex[r] + ClearItemType_BEGIN + 1);
	return type;
}

void Scanner_T::convertItemType(Entity* item, ItemType type){
	CCASSERT(item != NULL, "item null");
	Point pos_lgc = getItemBox()->getItemPos(item);
	Point pos_phy = item->getPosition();
	item->removeFromParent();

	//���ɶ�Ӧ���Ͷ���
	Entity* item_new = NULL;
	if (type == ItemType_NULL || type == ItemType_CLEAN || (type > ClearItemType_BEGIN && type < ClearItemType_END)){
		item_new = ClearItem_T::create();
		((ClearItem_T*)item_new)->bindSprite(type);
	}
	else if (type == ItemType_Break){
		item_new = BreakItem_T::create();
	}
	else if (type == ItemType_Drop){
		item_new = DropItem_T::create();
	}

	CCASSERT(item_new != NULL,"item_new null");

	//����ԭ��λ����Ϣ
	getItemBox()->addChild(item_new, ITEM);
	getItemBox()->setItem(pos_lgc.x, pos_lgc.y, item_new);
	item_new->setPosition(pos_phy);
}