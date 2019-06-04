#ifndef __SINGLE_SCENE_H__
#define __SINGLE_SCENE_H__

#include "cocos2d.h"
class SingleGame :public cocos2d::Layer
{
public:

	CREATE_FUNC(SingleGame);
	static cocos2d::Scene* createScene();
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void SwitchToFreeSelete(cocos2d::Ref* pSender);
	void SwitchToDesigner(cocos2d::Ref* pSender);
	void SwitchToSelector(cocos2d::Ref* pSender);
	
};
#endif
