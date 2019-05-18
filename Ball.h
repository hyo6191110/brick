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
	float _Vertical_velo=360;//这只是一个量度，速度决定公式规定板移动距离越长球速度越水平，但不会超过60度角
	float speed = 500;       //speed使球速度大小均等，配合弹性1.0的板球砖墙，可使球一直保持此速度
	int _belongtoPlate;      //双人对抗
};


#endif

