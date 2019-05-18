#ifndef __JSONPARSE_H__
#define __JSONPARSE_H__

#include "cocos2d.h"
#include"json/reader.h"
#include"json/rapidjson.h"
#include"json/document.h"

class BrickData :public cocos2d::Ref
{
protected:
	int _type;
	int _x;
	int _y;
public:
	static BrickData* create()
	{
		BrickData* s = new BrickData();
		if (s)
		{
			s->autorelease();
			return s;
		}
		CC_SAFE_DELETE(s);
		return nullptr;
	}
	void setType(int t) { _type = t; }
	int getType() { return _type; }
	void setX(int t) { _x = t; }
	int getX() { return _x; }
	void setY(int t) { _y = t; }
	int getY() { return _y; }
};

bool getAllBrickWithFile(const std::string& fileName, std::vector<BrickData*>& vec);
#endif


