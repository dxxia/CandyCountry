#include "Entity.h"
#include "cocos2d.h"
#include "stateType_T.h"
#include "ItemType_T.h"
#include "ActionState_T.h"
USING_NS_CC;

class ClearItem_T : public Entity
{
public:
	ClearItem_T();
	~ClearItem_T();

	bool init();
	CREATE_FUNC(ClearItem_T);
	
	//������������״̬
	void setStateType(StateType stateType);

	//��ȡ��������״̬
	StateType getStateType();

	//�����Ͳ��滻ͼƬ
	void bindSprite(Sprite* sprite);

	//�����Ͳ��滻ͼƬ
	void bindSprite(ItemType type);

	//���ʱ����
	void onTouched();

	//�������ʱ����
	void offTouched();
private:
	StateType m_stateType;//����״̬
};

