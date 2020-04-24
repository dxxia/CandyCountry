#include "ItemBoxController.h"
#include "ItemBox.h"

ItemBoxController::ItemBoxController()
{
	this->m_itemBox = NULL;
}

ItemBoxController::~ItemBoxController()
{
}

ItemBoxController* ItemBoxController::create(ItemBox* itemBox){
	auto *pRet = new ItemBoxController();
	if (pRet && pRet->init(itemBox))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ItemBoxController::init(ItemBox* itemBox){
	this->m_itemBox = itemBox;

	return true;
}

ItemBox* ItemBoxController::getItemBox(){
	return m_itemBox;
}