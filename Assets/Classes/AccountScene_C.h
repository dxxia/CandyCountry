#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;


class AccountScene_C : public Layer
{
public:
	AccountScene_C();
	~AccountScene_C();

	static Scene* createScene(int nowGoal);
	virtual bool init();
	CREATE_FUNC(AccountScene_C);

	void exitBtn(Ref*, TouchEventType type);

private:
	static int nowGoal;
	int bestGoal;
};


