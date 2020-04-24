#include "DropController_T.h"
#include "ItemBox_T.h"
#include "PostDate.h"
#include "SimpleAudioEngine.h"


DropController_T::DropController_T()
{
	this->scheduleUpdate();

}

DropController_T::~DropController_T()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool DropController_T::init(ItemBox_T* itemBox){
	bindItemBox(itemBox);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DropController_T::dropListener), "drop", NULL);

	return true;
}

void DropController_T::dropListener(Ref* date){
	findDropList();

	//若消除后无掉落项也恢复触摸
	if (dropList.size() == 0){
		NotificationCenter::getInstance()->postNotification("moveOver", NULL);
	}
}

void DropController_T::findDropList(){

	//从左下到右上遍历，获取droplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (getItemBox()->getItem_lgc(i, j)->getItemType() == ItemType_CLEAN){

				//找到最上面的clean状态物体，判断上方物体
				int k = 0;
				Entity* item = NULL;
				while ((item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getItemType() == ItemType_CLEAN){
					k++;
				}

				Entity* item_clean = getItemBox()->getItem_lgc(i, j + k - 1);
				//1.上方越界,生成物体
				if (item == NULL){
					ItemType type = ((Scanner_T*)getItemBox()->getScanner())->getLeastClearItemType();
					((ClearItem_T*)item_clean)->bindSprite(type);
					item_clean->setPosition(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)getItemBox()->getCellNum().y + 0.5f) * getItemBox()->getCellSize().y);
					item_clean->setActionState(Droping);
					dropList.pushBack(item_clean);
					aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1) + 0.5f) * getItemBox()->getCellSize().y));
				}//2.上方为可消除物体类型或可掉落的目标物体
				else if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop){
					//若掉落状态则忽略
					if (item->getActionState() == Droping){
						continue;
					}//否则可掉落，与下方交换（防止掉落冲突，提前交换）
					else{
						getItemBox()->setItem(i, j + k - 1, item);
						getItemBox()->setItem(i, j + k, item_clean);
						item->setActionState(Droping);
						dropList.pushBack(item);
						aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1) + 0.5f) * getItemBox()->getCellSize().y));
					}
				}//3.若上方阻塞，向斜上两边搜索
				else if (item->getItemType() == ItemType_NULL || item->getItemType() == ItemType_Break){
					int q = 0;

					//若上方的clean无法掉落，则判断其下方的clean,直到有clean项可以下落或者到尾端
					while ((item_clean = getItemBox()->getItem_lgc(i, j + k - 1 - q)) != NULL && item_clean->getItemType() == ItemType_CLEAN){
						//两边都可掉落则随机。
						if ((item = getItemBox()->getItem_lgc(i - 1, j + k - q)) != NULL && (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop) && item->getActionState() == Fixed\
							&& (item = getItemBox()->getItem_lgc(i + 1, j + k - q)) != NULL && (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop) && item->getActionState() == Fixed){
							int r = 1 - ((int)(CCRANDOM_0_1() * 2)) * 2;
							item = getItemBox()->getItem_lgc(i + r, j + k - q);
							log("%d",r);
							if (getItemBox()->getItem_lgc(i + r, j + k - 1 - q) != NULL && getItemBox()->getItem_lgc(i + r, j + k - 1 - q)->getItemType() == ItemType_CLEAN){
								break;
							}
							getItemBox()->setItem(i, j + k - 1 - q, item);
							getItemBox()->setItem(i + r, j + k - q, item_clean);
							item->setActionState(Droping);
							dropList.pushBack(item);
							aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1 - q) + 0.5f) * getItemBox()->getCellSize().y));
							break;
						}
						else if ((item = getItemBox()->getItem_lgc(i - 1, j + k - q)) != NULL && (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop) && item->getActionState() == Fixed){ 
							//若其下方可掉则等待
							if (getItemBox()->getItem_lgc(i - 1, j + k - 1 - q) != NULL && getItemBox()->getItem_lgc(i - 1, j + k - 1 - q)->getItemType() == ItemType_CLEAN){
								break;
							}
							getItemBox()->setItem(i, j + k - 1 - q, item);
							getItemBox()->setItem(i - 1, j + k - q, item_clean);
							item->setActionState(Droping);
							dropList.pushBack(item);
							aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1 - q) + 0.5f) * getItemBox()->getCellSize().y));
							break;
						}
						else if ((item = getItemBox()->getItem_lgc(i + 1, j + k - q)) != NULL && (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop) && item->getActionState() == Fixed){
							//若其下方可掉则等待
							if (getItemBox()->getItem_lgc(i + 1, j + k - 1 - q) != NULL && getItemBox()->getItem_lgc(i + 1, j + k - 1 - q)->getItemType() == ItemType_CLEAN){
								break;
							}
							getItemBox()->setItem(i, j + k - 1 - q, item);
							getItemBox()->setItem(i + 1, j + k - q, item_clean);
							item->setActionState(Droping);
							dropList.pushBack(item);
							aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1 - q) + 0.5f) * getItemBox()->getCellSize().y));
							break;
						}

						q++;
					}

				}

			}
		}
	}
}

