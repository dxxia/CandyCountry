#include "Entity.h"

Entity::Entity()
{
	m_sprite = NULL;
	this->setAnchorPoint(Point(0.5f, 0.5f));
}

Entity::~Entity()
{
}

void Entity::bindSprite(Sprite* sprite){
	CCASSERT(sprite != NULL,"sprite null");
	if (this->m_sprite != NULL){
		m_sprite->removeFromParentAndCleanup(true);
	}
	
	this->m_sprite = sprite;
	this->addChild(m_sprite);

	Size size = m_sprite->getContentSize();
	m_sprite->setPosition(Point(size.width * 0.5f, size.height * 0.5f));
	this->setContentSize(size);

}

int Entity::getItemType(){
	return this->m_itemType;
}

void Entity::setActionState(int actionState){
	this->m_actionState = actionState;
}

int Entity::getActionState(){
	return m_actionState;
}
