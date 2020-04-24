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
	
	//设置物体特殊状态
	void setStateType(StateType stateType);

	//获取物体特殊状态
	StateType getStateType();

	//绑定类型并替换图片
	void bindSprite(Sprite* sprite);

	//绑定类型并替换图片
	void bindSprite(ItemType type);

	//点击时动作
	void onTouched();

	//结束点击时动作
	void offTouched();
private:
	StateType m_stateType;//特殊状态
};

