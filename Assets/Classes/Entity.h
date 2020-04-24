#pragma once
#include "cocos2d.h"
USING_NS_CC;

//enum ItemType;
//enum ActionState;
//�������
class Entity : public Node
{
public:
	Entity();
	~Entity();

	//�󶨾���
	virtual void bindSprite(Sprite* sprite);

	//������������
	void setItemType(int type);

	//��ȡ��������
	int getItemType();

	//�����˶�״̬
	void setActionState(int actionState);

	//��ȡ�˶�״̬
	int getActionState();

	//���������
	virtual void onTouched() = 0;

	//����ָ�����
	virtual void offTouched() = 0;
protected:
	Sprite* m_sprite;//����
	int m_itemType;//��������
	int m_actionState;//�˶�����
};

