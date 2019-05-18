#ifndef __MULTI_SCENE_H__
#define __MULTI_SCENE_H__

#include "cocos2d.h"

class MultiGame :public cocos2d::Layer
{
public:

	CREATE_FUNC(MultiGame);
	static cocos2d::Scene* createScene();
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);

};
#endif

