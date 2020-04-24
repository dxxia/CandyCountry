#include "ItemBox.h"

ItemBox::ItemBox()
{
	this->setAnchorPoint(Point(0.5, 0.5));
	m_scanner = NULL;
	m_touchController = NULL;
	m_clearController = NULL;
	m_dropController = NULL;
}

ItemBox::~ItemBox()
{
}

Entity* ItemBox::getItem_lgc(int i, int j){
	if (i < 0 || i >= cellNum.x || j < 0 || j >= cellNum.y){
		return NULL;
	}

	return items[i][j];
}

Point ItemBox::getItemPos(Entity* item){
	for (int j = 0; j < cellNum.y; j++){
		for (int i = 0; i < cellNum.x; i++){
			if (items[i][j] == item){
				return Point(i, j);
			}
		}
	}
	CCASSERT(false, "ItemBox::getItemPos(Entity* item):�����ڸ����壡");
	return Point(-1, -1);
}

void ItemBox::setItem(int x, int y, Entity* item){
	CCASSERT((x >= 0 && x < cellNum.x && y >= 0 && y < cellNum.y), \
		"ItemBox::setItem(int x, int y, Entity* item):Ҫ���õ���������Ƿ�");
	CCASSERT(item != NULL, "����Ϊ��");
	this->items[x][y] = item;
}

Point ItemBox::getCellNum(){
	return cellNum;
}

Point ItemBox::getCellSize(){
	return cellSize;
}

Controller* ItemBox::getScanner(){
	return this->m_scanner;
}

Controller* ItemBox::getTouchController(){
	return this->m_touchController;
}

Controller* ItemBox::getClearController(){
	return this->m_clearController;
}

Controller* ItemBox::getDropController(){
	return this->m_dropController;
}

