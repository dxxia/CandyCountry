#include "ItemBox_T.h"
#include "json\reader.h"
#include "json\value.h"

ItemBox_T::ItemBox_T(int tollNum)
{
	Json::Reader read;
	Json::Value root;
	this->tollNum = tollNum;
	//设置表格属性
	int row = 8;
	int column = 8;
	std::string data = FileUtils::getInstance()->getStringFromFile("tollgate.json");
	std::string tollNum_s = StringUtils::format("%d", tollNum);
	if (read.parse(data, root, false)){
		row = root[tollNum_s]["size"]["row"].asInt();
		column = root[tollNum_s]["size"]["column"].asInt();
	}

	Point cellNum = Point(column, row);
	Point cellSize = Point(65.0, 65.0);
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

ItemBox_T::~ItemBox_T()
{
	//释放内存
	for (int i = 0; i < cellNum.x; i++){
		free(items[i]);
	}
	free(items);
}

ItemBox_T* ItemBox_T::create(int tollNum){
	ItemBox_T *pRet = new ItemBox_T(tollNum); 
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool ItemBox_T::init(){
	//设置容器大小
	this->setContentSize(Size(520, 520));

	//绑定控制器
	this->m_scanner = Scanner_T::create(this);
	this->m_touchController = TouchController_T::create(this);
	this->m_clearController = ClearController_T::create(this);
	this->m_dropController = DropController_T::create(this);
	this->addChild(m_scanner);
	this->addChild(m_touchController);
	this->addChild(m_clearController);
	this->addChild(m_dropController);

	//创建空物体对象
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			if (items[i][j] == NULL){
				items[i][j] = ClearItem_T::create();
				this->addChild(items[i][j], ITEM);
				items[i][j]->setPosition(Point(getCellSize().x * (0.5f + (float)i), getCellSize().y * (0.5f + (float)j)));
			}
		}
	}

	//加载布局配置
	loadMapConfig();

	//初始化物体数据
	((Scanner_T*)m_scanner)->initItems();

	//添加单元格背景
	drawCell();

	return true;
}

void ItemBox_T::loadMapConfig(){
	Json::Reader read;
	Json::Value root;

	std::map<std::string, int> s2e;
	s2e["ItemType_NULL"] = ItemType_NULL;
	s2e["ItemType_CLEAN"] = ItemType_CLEAN;
	s2e["ClearItemType_BEGIN"] = ClearItemType_BEGIN;
	s2e["ItemType_White"] = ItemType_White;
	s2e["ItemType_Blue"] = ItemType_Blue;
	s2e["ItemType_Brown"] = ItemType_Brown;
	s2e["ItemType_Green"] = ItemType_Green;
	s2e["ItemType_Purple"] = ItemType_Purple;
	s2e["ItemType_Yellow"] = ItemType_Yellow;
	//s2e["ItemType_Red"] = ItemType_Red;
	s2e["ItemType_Break"] = ItemType_Break;
	s2e["ItemType_Drop"] = ItemType_Drop;

	std::string data = FileUtils::getInstance()->getStringFromFile("tollgate.json");
	std::string tollNum_s = StringUtils::format("%d", tollNum);
	if (read.parse(data, root, false)){
		Json::Value v = root[tollNum_s]["map"];
		for (int i = 0; i < v.size(); i++){
			((Scanner_T*)getScanner())->convertItemType(getItem_lgc(v[i]["x"].asInt(), v[i]["y"].asInt()), (ItemType)s2e[v[i]["type"].asString()]);
		}
	}
}

void ItemBox_T::resetPos(Entity* item){
	if (item == NULL){
		return;
	}
	Point pos = getItemPos(item);
	item->setPosition((pos.x + 0.5f) * getCellSize().x, (pos.y + 0.5f) * getCellSize().y);
}

void ItemBox_T::drawCell(){
	for (int j = 0; j < getCellNum().y; j++){
		for (int i = 0; i < getCellNum().x; i++){
			//绘制单元格项的背景
			if (items[i][j]->getItemType() != ItemType_NULL){
				Sprite* sprite = Sprite::create("cellBG.png");
				this->addChild(sprite, CELLBG);
				sprite->setOpacity(180);
				sprite->setPosition(items[i][j]->getPosition());
			}
		}
	}
}