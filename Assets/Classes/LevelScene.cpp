#include "LevelScene.h"
#include "LogicLayer_L.h"
#include "MenuLayer_L.h"
#include "SimpleAudioEngine.h"

LevelScene::LevelScene(){
	//��������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/levelBG.mp3", true);
}

LevelScene::~LevelScene(){
}

Scene* LevelScene::createScene(){
	//��ӱ���ͼ��
	auto classScene = Scene::create();
	auto bgLayer = LevelScene::create();
	classScene->addChild(bgLayer, BGLAYER_TAG, BGLAYER_TAG);

	//����߼�ͼ��
	auto logicLayer = LogicLayer_L::create();
	classScene->addChild(logicLayer, LOGICLAYER_TAG, LOGICLAYER_TAG);

	//��Ӳ˵�ͼ��
	auto menuLayer = MenuLayer_L::create();
	classScene->addChild(menuLayer, MENULAYER_TAG, MENULAYER_TAG);

	return classScene;
}

bool LevelScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���ر���
	Sprite* levelBG = Sprite::create("bg/levelBG.png");
	this->addChild(levelBG, 0);
	levelBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//����
	Sprite* levelTitle = Sprite::create("levelTitle.png");
	this->addChild(levelTitle, 0);
	levelTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 9 / 10));

	return true;
}
