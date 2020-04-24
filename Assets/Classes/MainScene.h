#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

class MainScene : public Layer
{
public:
	MainScene();
	~MainScene();

	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);

	void classBtn(Ref*, TouchEventType type);
	void tollBtn(Ref*, TouchEventType type);
	void levelBtn(Ref*, TouchEventType type);
	void voiceBtn(Node* target, TouchEventType type);
	void exitGameBtn(Ref*, TouchEventType type);
	void exitSureBtn(Ref*, TouchEventType type);
	void exitCancelBtn(Ref*, TouchEventType type);
private:
	bool  voiceEnable;
};
