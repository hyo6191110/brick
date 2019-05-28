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
	float _vertical_velo=500;//��ֻ��һ�����ȣ��ٶȾ�����ʽ�涨���ƶ�����Խ�����ٶ�Խˮƽ�������ᳬ��60�Ƚ�
	float _speed = 500;       //speedʹ���ٶȴ�С���ȣ���ϵ���1.0�İ���שǽ����ʹ��һֱ���ִ��ٶ�
	Plate* _plate_belongto;
};


#endif

