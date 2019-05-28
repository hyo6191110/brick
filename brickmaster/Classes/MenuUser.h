#ifndef __USER_LAYER_H__
#define __USER_LAYER_H__

#include "cocos2d.h"

class MenuUser :public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuUser);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);

};
#endif
