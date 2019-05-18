#ifndef __STORE_LAYER_H__
#define __STORE_LAYER_H__

#include "cocos2d.h"

class MenuStore :public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuStore);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);

};
#endif

