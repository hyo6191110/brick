#ifndef __MULTI_SCENE_H__
#define __MULTI_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;


class MultiGame :public Layer
{
public:
	CREATE_FUNC(MultiGame);
	static cocos2d::Scene* createScene();
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void sendMsg(Ref* pSender);
	void startGame(Ref* pSender);
	void setInfo(const std::string& str) { _show_info->setString(str); }

private:
	Label* _show_info;

};
#endif

