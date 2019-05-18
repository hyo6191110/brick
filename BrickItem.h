#ifndef __BRICKITEM_H__
#define __BRICKITEM_H__

#include "cocos2d.h"

enum ItemType
{
	BallPlus,
	LifeRecover,
	Expande,
	Shrink,
	BallPlus2,
	Points
};

class BrickItem :public cocos2d::Sprite
{
	friend class Game;
public:
	static BrickItem* create(const std::string& filename)
	{
		auto sprite = new BrickItem();
		if (sprite&&sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x, float y);
	
private:
	ItemType type;
	static const int Max_ItemTypes;
	float dropspeed = 240;
};

#endif

