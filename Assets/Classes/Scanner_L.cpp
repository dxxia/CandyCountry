#include "Scanner_L.h"
#include "ItemBox_L.h"

Scanner_L::Scanner_L()
{
}

Scanner_L::~Scanner_L()
{
}

bool Scanner_L::init(ItemBox_L* itemBox){
	this->bindItemBox(itemBox);
	return true;
}

ItemType Scanner_L::getSameTowClearItemType(int x, int y, Direction direction){
	ItemType type = ClearItemType_END;
	Entity* item = NULL;

	//判断不同方向
	switch (direction)
	{
	case up:
		break;
	case down:
		item = this->getItemBox()->getItem_lgc(x, y - 1);
		if (item == NULL || item->getItemType() <= ClearItemType_BEGIN || item->getItemType() >= ClearItemType_END || item->getActionState() == Droping){
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

bool Scanner_L::isMoveable(Entity* lastItem, Entity* nowItem){
	CCASSERT(lastItem != NULL || nowItem != NULL, "TouchController_L::isMoveable(Entity_T* lastItem, Entity_T* nowItem)：判断目标不能全为空！");

	//不为NULL
	if (lastItem == NULL || nowItem == NULL){
		return false;
	}

	//类型不为ItemType_Clean
	if (lastItem->getItemType() == ItemType_CLEAN || nowItem->getItemType() == ItemType_CLEAN){
		return false;
	}

	//间距应为1
	if (abs(getItemBox()->getItemPos(lastItem).x - getItemBox()->getItemPos(nowItem).x) + abs(getItemBox()->getItemPos(lastItem).y - getItemBox()->getItemPos(nowItem).y) != 1){
		return false;
	}

	//至少有一个类型为clearitem
	if ((lastItem->getItemType() <= ClearItemType_BEGIN || lastItem->getItemType() >= ClearItemType_END)\
		&& (nowItem->getItemType() <= ClearItemType_BEGIN || nowItem->getItemType() >= ClearItemType_END)){
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

bool Scanner_L::isClearablePotentially(){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){

			//遍历每个对象，并将其模拟移动寻找潜在可消除项
			Entity* anotherItem = NULL;
			Entity* item = getItemBox()->getItem_lgc(i, j);

			//模拟左移
			if ((anotherItem = getItemBox()->getItem_lgc(i - 1, j)) != NULL && ((Scanner_L*)getItemBox()->getScanner())->isMoveable(item, anotherItem)){
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
			if ((anotherItem = getItemBox()->getItem_lgc(i, j - 1)) != NULL && ((Scanner_L*)getItemBox()->getScanner())->isMoveable(item, anotherItem)){
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

void Scanner_L::relocateItems(Vector<Entity*> items, std::vector<Point> points){
	//获取系统时间,设置随机种子
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //都转化为微秒 
	srand(rand_seed);

	CCASSERT(items.size() == points.size(), "Scanner_L::relocateItems(Vector<Entity_T*> items)：重新分配的对象数目非法！");
	//若随机的情况不符合则重新随机
	bool isRepeat;
	do{
		Vector<Entity*> items_copy(items);
		isRepeat = false;
		//在无三消情况下随机物体
		for (const auto& point : points){

			//若此次循环在此位置不存在不可消除的排列，则重新随机循环（防止接下来死循环）
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

			//开始随机,得到不可消除项
			Entity* item = NULL;
			do{
				item = items_copy.at(rand() % items_copy.size());
			} while (item->getItemType() == getSameTowClearItemType(point.x, point.y, down) || item->getItemType() == getSameTowClearItemType(point.x, point.y, left));

			//与之前该逻辑坐标的物体交换逻辑坐标,重设物理坐标,并从copy中移除
			Entity* beforeItem = getItemBox()->getItem_lgc(point.x, point.y);
			Point pos = getItemBox()->getItemPos(item);
			getItemBox()->setItem(point.x, point.y, item);
			getItemBox()->setItem(pos.x, pos.y, beforeItem);
			item->setPosition(getItemBox()->getCellSize().x * (point.x + 0.5), getItemBox()->getCellSize().y * (point.y + 0.5));
			items_copy.eraseObject(item);
		}
		//若无潜在消除项则重新随机
		if (!isRepeat && !isClearablePotentially()){
			isRepeat = true;
		}
	} while (isRepeat);//重新随机
}

bool Scanner_L::isClearable(Entity* item){
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

void Scanner_L::initItems(){
	Vector<Entity*> items;
	std::vector<Point> points;

	//获取系统时间,设置随机种子
	struct timeval now;
	gettimeofday(&now, NULL);
	unsigned rand_seed = (unsigned)(now.tv_sec * 1000000 + now.tv_usec);    //都转化为微秒 
	srand(rand_seed);

	//随机生成items
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (getItemBox()->getItem_lgc(i, j)->getItemType() == ClearItemType_END){
				Entity* item = getItemBox()->getItem_lgc(i, j);
				((ClearItem_L*)item)->bindSprite(ItemType(rand() % (ClearItemType_END - ClearItemType_BEGIN - 1) + ClearItemType_BEGIN + 1));
				items.pushBack(item);
				points.push_back(Point(i, j));
			}
		}
	}

	//将items随机排列
	relocateItems(items, points);
}

void Scanner_L::checkAndRelocate(){
	Vector<Entity*> items;
	std::vector<Point> points;

	//若有潜在消除项则跳过
	if (isClearablePotentially()){
		return;
	}

	//收集数据
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			//仅ClearItem可重排
			Entity* item = getItemBox()->getItem_lgc(i, j);
			if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END){
				items.pushBack(item);
				points.push_back(Point(i, j));
			}
		}
	}

	//重新排列
	relocateItems(items, points);
}

ItemType Scanner_L::getLeastClearItemType(){
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

