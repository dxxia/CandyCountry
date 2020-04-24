#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

class LogoScene : public Layer
{
public:
	LogoScene();
	~LogoScene();

	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LogoScene);

private:
};