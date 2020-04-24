#include "ClassScene.h"
#include "MainScene.h"
#include "LogicLayer.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"


ClassScene::ClassScene(){
	//¿ªÆôÒôÀÖ
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/classBG.mp3", true);
}

ClassScene::~ClassScene(){

}

Scene* ClassScene::createScene(){
	//Ìí¼Ó±³¾°Í¼²ã
	auto classScene = Scene::create();
	auto bgLayer = ClassScene::create();
	classScene->addChild(bgLayer, BGLAYER_TAG, BGLAYER_TAG);

	//Ìí¼ÓÂß¼­Í¼²ã
	auto logicLayer = LogicLayer::create();
	classScene->addChild(logicLayer, LOGICLAYER_TAG, LOGICLAYER_TAG);

	//Ìí¼Ó²Ëµ¥Í¼²ã
	auto menuLayer = MenuLayer::create();
	classScene->addChild(menuLayer, MENULAYER_TAG, MENULAYER_TAG);

	return classScene;
}

bool ClassScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//¼ÓÔØ±³¾°
	Sprite* classBG = Sprite::create("bg/classBG.png");
	this->addChild(classBG, 0);
	classBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//±êÌâ
	Sprite* classTitle = Sprite::create("classTitle.png");
	this->addChild(classTitle, 0);
	classTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 9 / 10));

	return true;
}
