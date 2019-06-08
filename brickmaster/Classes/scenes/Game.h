#ifndef __GAME_H__
#define __GAME_H__
#include "cocos2d.h"
#include "Level.h"
#include "MenuUser.h"
#include "models/Plate.h"
#include "models/Ball.h"
#include "models/Brick.h"
#include "models/BrickItem.h"

#include"utils/jsonparse.h"

//为各种游戏元素给一个特有的标签用于识别
const int BALL_TAG = 1;
const int BRICK_TAG = 2;
const int PLATE_TAG = 3;
const int DEADZONE_TAG = 4;
const int ITEM_TAG = 5;
const int WALL_TAG = 6;

class Game :public cocos2d::Layer
{
public:
	//init的一部分
	virtual void initMode();
	//update的一部分
	virtual void updateMode();
	//createHUD的一部分
	virtual void createHUDMode();
	//场景创建
	static cocos2d::Scene* createScene(int life,int level);
	//创建图层
	CREATE_FUNC(Game);
	//初始化场景
	virtual bool init() override;
	//在layer创建出来后初始化
	void initAfter();
	//更新
	void update(float delta);
	//游戏结束时调用
	void gameOver(bool isWinMode,bool isWin = true);
	//update的一部分
	void updateballspeed();
	//update的一部分
	void collisionDetection(Plate* player);
	//创建数据显示界面
	void createHUD();
	//创建用户区
	void createPlayerSide(int side,Plate*& player,Deadzone*& deadzone);
	//创建、销毁球
	void createballwithBall(Ball* origin);
	void createballforPlate(Plate* owner);
	void destroyball(Ball* ball);
	//根据配置文件加载砖块
	void createBricksFromFile(const std::string& fileName);
	//创建道具
	void createBrickItem(float x, float y, int side);
	//记录被销毁的砖块位置
	virtual void recordBrick(Brick* brick);
	//关闭场景
	void Closethis(cocos2d::Ref* pSender);
	//键盘监听
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	//物理碰撞监听
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	//根据游戏模式不同，回复道具的不同判断方式
	virtual void recoverTime();
	//将得分数写入用户文件
	virtual void writeScoreToUserData();
protected:
	//分值数据及显示
	int _level=1;
	int _life;
	int _score = 0;
	cocos2d::Label* _showlife;
	cocos2d::Label* _showscore;
	cocos2d::Label* _showlevel;
	bool islife = true;
	//位置数据
	float _gamescreenWidth;
	float _gamescreenHeight;
	cocos2d::Rect _gamescreen;
	float _centerX;
	float _centerY;
	//逻辑判断
	Ball* balltodestroy = nullptr;
	bool isGameOver = false;
	bool isRoundStarted = false;
	//盘、砖块、球等元素的管理
	Plate* _ball_side;
	Plate* _player1;
	Deadzone*  _deadzone1;
	std::vector<Ball*> V_balls;
	std::vector<BrickItem*> V_items;
	int brickcount = 0;
	int allbrickcount;
	int _point_multiple = 10;
	cocos2d::Label* _show_brickcount;
	cocos2d::Label* _show_point_multi;
	cocos2d::Label* _show_game_info;
public:
	void LOG_INFO(const std::string& str) { _show_game_info->setString(str); }
};

#endif

