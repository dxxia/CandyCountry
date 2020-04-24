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

	//��������ȼ�
	void setItemLevel(int level);

	//��ȡ����ȼ�
	int getItemLevel();

	//�����Ͳ��滻ͼƬ
	void bindSprite(Sprite* sprite);

	//�����Ͳ��滻ͼƬ
	void bindSprite(ItemType type);

	//���ʱ����
	void onTouched();

	//�������ʱ����
	void offTouched();
private:
	int m_itemLevel;//����ȼ�
	Label* levelLabel;
};

