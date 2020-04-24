#include "Entity.h"
#include "cocos2d.h"
#include "ItemType_T.h"
#include "ActionState_T.h"
USING_NS_CC;

class BreakItem_T : public Entity
{
public:
	BreakItem_T();
	~BreakItem_T();

	bool init();
	CREATE_FUNC(BreakItem_T);

	//点击时动作
	void onTouched();

	//结束点击时动作
	void offTouched();
private:
};

