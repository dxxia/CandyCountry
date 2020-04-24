#include "MainScene.h"
#include "ClassScene.h"
#include "TollgateScene.h"
#include "ChooseScene.h"
#include "LevelScene.h"
#include "SimpleAudioEngine.h"


MainScene::MainScene(){
	//²¥·ÅÒôÀÖ
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/mainBG.mp3", true);

	voiceEnable = true;
}

MainScene::~MainScene(){
}

Scene* MainScene::createScene(){
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer, 0);
	return scene;
}

bool MainScene::init(){
	if (!Layer::init()){
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//ÉèÖÃ±³¾°
	Sprite* mainBG = Sprite::create("bg/mainBG.png");
	this->addChild(mainBG, 0);
	mainBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//ÉèÖÃ±êÌâ
	Sprite* mainTitle = Sprite::create("mainTitle.png");
	this->addChild(mainTitle, 2);
	mainTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 4 / 5));
	//ÉèÖÃ°´Å¥
	Button* classBtn = Button::create("menu/classBtn.png");
	classBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height * 5 / 8));
	classBtn->addTouchEventListener(this, toucheventselector(MainScene::classBtn));
	this->addChild(classBtn, 5);
	//°´Å¥¶¯»­
	auto a11 = ScaleTo::create(0.8, 1.05, 0.9, 1.0);
	auto a12 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
	auto a13 = Sequence::create(a11, a12, NULL);
	auto a14 = RepeatForever::create(a13);
	classBtn->runAction(a14);

	Button* tollBtn = Button::create("menu/tollBtn.png");
	tollBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height * 4 / 8));
	tollBtn->addTouchEventListener(this, toucheventselector(MainScene::tollBtn));
	this->addChild(tollBtn, 5);
	//°´Å¥¶¯»­
	auto a21 = ScaleTo::create(0.8, 1.05, 0.9, 1.0);
	auto a22 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
	auto a23 = Sequence::create(a21, a22, NULL);
	auto a24 = RepeatForever::create(a23);
	tollBtn->runAction(a24);

	Button* levelBtn = Button::create("menu/levelBtn.png");
	levelBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height * 3 / 8));
	levelBtn->addTouchEventListener(this, toucheventselector(MainScene::levelBtn));
	this->addChild(levelBtn, 5);
	//°´Å¥¶¯»­
	auto a31 = ScaleTo::create(0.8, 1.05, 0.9, 1.0);
	auto a32 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
	auto a33 = Sequence::create(a31, a32, NULL);
	auto a34 = RepeatForever::create(a33);
	levelBtn->runAction(a34);

	Button* exitGameBtn = Button::create("menu/exitGameBtn.png");
	exitGameBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height * 2 / 8));
	exitGameBtn->addTouchEventListener(this, toucheventselector(MainScene::exitGameBtn));
	this->addChild(exitGameBtn, 5);
	//°´Å¥¶¯»­
	auto a41 = ScaleTo::create(0.8, 1.05, 0.9, 1.0);
	auto a42 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
	auto a43 = Sequence::create(a41, a42, NULL);
	auto a44 = RepeatForever::create(a43);
	exitGameBtn->runAction(a44);

	Button* voiceBtn = Button::create("menu/voiceBtn_enable.png");
	voiceBtn->setPosition(Point(visibleSize.width * 19 / 20, visibleSize.height * 19 / 20));
	voiceBtn->addTouchEventListener(this, toucheventselector(MainScene::voiceBtn));
	this->addChild(voiceBtn, 5);

	return true;
}

void MainScene::classBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//²¥·ÅÒôÐ§
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//ÇÐ»»³¡¾°
		Director::getInstance()->replaceScene(ClassScene::createScene());
		break;
	default:
		break;
	}
}

void MainScene::tollBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//²¥·ÅÒôÐ§
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//ÇÐ»»³¡¾°
		Director::getInstance()->replaceScene(ChooseScene::createScene());
		break;
	default:
		break;
	}
}
void MainScene::levelBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//²¥·ÅÒôÐ§
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//ÇÐ»»³¡¾°
		Director::getInstance()->replaceScene(LevelScene::createScene());
		break;
	default:
		break;
	}
}

void MainScene::exitGameBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//²¥·ÅÒôÐ§
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//ÕÚµ²Í¼²ã
		LayerColor* lc = LayerColor::create(Color4B(0, 0, 0, 230));
		this->addChild(lc, 10, 10);
		//¶ÔÏÂ²ãÍ¼²ãÆÁ±Î
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch* touch, Event* event){
			return true;
		};
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, lc);
		//¶Ô»°¿ò±³¾°
		Size visibelSize = Director::getInstance()->getVisibleSize();
		Sprite* exitBG = Sprite::create("menu/exitBG.png");
		exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
		lc->addChild(exitBG);
		//¶Ô»°¿ò°´Å¥
		Button* exitSureBtn = Button::create("menu/exitSureBtn.png");
		exitSureBtn->setPosition(Point(exitBG->getContentSize().width * 0.3, exitBG->getContentSize().height * 0.33));
		exitSureBtn->addTouchEventListener(this, toucheventselector(MainScene::exitSureBtn));
		exitBG->addChild(exitSureBtn);

		Button* exitCancelBtn = Button::create("menu/exitCancelBtn.png");
		exitCancelBtn->setPosition(Point(exitBG->getContentSize().width * 0.7, exitBG->getContentSize().height * 0.33));
		exitCancelBtn->addTouchEventListener(this, toucheventselector(MainScene::exitCancelBtn));
		exitBG->addChild(exitCancelBtn);

		break;
	}
}

void MainScene::voiceBtn(Node* target, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (voiceEnable){
			voiceEnable = false;
			//¹Ø±ÕÒôÀÖ
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
			//Í¼Æ¬¸ü»»
			target->removeFromParent();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Button* voiceBtn = Button::create("menu/voiceBtn_disable.png");
			voiceBtn->setPosition(Point(visibleSize.width * 19 / 20, visibleSize.height * 19 / 20));
			voiceBtn->addTouchEventListener(this, toucheventselector(MainScene::voiceBtn));
			this->addChild(voiceBtn, 5);
		}
		else{
			voiceEnable = true;
			//¿ªÆôÒôÀÖ
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
			//Í¼Æ¬¸ü»»
			target->removeFromParent();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Button* voiceBtn = Button::create("menu/voiceBtn_enable.png");
			voiceBtn->setPosition(Point(visibleSize.width * 19 / 20, visibleSize.height * 19 / 20));
			voiceBtn->addTouchEventListener(this, toucheventselector(MainScene::voiceBtn));
			this->addChild(voiceBtn, 5);
		}
		break;
	default:
		break;
	}
}

void MainScene::exitSureBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		CocosDenshion::SimpleAudioEngine::getInstance()->end();
		Director::getInstance()->end();
		break;
	}
}

void MainScene::exitCancelBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		this->removeChildByTag(10);
		break;
	}
}