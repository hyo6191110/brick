#ifndef __BRICKITEM_H__
#define __BRICKITEM_H__

#include "cocos2d.h"
using namespace cocos2d;
enum ItemType
{
	BallPlus,
	LifeRecover,
	Expande,
	Shrink,
	BallSpeedUp,
	BallSpeedDown,
	PlateSpeedUp,
	PlateSpeedDown,
	BallLarger,
	BallSmaller,
	Points
};

class BrickItem :public cocos2d::Sprite
{
public:
	static BrickItem* create(const std::string fileName)
	{
		auto sprite = new BrickItem();
		if (sprite&&sprite->initWithSpriteFrameName(fileName))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x, float y);
	float getDropspeed() { return _dropspeed; }
	ItemType getItemtype() { return _type; }
	void setItemType(ItemType t) { _type = t; }
	static int getMaxitemtype(){return Max_ItemTypes;}
private:
	ItemType _type;
	static const int Max_ItemTypes;
	float _dropspeed = 240;
};

#endif

