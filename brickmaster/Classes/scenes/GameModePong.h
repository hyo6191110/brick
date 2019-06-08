#ifndef __GAME_PONG_H__
#define __GAME_PONG_H__
#include "cocos2d.h"
#include "Game.h"

class GameModePong :public Game
{
public:
	virtual void initMode() override;
	virtual void updateMode() override;
	virtual void createHUDMode() override;
	virtual void recordBrick(Brick* brick) override;
	virtual void writeScoreToUserData() override;
	static cocos2d::Scene* createScenePong (int life, int level,int difficulty);
	void updateTime(float delta);
	void generateBricks();
	void oppositeAI(float delta);
	CREATE_FUNC(GameModePong);
protected:
	
	Plate* _player_opposite;
	Deadzone* _deadzone_opposite;
	cocos2d::Label* _showlife_opposite;
	int _generatetime = 10;
	int _shoot_waittime = 24;
	std::vector<cocos2d::Vec2> V_destroyedbricks;
};

#endif


