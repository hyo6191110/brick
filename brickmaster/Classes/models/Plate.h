#ifndef __PLATE_H__
#define __PLATE_H__

#include "cocos2d.h"
class Plate :public cocos2d::Sprite
{
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
	float getLeftPosition() { return getPositionX() - getContentSize().width / 2; }
	float getRightPosition() { return getPositionX() + getContentSize().width / 2; }
	float getUpPosition() { return getPositionY() + getContentSize().height / 2; }
	float getDownPosition() { return getPositionY() - getContentSize().height / 2; }

	bool isMovingLeft() { return _movingleft; }
	void  setMovingLeft(bool b) {_movingleft=b; }
	bool isMovingRight() { return _movingright; }
	void  setMovingRight(bool b) { _movingright = b; }
	bool isMoving() { return _movingleft || _movingright; }
	void moveLeft();
	void stopLeft();
	void moveRight();
	void stopRight();
	void stopMoving();

	float getVelocity() { return _velocity; }
	void  setVelocityMultiple(float multiply) { _velocity *= multiply; }
	float getStartPosition() { return _startPosition; }
	void  setStartPosition(float pos) { _startPosition = pos; }
	int   getLife() { return _life; }
	void  setLife(int hp) { _life = hp; }
	void  recover() { _life++; }
	void  damaged() { _life--; }
	void  speedUp() { if(_velocity<900)_velocity += 100; }
	void  speedDown() { if (_velocity > 300)_velocity -= 100; }

	bool checkLengthIntergrity() { return _length > 0.4&&_length < 1.8; }
	void expand();
	void shrink();
private:
	float _velocity=500;
	float _startPosition=0;
	bool  _movingleft=false;
	bool  _movingright = false;
	int   _life;
	float _length = 1.0;
};

class Deadzone :public cocos2d::Sprite
{
public:
	static Deadzone* create(const std::string& filename)
	{
		auto sprite = new Deadzone();
		if (sprite&&sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x, float y);
	Plate* getOwner() { return _owner; }
	void   setOwner(Plate* owner) { _owner = owner; }
private:
	Plate* _owner;
};
#endif

