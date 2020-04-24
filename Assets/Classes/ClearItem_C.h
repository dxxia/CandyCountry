#include "Entity.h"
#include "cocos2d.h"
#include "ItemType_C.h"
#include "stateType_C.h"
#include "ActionState_C.h"
USING_NS_CC;

class ClearItem_C : public Entity
{
public:
	ClearItem_C();
	~ClearItem_C();

	bool init();
	CREATE_FUNC(ClearItem_C);

	//������������״̬
	void setStateType(StateType stateType);

	//��ȡ��������״̬
	StateType getStateType();

	//�����Ͳ��滻ͼƬ
	void bindSprite(ItemType type);

	//���ʱ����
	void onTouched();

	//�������ʱ����
	void offTouched();
private:
	StateType m_stateType;//����״̬
};

