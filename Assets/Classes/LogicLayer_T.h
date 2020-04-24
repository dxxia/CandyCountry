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
	//����
	ItemBox* m_itemBox;

	//״̬����
	void update(float dt);

	//�÷ָ���,���ӷ���
	void updateGoal(Ref* addGoal);

	//��������
	void updateStep(Ref*);

	//��ֹʱ������
	void stopStaticTime(Ref*);

	//ÿ����������gameover�ж�
	void stepEnd(Ref*);

	//ʤ��
	void onWin();
	//ʧ��
	void onLose();
	//����ȷ����ť
	void exitSureBtn(Ref*, TouchEventType type);

	static int tollNum;//��ǰ�ؿ�
	int remainStep;//ʣ�ಽ��
	float staticTime;//�ۼƾ�ֹʱ��
	int goal;//�÷�
	int level2;
	int level3;
	int target;//��ϷĿ�� 
	Label* stepLabel;//������ǩ    �����Ķ����������һ��label
	Label* goalLabel;//�÷ֱ�ǩ
};
