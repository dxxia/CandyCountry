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

	//��ͣ����
	void pauseObserve(Ref*);

	//��ֹʱ������
	void stopStaticTime(Ref*);

	//��ͣ�ָ�����
	void releaseObserve(Ref*);
private:
	//����
	ItemBox* m_itemBox;

	//״̬����
	void update(float dt);

	//�÷ָ���
	void addGoal(Ref* addGoal);

	float remainTime;//ʣ��ʱ��
	int goal;//�÷�
	float staticTime;//�ۼƾ�ֹʱ��
	bool timeTick;//ʱ������
	Label* timeLabel;//ʱ���ǩ    �����Ķ����������һ��label
	Label* goalLabel;//�÷ֱ�ǩ
};
