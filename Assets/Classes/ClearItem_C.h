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

	//设置物体特殊状态
	void setStateType(StateType stateType);

	//获取物体特殊状态
	StateType getStateType();

	//绑定类型并替换图片
	void bindSprite(ItemType type);

	//点击时动作
	void onTouched();

	//结束点击时动作
	void offTouched();
private:
	StateType m_stateType;//特殊状态
};

