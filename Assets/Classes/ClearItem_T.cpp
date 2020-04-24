#include "ClearItem_T.h"
#include "SimpleAudioEngine.h"

ClearItem_T::ClearItem_T()
{
	this->m_stateType = StateType_normal;
	m_itemType = ClearItemType_END;
	m_actionState = Fixed;
}

ClearItem_T::~ClearItem_T()
{
}

bool ClearItem_T::init(){
	return true;
}

StateType ClearItem_T::getStateType(){
	return this->m_stateType;
}

void ClearItem_T::setStateType(StateType stateType){
	this->m_stateType = stateType;
	//onChangeStateType()...;
	if (m_stateType == StateType_super){
		std::string str = StringUtils::format("candy/%ds.png", (int)(this->getItemType() - ClearItemType_BEGIN));
		bindSprite(Sprite::create(str.c_str()));

	}
}

void ClearItem_T::bindSprite(Sprite* sprite){
	Entity::bindSprite(sprite);
	m_sprite->setScale(0.6f);
}

void ClearItem_T::bindSprite(ItemType type){
	this->m_itemType = type;
	if (type == ItemType_CLEAN || type == ItemType_NULL){//相对于经典模式，多出NULL状态
		if (this->m_sprite != NULL){
			m_sprite->removeFromParentAndCleanup(true);
			this->m_sprite = NULL;
		}
	}
	else{
		std::string str = StringUtils::format("candy/%d.png", (int)type - ClearItemType_BEGIN);
		bindSprite(Sprite::create(str.c_str()));
	}
}

void ClearItem_T::onTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 0.7f, 0.7f);
	this->runAction(action);
	//播放触摸声音
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touchItem.mp3");
}

void ClearItem_T::offTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 1.0f, 1.0f);
	this->runAction(action);
}