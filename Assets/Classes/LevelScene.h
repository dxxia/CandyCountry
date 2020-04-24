#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

#define BGLAYER_TAG 0
#define LOGICLAYER_TAG 1
#define MENULAYER_TAG 2

class LevelScene : public Layer
{
public:
	LevelScene();
	~LevelScene();

	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LevelScene);
private:
};

