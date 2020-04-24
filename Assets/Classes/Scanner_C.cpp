#include "TouchController_C.h"
#include "ItemBox_C.h"

Scanner_C::Scanner_C()
{
}

Scanner_C::~Scanner_C()
{
}

bool Scanner_C::init(ItemBox_C* itemBox){
	this->bindItemBox(itemBox);
	return true;
}

ItemType Scanner_C::getSameTowClearItemType(int x, int y, Direction direction){
	ItemType type = ClearItemType_END;
	Entity* item = NULL;

	//判断不同方向
	switch (direction)
	{
	case up:
		break;
	case down:
		item = this->getItemBox()->getItem_lgc(x, y - 1);
		if (item == NULL || item->getActionState() == Droping){
			break;
		}
		type = ItemType(item->getItemType());
		item = this->getItemBox()->getItem_lgc(x, y - 2);
		if (item == NULL || item->getActionState() == Droping){
			break;
		}
		if (type == item->getItemType()){
			return type;
		}
		else{
			break;
		}
		break;
	case left:
		item = this->getItemBox()->getItem_lgc(x - 1, y);
		if (item == NULL || item->getActionState() == Droping){
			break;
		}
		type = ItemType(item->getItemType());
		item = this->getItemBox()->getItem_lgc(x - 2, y);
		if (item == NULL || item->getActionState() == Droping){
			break;
		}
		if (type == item->getItemType()){
			return type;
		}
		else{
			break;
		}
		break;
	case right:
		break;
	default:
		break;
	}

	return ClearItemType_END;
}

