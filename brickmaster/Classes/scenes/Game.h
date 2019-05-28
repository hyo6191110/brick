#ifndef __GAME_H__
#define __GAME_H__
#include "cocos2d.h"
#include "Level.h"

#include "models/Plate.h"
#include "models/Ball.h"
#include "models/Brick.h"
#include "models/BrickItem.h"

#include"utils/jsonparse.h"

//Ϊ������ϷԪ�ظ�һ�����еı�ǩ����ʶ��
const int BALL_TAG = 1;
const int BRICK_TAG = 2;
const int PLATE_TAG = 3;
const int DEADZONE_TAG = 4;
const int ITEM_TAG = 5;
const int WALL_TAG = 6;

class Game :public cocos2d::Layer
{
protected:
	//��ֵ���ݼ���ʾ
	int _level;
	int _life;
	int _score = 0;
	cocos2d::Label* _showlife;
	cocos2d::Label* _showscore;
	cocos2d::Label* _showlevel;
	//λ������
	float _gamescreenWidth;
	float _gamescreenHeight;
	cocos2d::Rect _gamescreen;
	float _centerX;
	float _centerY;
	//�߼��ж�
	Ball* balltodestroy = nullptr;
	bool isGameOver = false;
	bool isRoundStarted = false;
	//�̡�ש�顢���Ԫ�صĹ���
	Plate* _player1;
	Deadzone*  _deadzone1;
	std::vector<Ball*> V_balls;
	std::vector<BrickItem*> V_items;
	int brickcount = 0;
public:
	virtual void initMode();
	virtual void updateMode();
	virtual void createHUDMode();
	//��������
	static cocos2d::Scene* createScene(int life,int level);
	CREATE_FUNC(Game);
	virtual bool init() override;
	void initAfter();
	//����
	void update(float delta);
	void gameOver(bool isWin);
	void updateballspeed();
	void collisionDetection(Plate* player);
	//��װ������ϷԪ��
	void createHUD();
	void createPlayerSide(int side,Plate*& player,Deadzone*& deadzone);
	void createballwithBall(Ball* origin);
	void createballforPlate(Plate* owner);
	void destroyball(Ball* ball);
	void createBricksFromFile(const std::string& fileName);
	void createBrickItem(float x, float y, int side);
	//�¼��ص�����
	void Closethis(cocos2d::Ref* pSender);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	void onContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
};

#endif

