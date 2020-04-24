#pragma once
#include "cocos2d.h"
#include "Controller.h"
#include "Entity.h"
USING_NS_CC;


class ItemBox : public Node
{
public:
	ItemBox();
	~ItemBox();

	//����ָ���߼������ȡ���߼��������������壬Խ�緵�ؿ�
	Entity* getItem_lgc(int i, int j);

	//��ȡָ��������߼�����
	Point getItemPos(Entity* item);

	//����ָ�����е�����
	void setItem(int x, int y, Entity* item);

	//��ȡ�󶨵��ƶ�������
	Controller* getTouchController();

	//��ȡ�󶨵�ɨ����
	Controller* getScanner();

	//��ȡ�󶨵�����������
	Controller* getClearController();

	//��ȡ�󶨵ĵ��������
	Controller* getDropController();

	//��ȡ����ֵ
	Point getCellNum();

	//��ȡ��Ԫ���С
	Point getCellSize();
protected:

	Controller* m_touchController;//�ƶ�������
	Controller* m_scanner;//ɨ����
	Controller* m_clearController;//����������
	Controller* m_dropController;//���������
	Point cellNum;//����ֵ
	Point cellSize;//��Ԫ���С
	Entity*** items;//��������
private:
};

