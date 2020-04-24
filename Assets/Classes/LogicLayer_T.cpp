#include "LogicLayer_T.h"
#include "json\reader.h"
#include "json\value.h"
#include "ChooseScene.h"
#include "SimpleAudioEngine.h"

int LogicLayer_T::tollNum = 1;

LogicLayer_T::LogicLayer_T()
{
	m_itemBox = NULL;
	remainStep = 0;
	target = 0;
	goal = 0;
	staticTime = 0;
	level2 = 0;
	level3 = 0;
	stepLabel = NULL;
	goalLabel = NULL;

	this->scheduleUpdate();
}

LogicLayer_T::~LogicLayer_T()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

LogicLayer_T* LogicLayer_T::create(int tollNum){
	LogicLayer_T::tollNum = tollNum;
	LogicLayer_T *pRet = new LogicLayer_T();
	if (pRet && pRet->init())
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

bool LogicLayer_T::init(){
	if (!Layer::init()){
		return false;
	}
	Json::Reader read;
	Json::Value root;
	//加载关卡数据
	std::string data = FileUtils::getInstance()->getStringFromFile("tollgate.json");
	std::string tollNum_s = StringUtils::format("%d", tollNum);
	if (read.parse(data, root, false)){
		//初始步数
		remainStep = root[tollNum_s]["steps"].asInt();
		//加载目标物数据
		target = root[tollNum_s]["target"].asInt();
		//加载等级判定界限
		level2 = root[tollNum_s]["level2"].asInt();
		level3 = root[tollNum_s]["level3"].asInt();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//加载物品格子
	ItemBox_T* itemBox = ItemBox_T::create(tollNum);
	this->addChild(itemBox, 1);
	itemBox->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	m_itemBox = itemBox;

	//分数监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::updateGoal), "addGoal", NULL);

	//静止时间监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::stopStaticTime), "stopStaticTime", NULL);

	//步数监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::updateStep), "updateStep", NULL);

	//游戏结束条件监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::stepEnd), "stepEnd", NULL);

	//分数标签
	Sprite* goalShow = Sprite::create("menu/goalShow.png");
	this->addChild(goalShow, 5);
	goalShow->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.82);

	goalLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	goalLabel->setString("0");
	this->addChild(goalLabel, 5);
	goalLabel->setPosition(visibleSize.width * 0.6, visibleSize.height * 0.82);

	//步数标签
	Sprite* stepShow = Sprite::create("menu/stepShow.png");
	this->addChild(stepShow, 5);
	stepShow->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.17);

	stepLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	stepLabel->setString(StringUtils::toString(remainStep));
	this->addChild(stepLabel, 5);
	stepLabel->setPosition(visibleSize.width * 0.7, visibleSize.height * 0.17);
	return true;
}

void LogicLayer_T::update(float dt){
	if (staticTime > 5){
		//移动提示动画
		Entity* item1 = NULL;
		Entity* item2 = NULL;
		((Scanner_T*)m_itemBox->getScanner())->getPotentialItem(item1, item2);
		if (item1 != NULL && item2 != NULL){
			auto a1 = FadeIn::create(0.2);
			auto a2 = MoveTo::create(0.5, this->convertToNodeSpace(m_itemBox->convertToWorldSpace(item2->getPosition())));
			auto a3 = FadeOut::create(0.2);
			auto a4 = Sequence::create(a1, a2, a3, NULL);

			Sprite* hand = Sprite::create("menu/hand.png");
			hand->setPosition(this->convertToNodeSpace(m_itemBox->convertToWorldSpace(item1->getPosition())));
			hand->setAnchorPoint(Point(0, 1));
			hand->setOpacity(0);
			this->addChild(hand, 5);
			hand->runAction(a4);

			staticTime = 3;
		}
	}
	staticTime += dt;
}

void LogicLayer_T::updateGoal(Ref* addGoal){
	goal = goal + (int)addGoal * 20;
	goalLabel->setString(StringUtils::toString(goal));
}

void LogicLayer_T::stopStaticTime(Ref*){
	staticTime = -100;
}

void LogicLayer_T::updateStep(Ref*){
	remainStep--;
	stepLabel->setString(StringUtils::toString(remainStep));
}


