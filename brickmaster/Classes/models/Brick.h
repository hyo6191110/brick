#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
#include "utils/jsonparse.h"
class Brick :public cocos2d::Sprite
{
public:
	static Brick* create(const std::string& filename, int hp = 1, int s = 1, bool unbreak = false, bool item = false)
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
	bool isbrickcount() { return brickcount; }
	void setbrickcount(bool b) { brickcount = b; }
	int gethp() { return _hp; }
	int getMaxhp() { return _max_hp; }
	int getscore() { return _score; }
	int gettype() { return _type; }
	void settype(int t) { _type = t; }
	void damaged();
private:
	int _type;
	int _hp;
	int _max_hp;
	int _score;
	bool unbreakable;
	bool dropitem;
	bool brickcount=true;
};

#endif

