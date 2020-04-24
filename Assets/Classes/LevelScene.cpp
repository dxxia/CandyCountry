#include "LevelScene.h"
#include "LogicLayer_L.h"
#include "MenuLayer_L.h"
#include "SimpleAudioEngine.h"

LevelScene::LevelScene(){
	//²¥·ÅÒôÀÖ
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/levelBG.mp3", true);
}

LevelScene::~LevelScene(){
}

Scene* LevelScene::createScene(){
	//Ìí¼Ó±³¾°Í¼²ã
	auto classScene = Scene::create();
	auto bgLayer = LevelScene::create();
	classScene->addChild(bgLayer, BGLAYER_TAG, BGLAYER_TAG);

	//Ìí¼ÓÂß¼­Í¼²ã
	auto logicLayer = LogicLayer_L::create();
	classScene->addChild(logicLayer, LOGICLAYER_TAG, LOGICLAYER_TAG);

	//Ìí¼Ó²Ëµ¥Í¼²ã
	auto menuLayer = MenuLayer_L::create();
	classScene->addChild(menuLayer, MENULAYER_TAG, MENULAYER_TAG);

	return classScene;
}

bool LevelScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//¼ÓÔØ±³¾°
	Sprite* levelBG = Sprite::create("bg/levelBG.png");
	this->addChild(levelBG, 0);
	levelBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//±êÌâ
	Sprite* levelTitle = Sprite::create("levelTitle.png");
	this->addChild(levelTitle, 0);
	levelTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 9 / 10));

	return true;
}
