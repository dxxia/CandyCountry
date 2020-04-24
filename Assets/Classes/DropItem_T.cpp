#include "DropItem_T.h"
#include "SimpleAudioEngine.h"

DropItem_T::DropItem_T(){
	m_itemType = ItemType_Drop;
	m_actionState = Fixed;
}

DropItem_T::~DropItem_T(){

}

bool DropItem_T::init(){
	bindSprite(Sprite::create("candy/drop.png"));
	m_sprite->setScale(0.6f);
	return true;
}

void DropItem_T::onTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 0.7f, 0.7f);
	this->runAction(action);
	//²¥·Å´¥ÃþÉùÒô
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touchItem.mp3");
}

void DropItem_T::offTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 1.0f, 1.0f);
	this->runAction(action);
}