#include "LogoScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"


LogoScene::LogoScene(){
	//预加载全部音效
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tick.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/titlePop.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/btnClick.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/mainBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/classBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/timeTick.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/accountBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/tollBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/chooseBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/win.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/lose.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/levelBG.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/touchItem.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/notSwap.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/moveBack.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/clear.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/skill.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/createSkill.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/skillMove.wav");
}

LogoScene::~LogoScene(){
}

Scene* LogoScene::createScene(){
	auto scene = Scene::create();
	auto layer = LogoScene::create();
	scene->addChild(layer, 0);
	return scene;
}

bool LogoScene::init(){
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置背景
	Sprite* mainBG = Sprite::create("bg/loginBG.png");
	this->addChild(mainBG, 0);
	mainBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//物体
	Sprite* item1 = Sprite::create("candy/3.png");
	item1->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.35);
	this->addChild(item1);

	Sprite* item2 = Sprite::create("candy/3.png");
	item2->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.45);
	this->addChild(item2);

	Sprite* item3 = Sprite::create("candy/3.png");
	item3->setPosition(visibleSize.width * 0.5, visibleSize.height);
	this->addChild(item3);
	//标题
	Sprite* title = Sprite::create("mainTitle.png");
	title->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.5);
	title->setScale(0);
	this->addChild(title);
	//动作
	//掉落动作
	ActionInterval* dropAction = MoveTo::create(1, Point(visibleSize.width * 0.5, visibleSize.height * 0.55));
	auto d1 = EaseExponentialIn::create(dropAction);
	
	CallFunc* callf1 = CallFunc::create([=](){
		//碰撞声音
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tick.wav");
		//消失动作
		ActionInterval* fadeOut = FadeOut::create(0.5);
		item1->runAction(fadeOut->clone());
		item2->runAction(fadeOut->clone());
		item3->runAction(fadeOut);
	});
	//标题出现动作
	CallFunc* callf2 = CallFunc::create([=](){
		//播放撞击声音
		CallFunc* playPop = CallFunc::create([](){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/titlePop.wav");
		});
		//场景切换
		CallFunc* callf3 = CallFunc::create([=](){
			Director::getInstance()->replaceScene(MainScene::createScene());
		});

		auto scale = ScaleTo::create(1.0,1.2);
		auto rotate = RotateBy::create(1.0, 360);
		auto spawn = Spawn::create(scale, rotate, NULL);
		ActionInterval* d3 = Sequence::create(spawn, playPop, DelayTime::create(1.0), callf3, NULL);
		title->runAction(d3);
	});
	auto d2 = Sequence::create(d1, callf1, callf2, NULL);
	item3->runAction(d2);

	return true;
}


