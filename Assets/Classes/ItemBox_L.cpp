#include "ItemBox_L.h"
#include "json\reader.h"
#include "json\value.h"

ItemBox_L::ItemBox_L()
{
	//设置表格属性
	int row = 9;
	int column = 8;
	Point cellNum = Point(column, row);
	Point cellSize = Point(65.0, 65.0);
	this->cellNum = cellNum;
	this->cellSize = cellSize;
	//申请内存
	this->items = (Entity***)malloc(sizeof(Entity**)* cellNum.x);
	for (int i = 0; i < cellNum.x; i++){
		this->items[i] = (Entity**)malloc(sizeof(Entity*)* cellNum.y);
	}
	//初始化
	for (int j = 0; j < cellNum.y; j++){
		for (int i = 0; i < cellNum.x; i++){
			this->items[i][j] = NULL;
		}
	}
}

ItemBox_L::~ItemBox_L()
{
	//释放内存
	for (int i = 0; i < cellNum.x; i++){
		free(items[i]);
	}
	free(items);
}

bool ItemBox_L::init(){
	////加载背景图片
	//Sprite* sprite = Sprite::create("box_t.png");
	//this->addChild(sprite);
	//Size size = sprite->getContentSize();
	//sprite->setPosition(Point(size.width * 0.5f, size.height * 0.5f));
	//设置容器大小
	this->setContentSize(Size(520, 520));

	//绑定控制器
	this->m_scanner = Scanner_L::create(this);
	this->m_touchController = TouchController_L::create(this);
	this->m_clearController = ClearController_L::create(this);
	this->m_dropController = DropController_L::create(this);
	this->addChild(m_scanner);
	this->addChild(m_touchController);
	this->addChild(m_clearController);
	this->addChild(m_dropController);

	//创建空物体对象
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			if (items[i][j] == NULL){
				items[i][j] = ClearItem_L::create();
				this->addChild(items[i][j], ITEM);
				items[i][j]->setPosition(Point(getCellSize().x * (0.5f + (float)i), getCellSize().y * (0.5f + (float)j)));
			}
		}
	}
	//初始化物体数据
	((Scanner_L*)m_scanner)->initItems();

	//添加单元格背景
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
			//绘制单元格项的背景
			Sprite* sprite = Sprite::create("cellBG.png");
			this->addChild(sprite, CELLBG);
			sprite->setPosition(items[i][j]->getPosition());
			sprite->setOpacity(180);
		}
	}
}