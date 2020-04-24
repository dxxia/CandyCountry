#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;


class AccountScene_L : public Layer
{
public:
	AccountScene_L();
	~AccountScene_L();

	static Scene* createScene(int goal);
	virtual bool init();
	CREATE_FUNC(AccountScene_L);

	void exitBtn(Ref*, TouchEventType type);
private:
	static int nowGoal;
	int bestGoal;
};


