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
	//���عؿ�����
	std::string data = FileUtils::getInstance()->getStringFromFile("tollgate.json");
	std::string tollNum_s = StringUtils::format("%d", tollNum);
	if (read.parse(data, root, false)){
		//��ʼ����
		remainStep = root[tollNum_s]["steps"].asInt();
		//����Ŀ��������
		target = root[tollNum_s]["target"].asInt();
		//���صȼ��ж�����
		level2 = root[tollNum_s]["level2"].asInt();
		level3 = root[tollNum_s]["level3"].asInt();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//������Ʒ����
	ItemBox_T* itemBox = ItemBox_T::create(tollNum);
	this->addChild(itemBox, 1);
	itemBox->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	m_itemBox = itemBox;

	//��������
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::updateGoal), "addGoal", NULL);

	//��ֹʱ�����
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::stopStaticTime), "stopStaticTime", NULL);

	//��������
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::updateStep), "updateStep", NULL);

	//��Ϸ������������
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LogicLayer_T::stepEnd), "stepEnd", NULL);

	//������ǩ
	Sprite* goalShow = Sprite::create("menu/goalShow.png");
	this->addChild(goalShow, 5);
	goalShow->setPosition(visibleSize.width * 0.4, visibleSize.height * 0.82);

	goalLabel = Label::createWithCharMap("menu/num.png", 32, 42, '0');
	goalLabel->setString("0");
	this->addChild(goalLabel, 5);
	goalLabel->setPosition(visibleSize.width * 0.6, visibleSize.height * 0.82);

	//������ǩ
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
		//�ƶ���ʾ����
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
	//����������ʼ���㾲ֹʱ��
	staticTime = 0;

	bool exist = false;
	switch (target){
	case 1://Ŀ��Ϊ������
		for (int i = 0; i < m_itemBox->getCellNum().x; i++){
			for (int j = 0; j < m_itemBox->getCellNum().y; j++){
				if (m_itemBox->getItem_lgc(i, j)->getItemType() == ItemType_Drop){
					exist = true;
				}
			}
		}
		if (exist == false){
			//ʤ��
			updateGoal((Ref*)(remainStep * 10));
			remainStep = 1;
			updateStep(NULL);
			onWin();
			return;
		}
		break;
	case 0://��Ŀ��
		if (remainStep <= 0){
			//ʤ��
			onWin();
			return;
		}
		break;
	}

	//��Ϸʧ��
	if (remainStep <= 0){
		//lose
		onLose();
	}
}

void LogicLayer_T::onWin(){
	//����
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/win.mp3");
	//�жϵ�ǰ��õȼ�
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

	//�ڵ�ͼ��
	this->setLocalZOrder(5);
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
	Sprite* exitBG = Sprite::create("menu/win.png");
	exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
	lc->addChild(exitBG);
	//����Ǽ�
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
	//�Ի���ť
	Button* exitSureBtn = Button::create("menu/exitSureBtn.png");
	exitSureBtn->setPosition(Point(exitBG->getContentSize().width * 0.5, exitBG->getContentSize().height * 0.2));
	exitSureBtn->addTouchEventListener(this, toucheventselector(LogicLayer_T::exitSureBtn));
	exitBG->addChild(exitSureBtn);

}

void LogicLayer_T::onLose(){
	//����
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/lose.mp3");

	this->setLocalZOrder(5);
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
	Sprite* exitBG = Sprite::create("menu/lose.png");
	exitBG->setPosition(visibelSize.width * 0.5, visibelSize.height * 0.5);
	lc->addChild(exitBG);
	//�Ի���ť
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