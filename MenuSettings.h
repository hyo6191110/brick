#ifndef __SETTINGS_LAYER_H__
#define __SETTINGS_LAYER_H__

#include "cocos2d.h"

class MenuSettings :public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuSettings);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);

};
#endif
