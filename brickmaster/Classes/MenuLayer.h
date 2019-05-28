#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuLayer :public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuLayer);
	static cocos2d::Scene* createScene();
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void SwitchtoSingle(cocos2d::Ref* pSender);
	void SwitchtoMulti(cocos2d::Ref* pSender);
	void PoptoSettings(cocos2d::Ref* pSender);
	void PoptoUser(cocos2d::Ref* pSender);
	void PoptoStore(cocos2d::Ref* pSender);
};
#endif