void LogicLayer_T::stepEnd(Ref*){
	//步数结束后开始计算静止时间
	staticTime = 0;

	bool exist = false;
	switch (target){
	case 1://目标为掉落物
		for (int i = 0; i < m_itemBox->getCellNum().x; i++){
			for (int j = 0; j < m_itemBox->getCellNum().y; j++){
				if (m_itemBox->getItem_lgc(i, j)->getItemType() == ItemType_Drop){
					exist = true;
				}
			}
		}
		if (exist == false){
			//胜利
			updateGoal((Ref*)(remainStep * 10));
			remainStep = 1;
			updateStep(NULL);
			onWin();
			return;
		}
		break;
	case 0://无目标
		if (remainStep <= 0){
			//胜利
			onWin();
			return;
		}
		break;
	}

	//游戏失败
	if (remainStep <= 0){
		//lose
		onLose();
	}
}

void LogicLayer_T::onWin(){
	//音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/win.mp3");
	//判断当前获得等级
	int level = 0;
	if (goal >= level3){
		level = 3;
	}
	else if (goal >= level2){
		level = 2;
	}
	else{
		level = 1;
	}
	if (level > UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level_toll%d", tollNum).c_str(), 0)){
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("level_toll%d", tollNum).c_str(), level);
	}

	//遮挡图层
	this->setLocalZOrder(5);
	LayerColor* lc = LayerColor::create(Color4B(0, 0, 0, 230));
	this->addChild(lc, 10, 10);
	//对下层图层屏蔽
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, lc);
	//对话框背景
	Size visibelSize = Director::getInstance()->getVisibleSize();
	Sprite* exitBG = Sprite::create("menu/win.png");
	exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
	lc->addChild(exitBG);
	//获得星级
	Sprite *sp1, *sp2, *sp3;
	switch (UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level_toll%d", tollNum).c_str(), 0))
	{
	case 3:
		sp3 = Sprite::create("menu/star.png");
		sp3->setScale(1.5);
		sp3->setPosition(exitBG->getContentSize().width * 0.7, exitBG->getContentSize().height * 0.4);
		exitBG->addChild(sp3, 5);
	case 2:
		sp2 = Sprite::create("menu/star.png");
		sp2->setScale(1.5);
		sp2->setPosition(exitBG->getContentSize().width * 0.5, exitBG->getContentSize().height * 0.4);
		exitBG->addChild(sp2, 5);
	case 1:
		sp1 = Sprite::create("menu/star.png");
		sp1->setScale(1.5);
		sp1->setPosition(exitBG->getContentSize().width * 0.3, exitBG->getContentSize().height * 0.4);
		exitBG->addChild(sp1, 5);
	default:
		break;
	}
	//对话框按钮
	Button* exitSureBtn = Button::create("menu/exitSureBtn.png");
	exitSureBtn->setPosition(Point(exitBG->getContentSize().width * 0.5, exitBG->getContentSize().height * 0.2));
	exitSureBtn->addTouchEventListener(this, toucheventselector(LogicLayer_T::exitSureBtn));
	exitBG->addChild(exitSureBtn);

}

void LogicLayer_T::onLose(){
	//音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/lose.mp3");

	this->setLocalZOrder(5);
	//遮挡图层
	LayerColor* lc = LayerColor::create(Color4B(0, 0, 0, 230));
	this->addChild(lc, 10, 10);
	//对下层图层屏蔽
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, lc);
	//对话框背景
	Size visibelSize = Director::getInstance()->getVisibleSize();
	Sprite* exitBG = Sprite::create("menu/lose.png");
	exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
	lc->addChild(exitBG);
	//对话框按钮
	Button* exitSureBtn = Button::create("menu/exitSureBtn.png");
	exitSureBtn->setPosition(Point(exitBG->getContentSize().width * 0.5, exitBG->getContentSize().height * 0.3));
	exitSureBtn->addTouchEventListener(this, toucheventselector(LogicLayer_T::exitSureBtn));
	exitBG->addChild(exitSureBtn);
}

void LogicLayer_T::exitSureBtn(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		Director::getInstance()->replaceScene(ChooseScene::createScene());
		break;
	}
}