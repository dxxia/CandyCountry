#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;


class ChooseScene : public Layer
{
public:
	ChooseScene();
	~ChooseScene();

	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ChooseScene);

	void choose(Node* target, TouchEventType type);
	void exitBtn(Ref*, TouchEventType type);

};

