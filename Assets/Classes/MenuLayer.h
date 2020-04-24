#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

class MenuLayer : public Layer
{
public:
	MenuLayer();
	~MenuLayer();

	bool init();
	CREATE_FUNC(MenuLayer);

	void pauseBtn(Ref*, TouchEventType type);
	void exitBtn(Ref*, TouchEventType type);

	void exitSureBtn(Ref*, TouchEventType type);
	void exitCancelBtn(Ref*, TouchEventType type);
	void startBtn(Ref*, TouchEventType type);
private:
};
