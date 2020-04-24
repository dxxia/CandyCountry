#include "BreakItem_T.h"

BreakItem_T::BreakItem_T(){
	m_itemType = ItemType_Break;
	m_actionState = Fixed;
}

BreakItem_T::~BreakItem_T(){

}

bool BreakItem_T::init(){
	bindSprite(Sprite::create("candy/break.png"));
	m_sprite->setScale(0.8f);
	return true;
}

void BreakItem_T::onTouched(){

}

void BreakItem_T::offTouched(){

}