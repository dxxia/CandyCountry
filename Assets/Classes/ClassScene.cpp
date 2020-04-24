#include "ClassScene.h"
#include "MainScene.h"
#include "LogicLayer.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"


ClassScene::ClassScene(){
	//��������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/classBG.mp3", true);
}

ClassScene::~ClassScene(){

}

Scene* ClassScene::createScene(){
	//��ӱ���ͼ��
	auto classScene = Scene::create();
	auto bgLayer = ClassScene::create();
	classScene->addChild(bgLayer, BGLAYER_TAG, BGLAYER_TAG);

	//����߼�ͼ��
	auto logicLayer = LogicLayer::create();
	classScene->addChild(logicLayer, LOGICLAYER_TAG, LOGICLAYER_TAG);

	//��Ӳ˵�ͼ��
	auto menuLayer = MenuLayer::create();
	classScene->addChild(menuLayer, MENULAYER_TAG, MENULAYER_TAG);

	return classScene;
}

bool ClassScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���ر���
	Sprite* classBG = Sprite::create("bg/classBG.png");
	this->addChild(classBG, 0);
	classBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//����
	Sprite* classTitle = Sprite::create("classTitle.png");
	this->addChild(classTitle, 0);
	classTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 9 / 10));

	return true;
}
