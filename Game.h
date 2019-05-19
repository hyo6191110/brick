#ifndef __GAME_H__
#define __GAME_H__
#include "cocos2d.h"
#include "Level.h"

#include "Plate.h"
#include "Ball.h"
#include "Brick.h"
#include "BrickItem.h"

#include"jsonparse.h"

class Game :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createSceneWithLevel(Level::GameMode mode,int life,int level);
	CREATE_FUNC(Game);
	virtual bool init();
	void createClassicLevel();
	void Closethis(cocos2d::Ref* pSender);
	void update(float delta);
	void gameOver(bool isWin);
public:
	void createball(float x=0,float y=0);
	void destroyball(Ball* ball);
	void createBricksFromFile(const std::string& fileName);
public:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
protected:
	Level::GameMode _mode;
	int _level;
	int _life;
	int _score=0;
	cocos2d::Label* _showlife;
	cocos2d::Label* _showscore;
	cocos2d::Label* _showlevel;
	float _gamescreenWidth;
	float _gamescreenHeight;
	float _centerX;
	float _centerY;
protected:
	Ball* balltodestroy=nullptr;
	bool isGameOver=false;
	bool isRoundStarted=false;
protected:
	Plate* _player1;
	cocos2d::Sprite*  _deadzone1;
	std::vector<Ball*> V_balls;
	int brickcount = 0;
};

#endif

