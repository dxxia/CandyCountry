#include "ChooseScene.h"
#include "MainScene.h"
#include "TollgateScene.h"
#include "SimpleAudioEngine.h"


ChooseScene::ChooseScene(){
	//播放音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/chooseBG.mp3", true);
}

ChooseScene::~ChooseScene(){
}

Scene* ChooseScene::createScene(){
	//添加背景图层
	auto chooseScene = Scene::create();
	auto bgLayer = ChooseScene::create();
	chooseScene->addChild(bgLayer);

	//退出按钮

	return chooseScene;
}

bool ChooseScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//加载背景
	Sprite* tollgateBG = Sprite::create("bg/chooseBG.png");
	this->addChild(tollgateBG, 0);
	tollgateBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//标题
	Sprite* tollTitle = Sprite::create("chooseTitle.png");
	this->addChild(tollTitle, 2);
	tollTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height * 7 / 8));
	//关卡选择按钮
	Button* chooseBtn[3][3];
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			chooseBtn[i][j] = Button::create("menu/chooseBtn.png");
			chooseBtn[i][j]->setPosition(Point(110 + j * (100 + 60), 640 - i * (100 + 60)));
			int tag = i * 3 + j + 1;
			this->addChild(chooseBtn[i][j],2,tag);
			chooseBtn[i][j]->addTouchEventListener(this, toucheventselector(ChooseScene::choose));
			//序号
			Label* label = Label::create(StringUtils::toString(tag), "Arial", 30);
			label->setPosition(50, 70);
			chooseBtn[i][j]->addChild(label, 2);
			//按钮动画
			auto a1 = ScaleTo::create(0.8, 1.05, 0.95, 1.0);
			auto a2 = ScaleTo::create(0.8, 1.0, 1.0, 1.0);
			auto a3 = Sequence::create(a1, a2, NULL);
			auto a4 = RepeatForever::create(a3);
			chooseBtn[i][j]->runAction(a4);

			//获得星级
			Sprite *sp1, *sp2, *sp3;
			switch (UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level_toll%d", tag).c_str(), 0))
			{
			case 3:
				sp3 = Sprite::create("menu/star.png");
				sp3->setPosition(chooseBtn[i][j]->getContentSize().width * 0.9, chooseBtn[i][j]->getContentSize().height * 0.3);
				chooseBtn[i][j]->addChild(sp3, 5);
			case 2:
				sp2 = Sprite::create("menu/star.png");
				sp2->setPosition(chooseBtn[i][j]->getContentSize().width * 0.5, chooseBtn[i][j]->getContentSize().height * 0.2);
				chooseBtn[i][j]->addChild(sp2, 5);
			case 1:
				sp1 = Sprite::create("menu/star.png");
				sp1->setPosition(chooseBtn[i][j]->getContentSize().width * 0.1, chooseBtn[i][j]->getContentSize().height * 0.3);
				chooseBtn[i][j]->addChild(sp1, 5);
			default:
				break;
			}
		}
	}
	//退出按钮
	Button* exitBtn = Button::create("menu/exitBtn.png");
	exitBtn->setPosition(Point(visibleSize.width * 9 / 10, visibleSize.height * 1 / 10));
	exitBtn->addTouchEventListener(this, toucheventselector(ChooseScene::exitBtn));
	this->addChild(exitBtn);

	return true;
}

void ChooseScene::choose(Node* target, TouchEventType type){
	int tag = target->getTag();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//播放音效
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//切换场景
		Director::getInstance()->replaceScene(TollgateScene::createScene(tag));
		break;
	}
}
void ChooseScene::exitBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		//播放音效
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/btnClick.wav");
		//切换场景
		Director::getInstance()->replaceScene(MainScene::createScene());
		break;
	}
}