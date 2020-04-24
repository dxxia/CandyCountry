#include "ItemBox_L.h"
#include "json\reader.h"
#include "json\value.h"

ItemBox_L::ItemBox_L()
{
	//���ñ������
	int row = 9;
	int column = 8;
	Point cellNum = Point(column, row);
	Point cellSize = Point(65.0, 65.0);
	this->cellNum = cellNum;
	this->cellSize = cellSize;
	//�����ڴ�
	this->items = (Entity***)malloc(sizeof(Entity**)* cellNum.x);
	for (int i = 0; i < cellNum.x; i++){
		this->items[i] = (Entity**)malloc(sizeof(Entity*)* cellNum.y);
	}
	//��ʼ��
	for (int j = 0; j < cellNum.y; j++){
		for (int i = 0; i < cellNum.x; i++){
			this->items[i][j] = NULL;
		}
	}
}

ItemBox_L::~ItemBox_L()
{
	//�ͷ��ڴ�
	for (int i = 0; i < cellNum.x; i++){
		free(items[i]);
	}
	free(items);
}

bool ItemBox_L::init(){
	////���ر���ͼƬ
	//Sprite* sprite = Sprite::create("box_t.png");
	//this->addChild(sprite);
	//Size size = sprite->getContentSize();
	//sprite->setPosition(Point(size.width * 0.5f, size.height * 0.5f));
	//����������С
	this->setContentSize(Size(520, 520));

	//�󶨿�����
	this->m_scanner = Scanner_L::create(this);
	this->m_touchController = TouchController_L::create(this);
	this->m_clearController = ClearController_L::create(this);
	this->m_dropController = DropController_L::create(this);
	this->addChild(m_scanner);
	this->addChild(m_touchController);
	this->addChild(m_clearController);
	this->addChild(m_dropController);

	//�������������
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			if (items[i][j] == NULL){
				items[i][j] = ClearItem_L::create();
				this->addChild(items[i][j], ITEM);
				items[i][j]->setPosition(Point(getCellSize().x * (0.5f + (float)i), getCellSize().y * (0.5f + (float)j)));
			}
		}
	}
	//��ʼ����������
	((Scanner_L*)m_scanner)->initItems();

	//��ӵ�Ԫ�񱳾�
	drawCell();

	return true;
}

void ItemBox_L::resetPos(Entity* item){
	if (item == NULL){
		return;
	}
	Point pos = getItemPos(item);
	item->setPosition((pos.x + 0.5f) * getCellSize().x, (pos.y + 0.5f) * getCellSize().y);
}

void ItemBox_L::drawCell(){
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			//���Ƶ�Ԫ����ı���
			Sprite* sprite = Sprite::create("cellBG.png");
			this->addChild(sprite, CELLBG);
			sprite->setPosition(items[i][j]->getPosition());
			sprite->setOpacity(180);
		}
	}
}