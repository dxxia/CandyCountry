#include "ClearItem_C.h"
#include "SimpleAudioEngine.h"

ClearItem_C::ClearItem_C()
{
	this->m_stateType = StateType_normal;
	m_itemType = ClearItemType_END;
	m_actionState = Fixed;
}

ClearItem_C::~ClearItem_C()
{
}

bool ClearItem_C::init(){

	return true;
}

StateType ClearItem_C::getStateType(){
	return this->m_stateType;
}

void ClearItem_C::setStateType(StateType stateType){
	this->m_stateType = stateType;
	//onChangeStateType()...;
	if (m_stateType == StateType_super){
		std::string str = StringUtils::format("candy/%ds.png", (int)(this->getItemType() - ClearItemType_BEGIN));
		Entity::bindSprite(Sprite::create(str.c_str()));
		m_sprite->setScale(0.65f);
	}
}

void ClearItem_C::bindSprite(ItemType type){
	this->m_itemType = type;
	if (type == ItemType_CLEAN){
		if (this->m_sprite != NULL){
			m_sprite->removeFromParentAndCleanup(true);
			this->m_sprite = NULL;
		}
	}
	else{
		std::string str = StringUtils::format("candy/%d.png", (int)type - ClearItemType_BEGIN);
		Entity::bindSprite(Sprite::create(str.c_str()));
		m_sprite->setScale(0.65f);
	}
}

void ClearItem_C::onTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 0.7f, 0.7f);
	this->runAction(action);
	//²¥·Å´¥ÃþÉùÒô
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touchItem.mp3");
}

void ClearItem_C::offTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 1.0f, 1.0f);
	this->runAction(action);
}