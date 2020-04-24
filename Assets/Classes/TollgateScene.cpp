#include "TollgateScene.h"
#include "LogicLayer_T.h"
#include "MenuLayer_T.h"
#include "SimpleAudioEngine.h"


TollgateScene::TollgateScene(){
	//²¥·ÅÒôÀÖ
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/tollBG.mp3", true);
}

TollgateScene::~TollgateScene(){
}

Scene* TollgateScene::createScene(int tollNum){
	//Ìí¼Ó±³¾°Í¼²ã
	auto classScene = Scene::create();
	auto bgLayer = TollgateScene::create(tollNum);
	classScene->addChild(bgLayer, BGLAYER_TAG, BGLAYER_TAG);

	//Ìí¼ÓÂß¼­Í¼²ã
	auto logicLayer = LogicLayer_T::create(tollNum);
	classScene->addChild(logicLayer, LOGICLAYER_TAG, LOGICLAYER_TAG);

	//Ìí¼Ó²Ëµ¥Í¼²ã
	auto menuLayer = MenuLayer_T::create();
	classScene->addChild(menuLayer, MENULAYER_TAG, MENULAYER_TAG);

	return classScene;
}

TollgateScene* TollgateScene::create(int tollNum){
	TollgateScene *pRet = new TollgateScene();
	if (pRet && pRet->init(tollNum))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool TollgateScene::init(int tollNum){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//¼ÓÔØ±³¾°
	Sprite* tollgateBG = Sprite::create(StringUtils::format("bg/%d.png",tollNum));
	this->addChild(tollgateBG, 0);
	tollgateBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//±êÌâ
	Sprite* tollTitle = Sprite::create("tollTitle.png");
	this->addChild(tollTitle, 0);
	tollTitle->setPosition(Point(visibleSize.width * 0.5, visibleSize.height * 0.9));

	return true;
}
