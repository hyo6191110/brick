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
		if (sprite&&sprite->initWithSpriteFrameName(filename))
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

	void  setRadius(float f) { _radius = f; }
	float getRadius() { return _radius; }
	bool  checkRadiusIntergrity() { return _radius > 0.8&&_radius < 1.4; }
	void  larger();
	void  smaller();
	void  speedUp() {if (_speed < 1100)_speed += 200;}
	void  speedDown() { if (_speed > 300)_speed -= 200; }
private:
	float _vertical_velo=500;//��ֻ��һ�����ȣ��ٶȾ�����ʽ�涨���ƶ�����Խ�����ٶ�Խˮƽ�������ᳬ��60�Ƚ�
	float _speed = 500;       //speedʹ���ٶȴ�С���ȣ���ϵ���1.0�İ���שǽ����ʹ��һֱ���ִ��ٶ�
	Plate* _plate_belongto;
	float  _radius=1.0;
};


#endif

