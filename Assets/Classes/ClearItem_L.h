#include "Entity.h"
#include "cocos2d.h"
#include "ItemType_L.h"
#include "ActionState_L.h"
USING_NS_CC;

class ClearItem_L : public Entity
{
public:
	ClearItem_L();
	~ClearItem_L();

	bool init();
	CREATE_FUNC(ClearItem_L);

	//设置物体等级
	void setItemLevel(int level);

	//获取物体等级
	int getItemLevel();

	//绑定类型并替换图片
	void bindSprite(Sprite* sprite);

	//绑定类型并替换图片
	void bindSprite(ItemType type);

	//点击时动作
	void onTouched();

	//结束点击时动作
	void offTouched();
private:
	int m_itemLevel;//物体等级
	Label* levelLabel;
};

