#include "MenuLayer.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

MenuLayer::MenuLayer()
{
}

MenuLayer::~MenuLayer()
{
}

bool MenuLayer::init(){
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//��ͣ��ť
	Button* pauseBtn = Button::create("menu/pauseBtn.png");
	pauseBtn->setPosition(Point(visibleSize.width * 9 / 10, visibleSize.height * 18.5 / 20));
	pauseBtn->addTouchEventListener(this, toucheventselector(MenuLayer::pauseBtn));
	this->addChild(pauseBtn);
	//�˳���ť
	Button* exitBtn = Button::create("menu/exitBtn.png");
	exitBtn->setPosition(Point(visibleSize.width * 9 / 10, visibleSize.height * 1 / 10));
	exitBtn->addTouchEventListener(this, toucheventselector(MenuLayer::exitBtn));
	this->addChild(exitBtn);

	return true;
}

void MenuLayer::pauseBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//������Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//�ر�����
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		//��ͣ��Ϸ
		NotificationCenter::getInstance()->postNotification("pauseGame");
		//�ڵ�ͼ��
		LayerColor* lc = LayerColor::create(Color4B(0, 0, 0, 230));
		this->addChild(lc, 10, 10);
		//���²�ͼ������
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch* touch, Event* event){
			return true;
		};
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, lc);

		//�Ի��򱳾�
		Size visibelSize = Director::getInstance()->getVisibleSize();
		Sprite* exitBG = Sprite::create("menu/pauseBG.png");
		exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
		lc->addChild(exitBG);
		//�Ի���ť
		Button* startBtn = Button::create("menu/startBtn.png");
		startBtn->setPosition(Point(exitBG->getContentSize().width * 0.5, exitBG->getContentSize().height * 0.33));
		startBtn->addTouchEventListener(this, toucheventselector(MenuLayer::startBtn));
		exitBG->addChild(startBtn);

		break;
	}
}

void MenuLayer::exitBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//������Ч
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//�ر�����
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		//��ͣ��Ϸ
		NotificationCenter::getInstance()->postNotification("pauseGame");
		//�ڵ�ͼ��
		LayerColor* lc = LayerColor::create(Color4B(0, 0, 0, 230));
		this->addChild(lc, 10, 10);
		//���²�ͼ������
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch* touch, Event* event){
			return true;
		};
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, lc);

		//�Ի��򱳾�
		Size visibelSize = Director::getInstance()->getVisibleSize();
		Sprite* exitBG = Sprite::create("menu/exitBG.png");
		exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
		lc->addChild(exitBG);
		//�Ի���ť
		Button* exitSureBtn = Button::create("menu/exitSureBtn.png");
		exitSureBtn->setPosition(Point(exitBG->getContentSize().width * 0.3, exitBG->getContentSize().height * 0.33));
		exitSureBtn->addTouchEventListener(this, toucheventselector(MenuLayer::exitSureBtn));
		exitBG->addChild(exitSureBtn);

		Button* exitCancelBtn = Button::create("menu/exitCancelBtn.png");
		exitCancelBtn->setPosition(Point(exitBG->getContentSize().width * 0.7, exitBG->getContentSize().height * 0.33));
		exitCancelBtn->addTouchEventListener(this, toucheventselector(MenuLayer::exitCancelBtn));
		exitBG->addChild(exitCancelBtn);

		break;
	}
}

void MenuLayer::exitSureBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//�ָ�����
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);

		Director::getInstance()->replaceScene(MainScene::createScene());
		break;
	default:
		break;
	}
}

void MenuLayer::exitCancelBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//�ָ�����
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
		//�ָ���Ϸ
		NotificationCenter::getInstance()->postNotification("releaseGame");
		this->removeChildByTag(10);
		break;
	}
}

void MenuLayer::startBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//�ָ�����
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
		//�ָ���Ϸ
		NotificationCenter::getInstance()->postNotification("releaseGame");
		this->removeChildByTag(10);
		break;
	default:
		break;
	}
}