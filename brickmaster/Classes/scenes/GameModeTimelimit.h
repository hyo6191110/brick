#ifndef __GAME_TIMELIMIT_H__
#define __GAME_TIMELIMIT_H__
#include "cocos2d.h"
#include "Game.h"

class GameModeTimelimit :public Game
{
protected:
	int _time;
	int _totaltime;
	int _generatetime = 10;
	cocos2d::Label* ttime;
	cocos2d::Label* _showtime;
	std::vector<cocos2d::Vec2> V_destroyedbricks;
public:
	virtual void initMode() override;
	virtual void updateMode() override;
	virtual void createHUDMode() override;
	virtual void recordBrick(Brick* brick) override;
	virtual void recoverTime() override;
	virtual void writeScoreToUserData() override;
	static cocos2d::Scene* createSceneTimelimit( int level,int time);
	void updateTime(float delta);
	void generateBricks();
	CREATE_FUNC(GameModeTimelimit);

	virtual void createHUDforWeb() ;
};

#endif


