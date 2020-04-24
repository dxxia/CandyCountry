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
	//״̬����
	void update(float dt);

	//ÿ����������gameover�ж�
	void stepEnd(Ref*);

	ItemBox* m_itemBox;
};
