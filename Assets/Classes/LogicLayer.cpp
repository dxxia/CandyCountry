#include "LogicLayer.h"
#include "AccountScene_C.h"
#include "SimpleAudioEngine.h"


LogicLayer::LogicLayer()
{
	m_itemBox = NULL;
	remainTime = 60.0;
	goal = 0;
	staticTime = 0;
	timeTick = false;
	timeLabel = NULL;
	goalLabel = NULL;
	this->scheduleUpdate();
}

LogicLayer::~LogicLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool LogicLayer::init(){
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//加载物品格子
	ItemBox_C* itemBox = ItemBox_C::create();
	this->addChild(itemBox, 1);
	itemBox->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	m_itemBox = itemBox;

	//分数监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer::addGoal), "addGoal", NULL);
	//暂停和恢复监听
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer::pauseObserve), "pauseGame", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer::releaseObserve), "releaseGame", NULL);
	//监听并停止计算静止时间
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer::stopStaticTime), "stopStaticTime", NULL);

	//分数标签
	Sprite* goalShow = Sprite::create("menu/goalShow.png");
	this->addChild(goalShow, 5);
	goalShow->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.82);
	goalLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	goalLabel->setString("0");
	this->addChild(goalLabel, 5);
	goalLabel->setPosition(visibleSize.width * 0.6, visibleSize.height * 0.82);

	//时间标签
	Sprite* timeShow = Sprite::create("menu/timeShow.png");
	this->addChild(timeShow, 5);
	timeShow->setPosition(visibleSize.width * 2 / 5, visibleSize.height  * 0.18);
	timeLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	timeLabel->setString(StringUtils::toString(remainTime));
	this->addChild(timeLabel, 5);
	timeLabel->setPosition(visibleSize.width * 0.7, visibleSize.height * 0.18);
	return true;
}

void LogicLayer::stopStaticTime(Ref*){
	staticTime = -100;
}

void LogicLayer::addGoal(Ref* addGoal){
	goal = goal + (int)addGoal * 20;
	goalLabel->setString(StringUtils::toString(goal));
}


void LogicLayer::pauseObserve(Ref*){
	this->unscheduleUpdate();
}

void LogicLayer::releaseObserve(Ref*){
	this->scheduleUpdate();
}

void LogicLayer::update(float dt){

	if (staticTime > 5){
		//移动提示动画
		Entity* item1 = NULL;
		Entity* item2 = NULL;
		((Scanner_C*)m_itemBox->getScanner())->getPotentialItem(item1, item2);
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

	//每秒监听静止状态
	if ((int)remainTime != (int)(remainTime - dt)){
		for (int j = 0; j < m_itemBox->getCellNum().y; j++){
			for (int i = 0; i < m_itemBox->getCellNum().x; i++){
				if (m_itemBox->getItem_lgc(i, j)->getActionState() != Fixed){
					return;
				}

				if (j == m_itemBox->getCellNum().y - 1 && i == m_itemBox->getCellNum().x - 1 && staticTime < 0){
					staticTime = 0;
				}
			}
		}
	}

	if (remainTime > 0){
		//时间警告
		if ((int)remainTime != (int)(remainTime - dt) && remainTime <= 10){
			auto a1 = ScaleTo::create(0.4, 2);
			auto a2 = ScaleTo::create(0.4, 1);
			auto a3 = Sequence::create(a1, a2, NULL);
			timeLabel->runAction(a3);
		}
		if (!timeTick && remainTime <= 10){
			timeTick = true;
			//音效
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/timeTick.wav", true);
		}
		remainTime = remainTime - dt;
	}
	else{
		remainTime = 0;
		NotificationCenter::getInstance()->postNotification("timeOver", NULL);
		//结束判断
		for (int j = 0; j < m_itemBox->getCellNum().y; j++){
			for (int i = 0; i < m_itemBox->getCellNum().x; i++){
				if (m_itemBox->getItem_lgc(i, j)->getActionState() != Fixed){
					timeLabel->setString(StringUtils::toString((int)remainTime));
					return;
				}

				if (j == m_itemBox->getCellNum().y - 1 && i == m_itemBox->getCellNum().x - 1){
					Director::getInstance()->replaceScene(AccountScene_C::createScene(goal));
				}
			}
		}
	}
	timeLabel->setString(StringUtils::toString((int)remainTime));
}

