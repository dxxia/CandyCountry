#include "Entity.h"
#include "cocos2d.h"
#include "ItemType_T.h"
#include "ActionState_T.h"
USING_NS_CC;

class DropItem_T : public Entity
{
public:
	DropItem_T();
	~DropItem_T();

	bool init();
	CREATE_FUNC(DropItem_T);

	//���ʱ����
	void onTouched();

	//�������ʱ����
	void offTouched();
private:
};

