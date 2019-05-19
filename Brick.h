#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
class Brick :public cocos2d::Sprite
{
	friend class Game;
public:
	static Brick* create(const std::string& filename, int hp = 1, int s = 100, bool unbreak = false, bool item = false)
	{
		auto sprite = new Brick();

		if (sprite&&sprite->initWithFile(filename))
		{
			sprite->_hp = hp;
			sprite->_score = s;
			sprite->unbreakable = unbreak;
			sprite->dropitem = item;

			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void initWithData(float x, float y);
	bool isunbreakable() { return unbreakable; }
	bool isdropitem() { return dropitem; }
	int gethp() { return _hp; }
	int getscore() { return _score; }
	int gettype() { return _type; }
	void settype(int t) { _type = t; }
	void damaged();
private:
	int _type;
	int _hp;
	int _score;
	bool unbreakable;
	bool dropitem;
};

#endif

