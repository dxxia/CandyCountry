#pragma once

#include "ItemBox_T.h"
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

class LogicLayer_T : public Layer
{
public:
	LogicLayer_T();
	~LogicLayer_T();

	bool init();
	static LogicLayer_T* create(int tollNum);
private:
	//棋盘
	ItemBox* m_itemBox;

	//状态更新
	void update(float dt);

	//得分更新,增加分数
	void updateGoal(Ref* addGoal);

	//步数更新
	void updateStep(Ref*);

	//静止时间重置
	void stopStaticTime(Ref*);

	//每步结束进行gameover判断
	void stepEnd(Ref*);

	//胜利
	void onWin();
	//失败
	void onLose();
	//结束确定按钮
	void exitSureBtn(Ref*, TouchEventType type);

	static int tollNum;//当前关卡
	int remainStep;//剩余步数
	float staticTime;//累计静止时间
	int goal;//得分
	int level2;
	int level3;
	int target;//游戏目标 
	Label* stepLabel;//步数标签    经常改动情况下用另一种label
	Label* goalLabel;//得分标签
};
