#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"



class Level :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(Level);
	virtual bool init();

	void Closethis(cocos2d::Ref* pSender);
	void addmode(cocos2d::Ref* pSender);
	void minusmode(cocos2d::Ref* pSender);
	void addlife(cocos2d::Ref* pSender);
	void minuslife(cocos2d::Ref* pSender);
	void setlevel(int level);
	void openlevel(cocos2d::Ref* pSender);

	enum GameMode {
		Classic
	};
	const int MaxLevel = 18;
	const int MaxMode = 1;
	const int MaxLife = 10;
private:
	GameMode _currentmode;
	int _currentlife;
	int _currentlevel;
	cocos2d::Label* _showmode;
	cocos2d::Label* _showlife;
	cocos2d::Label* _showlevel;
};
#endif

