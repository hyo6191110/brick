#ifndef __GAME_PONG_H__
#define __GAME_PONG_H__
#include "cocos2d.h"
#include "Game.h"

class GameModePong :public Game
{
protected:
	Plate* _player_opposite;
	Deadzone* _deadzone_opposite;
	cocos2d::Label* _showlife_opposite;
public:
	virtual void initMode() override;
	virtual void updateMode() override;
	virtual void createHUDMode() override;
	static cocos2d::Scene* createScenePong (int life, int level);
	CREATE_FUNC(GameModePong);
};

#endif