bool Scanner_C::isClearablePotentially(){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//遍历每个对象，并将其模拟移动寻找潜在可消除项
			Entity* anotherItem = NULL;
			Entity* item = getItemBox()->getItem_lgc(i, j);

			//模拟左移
			if ((anotherItem = getItemBox()->getItem_lgc(i - 1, j)) == NULL){
				log("NULL");
			}
			else if (((Scanner_C*)getItemBox()->getScanner())->isMoveable(item, anotherItem)){
				log("can move");
			}
			if ((anotherItem = getItemBox()->getItem_lgc(i - 1, j)) != NULL && ((Scanner_C*)getItemBox()->getScanner())->isMoveable(item, anotherItem)){
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

			//模拟下移
			if ((anotherItem = getItemBox()->getItem_lgc(i, j - 1)) != NULL && ((Scanner_C*)getItemBox()->getScanner())->isMoveable(item, anotherItem)){
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

void Scanner_C::getPotentialItem(Entity* &item1, Entity* &item2){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//遍历每个对象，并将其模拟移动寻找潜在可消除项
			Entity* anotherItem = NULL;
			Entity* item = getItemBox()->getItem_lgc(i, j);

			//模拟左移
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

			//模拟下移
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

void Scanner_C::relocateItems(Vector<Entity*> items){
	//获取系统时间,设置随机种子
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //都转化为微秒 
	srand(rand_seed);

	//若随机的情况不符合则重新随机
	bool isRepeat;
	do{
		Vector<Entity*> items_copy(items);
		isRepeat = false;
		//在无三消情况下随机物体
		for (int j = 0; j < getItemBox()->getCellNum().y && !isRepeat; j++){
			for (int i = 0; i < getItemBox()->getCellNum().x && !isRepeat; i++){

				CCASSERT(items_copy.size() != 0, "Scanner_C::relocateItems(Vector<Entity_C*> items)：重新分配的对象数目非法！");

				//若此次循环在此位置不存在不可消除的对象，则重新随机循环（防止接下来死循环）
				int k;
				for (k = 0; k < items_copy.size(); k++){
					if (items_copy.at(k)->getItemType() != getSameTowClearItemType(i, j, down) && items_copy.at(k)->getItemType() != getSameTowClearItemType(i, j, left)){
						break;
					}
				}
				if (k == items_copy.size()){
					isRepeat = true;
					log("1");
					continue;
				}

				//开始随机,得到不可消除项
				Entity* item = NULL;
				do{
					item = items_copy.at(rand() % items_copy.size());
				} while (item->getItemType() == getSameTowClearItemType(i, j, down) || item->getItemType() == getSameTowClearItemType(i, j, left));

				//与之前该逻辑坐标的物体交换逻辑坐标,重设物理坐标,并从copy中移除
				Entity* beforeItem = getItemBox()->getItem_lgc(i, j);
				Point pos = getItemBox()->getItemPos(item);
				getItemBox()->setItem(i, j, item);
				getItemBox()->setItem(pos.x, pos.y, beforeItem);
				item->setPosition(getItemBox()->getCellSize().x * ((float)i + 0.5), getItemBox()->getCellSize().y * ((float)j + 0.5));
				items_copy.eraseObject(item);
			}
		}
		//若无潜在消除项则重新随机
		if (!isClearablePotentially()){
			log("2");
			isRepeat = true;
		}
	} while (isRepeat);//重新随机
}

bool Scanner_C::isClearable(Entity* item){
	//不符条件，无法消除
	if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END \
		|| item->getActionState() != Fixed){
		return false;
	}

	//获取逻辑坐标
	ItemType type = ItemType(item->getItemType());
	int x = this->getItemBox()->getItemPos(item).x;
	int y = this->getItemBox()->getItemPos(item).y;

	//竖直方向判断
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

	//水平方向判断
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

bool Scanner_C::isMoveable(Entity* lastItem, Entity* nowItem){
	//间距应为1
	if (abs(getItemBox()->getItemPos(lastItem).x - getItemBox()->getItemPos(nowItem).x) + abs(getItemBox()->getItemPos(lastItem).y - getItemBox()->getItemPos(nowItem).y) != 1){
		return false;
	}

	//类型应为clearitem
	if (lastItem->getItemType() <= ClearItemType_BEGIN || lastItem->getItemType() >= ClearItemType_END){
		return false;
	}
	if (nowItem->getItemType() <= ClearItemType_BEGIN || nowItem->getItemType() >= ClearItemType_END){
		return false;
	}

	//运动类型应为Fixed
	if (lastItem->getActionState() != Fixed || nowItem->getActionState() != Fixed){
		return false;
	}

	//两者类型不应相同
	if (lastItem->getItemType() == nowItem->getItemType()){
		return false;
	}

	return true;
}


void Scanner_C::initItems(){
	Vector<Entity*> items;

	//获取系统时间,设置随机种子
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //都转化为微秒 
	srand(rand_seed);

	//随机生成items
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			Entity* item = getItemBox()->getItem_lgc(i, j);
			((ClearItem_C*)item)->bindSprite(ItemType(rand() % (ClearItemType_END - ClearItemType_BEGIN - 1) + ClearItemType_BEGIN + 1));
			items.pushBack(item);
		}
	}

	//将items随机排列
	relocateItems(items);
	items.clear();
}

void Scanner_C::checkAndRelocate(){
	Vector<Entity*> items;

	//若有潜在消除项则跳过
	if (isClearablePotentially()){
		return;
	}

	//收集数据
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			items.pushBack(getItemBox()->getItem_lgc(i, j));
		}
	}

	//重新排列
	relocateItems(items);
}

ItemType Scanner_C::getLeastClearItemType(){
	//设置一个用来存储各个类型的数量的数组
	const int maxNum = 100;
	int allType[ClearItemType_END - ClearItemType_BEGIN - 1 + 1];//多一位作为理论最大值
	for (int i = 0; i < ClearItemType_END - ClearItemType_BEGIN - 1; i++){
		allType[i] = 0;
	}
	allType[ClearItemType_END - ClearItemType_BEGIN - 1] = maxNum;

	//获取各个类型的数量
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			ItemType type = ItemType(getItemBox()->getItem_lgc(i, j)->getItemType());
			if (type > ClearItemType_BEGIN && type < ClearItemType_END){
				allType[(int)type - (int)ClearItemType_BEGIN - 1]++;
			}
		}
	}

	//获取最少个数的3个类型
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

	//获取系统时间,设置随机种子
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_usec);    //微秒 
	srand(rand_seed);

	//随机获取3类型之一
	int r = (int)rand() % 3;

	//下标转换成类型返回值
	ItemType type = ItemType(leastIndex[r] + ClearItemType_BEGIN + 1);
	return type;
}