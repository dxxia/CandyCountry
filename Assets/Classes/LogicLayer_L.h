#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ItemBox;
class LogicLayer_L : public Layer
{
public:
	LogicLayer_L();
	~LogicLayer_L();

	bool init();
	CREATE_FUNC(LogicLayer_L);

	void gameOver();
private:
	//状态更新
	void update(float dt);

	//每步结束进行gameover判断
	void stepEnd(Ref*);

	ItemBox* m_itemBox;
};
