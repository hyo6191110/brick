#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
#include "Plate.h"
class Ball :public cocos2d::Sprite
{
public:
	static Ball* create(const std::string& filename)
	{
		auto sprite = new Ball();
		if (sprite&&sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x, float y);
	float getSpeed() { return _speed; }
	void  setSpeedMultiple(float multiply) { _speed *= multiply; }
	float getVerticalVelo() { return _vertical_velo; }
	void    setPlateBelongto(Plate* p) { _plate_belongto = p; }
	Plate*  getPlateBelongto() { return _plate_belongto; }
	void  shootBall(Plate* shooter);
private:
	float _vertical_velo=500;//这只是一个量度，速度决定公式规定板移动距离越长球速度越水平，但不会超过60度角
	float _speed = 500;       //speed使球速度大小均等，配合弹性1.0的板球砖墙，可使球一直保持此速度
	Plate* _plate_belongto;
};


#endif

