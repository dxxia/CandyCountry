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

	//���������޵�����Ҳ�ָ�����
	if (dropList.size() == 0){
		NotificationCenter::getInstance()->postNotification("moveOver", NULL);
	}
}

void DropController_T::findDropList(){

	//�����µ����ϱ�������ȡdroplist
	for (int j = 0; j < getItemBox()->getCellNum().y; j++){
		for (int i = 0; i < getItemBox()->getCellNum().x; i++){
			if (getItemBox()->getItem_lgc(i, j)->getItemType() == ItemType_CLEAN){

				//�ҵ��������clean״̬���壬�ж��Ϸ�����
				int k = 0;
				Entity* item = NULL;
				while ((item = getItemBox()->getItem_lgc(i, j + k)) != NULL && item->getItemType() == ItemType_CLEAN){
					k++;
				}

				Entity* item_clean = getItemBox()->getItem_lgc(i, j + k - 1);
				//1.�Ϸ�Խ��,��������
				if (item == NULL){
					ItemType type = ((Scanner_T*)getItemBox()->getScanner())->getLeastClearItemType();
					((ClearItem_T*)item_clean)->bindSprite(type);
					item_clean->setPosition(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)getItemBox()->getCellNum().y + 0.5f) * getItemBox()->getCellSize().y);
					item_clean->setActionState(Droping);
					dropList.pushBack(item_clean);
					aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1) + 0.5f) * getItemBox()->getCellSize().y));
				}//2.�Ϸ�Ϊ�������������ͻ�ɵ����Ŀ������
				else if (item->getItemType() > ClearItemType_BEGIN && item->getItemType() < ClearItemType_END || item->getItemType() == ItemType_Drop){
					//������״̬�����
					if (item->getActionState() == Droping){
						continue;
					}//����ɵ��䣬���·���������ֹ�����ͻ����ǰ������
					else{
						getItemBox()->setItem(i, j + k - 1, item);
						getItemBox()->setItem(i, j + k, item_clean);
						item->setActionState(Droping);
						dropList.pushBack(item);
						aimPos.push_back(Point(((float)i + 0.5f) * getItemBox()->getCellSize().x, ((float)(j + k - 1) + 0.5f) * getItemBox()->getCellSize().y));
					}
				}//3.���Ϸ���������б����������
				else if (item->getItemType() == ItemType_NULL || item->getItemType() == ItemType_Break){
					int q = 0;

					//���Ϸ���clean�޷����䣬���ж����·���clean,ֱ����clean�����������ߵ�β��
					while ((item_clean = getItemBox()->getItem_lgc(i, j + k - 1 - q)) != NULL && item_clean->getItemType() == ItemType_CLEAN){
						//���߶��ɵ����������
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
							//�����·��ɵ���ȴ�
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
							//�����·��ɵ���ȴ�
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

			//�ѵ���Ŀ��λ��
			if ((item = dropList.at(i))->getPosition().y <= (pos = aimPos.at(i)).y){
				item->setActionState(Arrived);
				//����x�����ƶ����ȣ�y���겻����������������ٸУ�
				item->setPosition((getItemBox()->getItemPos(item).x + 0.5f) * getItemBox()->getCellSize().x, item->getPosition().y);
				//�Ƴ�Ŀ��
				dropList.eraseObject(item);
				aimPos.erase(aimPos.begin() + i);
				i--;
			}//δ����Ŀ��λ�ã��������䶯��������λ��
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

			//��ȫ���������
			if (dropList.size() == 0){
				//��Ч
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tick.wav");
				//����Arrived - Fixed
				updateToFixed();
				//�ٴ�ɨ���Է����µĵ�����
				findDropList();
				//�޿ɵ�������в��������߼��ж�
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
			//������Ŀ����������ײ�������ʧ
			if (item->getItemType() == ItemType_Drop && getItemBox()->getItemPos(item).y == 0){
				((ClearController_T*)getItemBox()->getClearController())->clearEntity(item);
			}
			//����Arrived״̬��
			if (item->getActionState() == Arrived){
				//�����·�����Ϊ��Clean�Ĺ̶�̬�������ɹ̶�
				Entity* belowItem = getItemBox()->getItem_lgc(i, j - 1);
				if (belowItem == NULL || (belowItem->getActionState() == Fixed && belowItem->getItemType() != ItemType_CLEAN)){
					item->setActionState(Fixed);
					((ItemBox_T*)getItemBox())->resetPos(item);
					//������������������Ϣ
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
	//�������
	((Scanner_T*)getItemBox()->getScanner())->checkAndRelocate();
	
	//����ƶ�״̬
	NotificationCenter::getInstance()->postNotification("moveOver", NULL);

	//������Ϸ�����ж���Ϣ
	NotificationCenter::getInstance()->postNotification("stepEnd", NULL);
}