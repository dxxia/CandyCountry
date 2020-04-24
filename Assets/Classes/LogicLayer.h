#pragma once
#include "cocos2d.h"
#include "ItemBox_C.h"
USING_NS_CC;

class LogicLayer : public Layer
{
public:
	LogicLayer();
	~LogicLayer();

	bool init();
	CREATE_FUNC(LogicLayer);

	//暂停监听
	void pauseObserve(Ref*);

	//静止时间重置
	void stopStaticTime(Ref*);

	//暂停恢复监听
	void releaseObserve(Ref*);
private:
	//棋盘
	ItemBox* m_itemBox;

	//状态更新
	void update(float dt);

	//得分更新
	void addGoal(Ref* addGoal);

	float remainTime;//剩余时间
	int goal;//得分
	float staticTime;//累计静止时间
	bool timeTick;//时间提醒
	Label* timeLabel;//时间标签    经常改动情况下用另一种label
	Label* goalLabel;//得分标签
};
