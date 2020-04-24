#include "ClearItem_L.h"
#include "SimpleAudioEngine.h"

ClearItem_L::ClearItem_L()
{
	m_itemType = ClearItemType_END;
	m_actionState = Fixed;
	m_itemLevel = 0;
	levelLabel = NULL;
}

ClearItem_L::~ClearItem_L()
{
}

bool ClearItem_L::init(){
	levelLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	levelLabel->setString("");
	this->addChild(levelLabel, 5);
	return true;
}

void ClearItem_L::setItemLevel(int level){
	this->m_itemLevel = level;
	if (level == 0){
		levelLabel->setString("");
	}
	else{
		//if (level > 4){
		//	m_itemLevel = 4;
		//}
		levelLabel->setString(StringUtils::toString(m_itemLevel));
	}
}

int ClearItem_L::getItemLevel(){
	return m_itemLevel;
}

void ClearItem_L::bindSprite(Sprite* sprite){
	Entity::bindSprite(sprite);
	m_sprite->setScale(0.6f);
	levelLabel->setPosition(m_sprite->getContentSize().width * 0.5, m_sprite->getContentSize().height * 0.5);
}

void ClearItem_L::bindSprite(ItemType type){
	this->m_itemType = type;
	if (type == ItemType_CLEAN){
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

void ClearItem_L::onTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 0.7f, 0.7f);
	this->runAction(action);
	//²¥·Å´¥ÃþÉùÒô
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touchItem.mp3");
}

void ClearItem_L::offTouched(){
	ScaleTo* action = ScaleTo::create(0.1f, 1.0f, 1.0f);
	this->runAction(action);
}