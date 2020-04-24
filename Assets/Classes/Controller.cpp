#include "Controller.h"


Controller::Controller()
{
	m_itemBox = NULL;
}

Controller::~Controller()
{
}


ItemBox* Controller::getItemBox(){
	return m_itemBox;
}

void Controller::bindItemBox(ItemBox* itemBox){
	this->m_itemBox = itemBox;
}
