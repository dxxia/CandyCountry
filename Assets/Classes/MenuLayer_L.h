#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

class MenuLayer_L : public Layer
{
public:
	MenuLayer_L();
	~MenuLayer_L();

	bool init();
	CREATE_FUNC(MenuLayer_L);

	void exitBtn(Ref*, TouchEventType type);
	void exitSureBtn(Ref*, TouchEventType type);
	void exitCancelBtn(Ref*, TouchEventType type);
private:
};
