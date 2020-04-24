#include "ItemBox_C.h"


ItemBox_C::ItemBox_C()
{
	//设置表格属性
	Point cellNum = Point(7, 7);
	Point cellSize = Point(70.0, 70.0);
	this->cellNum = cellNum;
	this->cellSize = cellSize;
	//申请内存
	this->items = (Entity***)malloc(sizeof(Entity**) * cellNum.x);
	for (int i = 0; i < cellNum.x; i++){
		this->items[i] = (Entity**)malloc(sizeof(Entity*) * cellNum.y);
	}
	//初始化
	for (int j = 0; j < cellNum.y; j++){
		for (int i = 0; i < cellNum.x; i++){
			this->items[i][j] = NULL;
		}
	}
}

ItemBox_C::~ItemBox_C()
{
	//释放内存
	for (int i = 0; i < cellNum.x; i++){
			free(items[i]);
	}
	free(items);
}

bool ItemBox_C::init(){
	//加载背景图片
	Sprite* sprite = Sprite::create("box.png");
	this->addChild(sprite);
	Size size = sprite->getContentSize();
	sprite->setPosition(Point(size.width * 0.5f, size.height * 0.5f));
	this->setContentSize(size);
	sprite->setOpacity(180);

	//绑定控制器
	m_scanner = Scanner_C::create(this);
	this->addChild(m_scanner);
	m_touchController = TouchController_C::create(this);
	this->addChild(m_touchController);
	m_clearController = ClearController_C::create(this);
	this->addChild(m_clearController);
	m_dropController = DropController_C::create(this);
	this->addChild(m_dropController);

	//创建空物体对象
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			items[i][j] = ClearItem_C::create();
			this->addChild(items[i][j]);
			getItem_lgc(i, j)->setPosition(Point(getCellSize().x * (0.5f + (float)i), getCellSize().y * (0.5f + (float)j)));
		}
	}

	//初始化物体数据
	((Scanner_C*)getScanner())->initItems();

	return true;
}


Entity* ItemBox_C::getItem_lgc2phy(int ii, int jj){
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			Point pos = getItem_lgc(i, j)->getPosition();
			if (pos.x / getCellSize().x == ii && pos.y / getCellSize().y == jj){
				return getItem_lgc(i, j);
			}
		}
	}
	return NULL;
}

