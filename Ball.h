#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
class Ball :public cocos2d::Sprite
{
	friend class Game;
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
	
private:
	float _Vertical_velo=360;//��ֻ��һ�����ȣ��ٶȾ�����ʽ�涨���ƶ�����Խ�����ٶ�Խˮƽ�������ᳬ��60�Ƚ�
	float speed = 500;       //speedʹ���ٶȴ�С���ȣ���ϵ���1.0�İ���שǽ����ʹ��һֱ���ִ��ٶ�
	int _belongtoPlate;      //˫�˶Կ�
};


#endif

