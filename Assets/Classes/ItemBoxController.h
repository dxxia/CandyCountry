#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ItemBox;
class ItemBoxController : public Node
{
public:
	ItemBoxController();
	~ItemBoxController();

	virtual bool init(ItemBox* itemBox);
	static ItemBoxController* create(ItemBox* itemBox);

	ItemBox* getItemBox();
private:
	ItemBox* m_itemBox;
};

