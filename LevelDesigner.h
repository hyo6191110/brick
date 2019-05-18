#ifndef __DESIGNER_H__
#define __DESIGNER_H__

#include "cocos2d.h"

#include"json/writer.h"
#include"json/rapidjson.h"
#include"json/document.h"

class LevelDesigner :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(LevelDesigner);
	virtual bool init();

	void Closethis(cocos2d::Ref* pSender);
private:
};
#endif


