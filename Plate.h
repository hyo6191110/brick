#ifndef __PLATE_H__
#define __PLATE_H__

#include "cocos2d.h"
class Plate :public cocos2d::Sprite
{
	friend class Game;
public:
	static Plate* create(const std::string& filename)
	{
		auto sprite = new Plate();
		if (sprite&&sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x,float y);
	
private:
	float _velocity=340;
	float _startPosition=0;
};


#endif

