#ifndef __GAME_TIMELIMIT_H__
#define __GAME_TIMELIMIT_H__
#include "cocos2d.h"
#include "Game.h"

class GameModeTimelimit :public Game
{
protected:
	
public:
	virtual void initMode() override;
	virtual void updateMode() override;
	virtual void createHUDMode() override;
	static cocos2d::Scene* createSceneTimelimit( int life, int level);
	CREATE_FUNC(GameModeTimelimit);
};

#endif