void DropController_T::update(float dt){
	if (dropList.size() > 0){
		for (int i = 0; i < dropList.size(); i++){
			Entity* item;
			Point pos;

			//已到达目标位置
			if ((item = dropList.at(i))->getPosition().y <= (pos = aimPos.at(i)).y){
				item->setActionState(Arrived);
				//修正x坐标移动过度（y坐标不修正，以免产生卡顿感）
				item->setPosition((getItemBox()->getItemPos(item).x + 0.5f) * getItemBox()->getCellSize().x, item->getPosition().y);
				//移除目标
				dropList.eraseObject(item);
				aimPos.erase(aimPos.begin() + i);
				i--;
			}//未到达目标位置，继续掉落动画，更改位置
			else{
				float x = item->getPosition().x;
				float y = item->getPosition().y;
				y -= dt * dropSpeed;
				if (x < pos.x){
					x += dt * dropSpeed;
				}
				else if(x > pos.x){
					x -= dt * dropSpeed;
				}
				item->setPosition(x, y);
			}

			//若全部掉落完毕
			if (dropList.size() == 0){
				//音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tick.wav");
				//更新Arrived - Fixed
				updateToFixed();
				//再次扫描以发现新的掉落项
				findDropList();
				//无可掉落项，进行步数结束逻辑判断
				if (dropList.size() == 0){
					onStepEnd();
				}
			}
		}

	}

}

void DropController_T::updateToFixed(){
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			
			Entity* item = getItemBox()->getItem_lgc(i, j);
			//若掉落目标物掉落至底部，则消失
			if (item->getItemType() == ItemType_Drop && getItemBox()->getItemPos(item).y == 0){
				((ClearController_T*)getItemBox()->getClearController())->clearEntity(item);
			}
			//搜索Arrived状态项
			if (item->getActionState() == Arrived){
				//若其下方物体为非Clean的固定态，则此项可固定
				Entity* belowItem = getItemBox()->getItem_lgc(i, j - 1);
				if (belowItem == NULL || (belowItem->getActionState() == Fixed && belowItem->getItemType() != ItemType_CLEAN)){
					item->setActionState(Fixed);
					((ItemBox_T*)getItemBox())->resetPos(item);
					//若可消除则发送消除消息
					if (((Scanner_T*)getItemBox()->getScanner())->isClearable(item)){
						Vector<Entity*> list;
						list.pushBack(item);
						PostDate* date = PostDate::create();
						date->setVector(list);
						date->retain();
						NotificationCenter::getInstance()->postNotification("clear", date);
					}
				}
			}
		}
	}
}

void DropController_T::onStepEnd(){
	//检查重排
	((Scanner_T*)getItemBox()->getScanner())->checkAndRelocate();
	
	//解除移动状态
	NotificationCenter::getInstance()->postNotification("moveOver", NULL);

	//发送游戏结束判断消息
	NotificationCenter::getInstance()->postNotification("stepEnd", NULL);
}