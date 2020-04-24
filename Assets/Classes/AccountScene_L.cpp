#include "AccountScene_L.h"
#include "MainScene.h"
#include "LogicLayer_L.h"
#include "SimpleAudioEngine.h"

int AccountScene_L::nowGoal = 0;

AccountScene_L::AccountScene_L(){
	//��������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/accountBG.mp3");

	bestGoal = UserDefault::getInstance()->getIntegerForKey("bestGoal_L", 0);
	if (bestGoal < nowGoal){
		UserDefault::getInstance()->setIntegerForKey("bestGoal_L", nowGoal);
	}
}

AccountScene_L::~AccountScene_L(){
}

Scene* AccountScene_L::createScene(int goal){
	AccountScene_L::nowGoal = goal;
	//��ӱ���ͼ��
	auto accountScene = Scene::create();
	auto bgLayer = AccountScene_L::create();
	accountScene->addChild(bgLayer);

	return accountScene;
}

bool AccountScene_L::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���ر���
	Sprite* accountBG = Sprite::create("bg/accountBG.png");
	this->addChild(accountBG, 0);
	accountBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//����
	Sprite* accountTitle = Sprite::create("accountTitle.png");
	this->addChild(accountTitle, 0);
	accountTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 4 / 5));
	//��ť
	Button* exitBtn = Button::create("menu/acBackBtn.png");
	exitBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height * 1 / 10));
	exitBtn->addTouchEventListener(this, toucheventselector(AccountScene_L::exitBtn));
	this->addChild(exitBtn);
	auto a1 = ScaleTo::create(0.8, 1.05, 0.9, 1.0);
	auto a2 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
	auto a3 = Sequence::create(a1, a2, NULL);
	auto a4 = RepeatForever::create(a3);
	exitBtn->runAction(a4);
	//��ǰ�÷�
	Label* nGoal = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	nGoal->setString(StringUtils::toString(nowGoal));
	nGoal->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.58);
	this->addChild(nGoal, 5);
	//��ʷ��߷�
	Label* bGoal = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	bGoal->setString(StringUtils::toString(bestGoal));
	bGoal->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.43);
	this->addChild(bGoal, 5);

	return true;
}

void AccountScene_L::exitBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//������Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//�л�����
		Director::getInstance()->replaceScene(MainScene::createScene());
		break;
	default:
		break;
	}
}
