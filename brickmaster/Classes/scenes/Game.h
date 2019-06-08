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

//Ϊ������ϷԪ�ظ�һ�����еı�ǩ����ʶ��
const int BALL_TAG = 1;
const int BRICK_TAG = 2;
const int PLATE_TAG = 3;
const int DEADZONE_TAG = 4;
const int ITEM_TAG = 5;
const int WALL_TAG = 6;

class Game :public cocos2d::Layer
{
public:
	//init��һ����
	virtual void initMode();
	//update��һ����
	virtual void updateMode();
	//createHUD��һ����
	virtual void createHUDMode();
	//��������
	static cocos2d::Scene* createScene(int life,int level);
	//����ͼ��
	CREATE_FUNC(Game);
	//��ʼ������
	virtual bool init() override;
	//��layer�����������ʼ��
	void initAfter();
	//����
	void update(float delta);
	//��Ϸ����ʱ����
	void gameOver(bool isWinMode,bool isWin = true);
	//update��һ����
	void updateballspeed();
	//update��һ����
	void collisionDetection(Plate* player);
	//����������ʾ����
	void createHUD();
	//�����û���
	void createPlayerSide(int side,Plate*& player,Deadzone*& deadzone);
	//������������
	void createballwithBall(Ball* origin);
	void createballforPlate(Plate* owner);
	void destroyball(Ball* ball);
	//���������ļ�����ש��
	void createBricksFromFile(const std::string& fileName);
	//��������
	void createBrickItem(float x, float y, int side);
	//��¼�����ٵ�ש��λ��
	virtual void recordBrick(Brick* brick);
	//�رճ���
	void Closethis(cocos2d::Ref* pSender);
	//���̼���
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
	//������ײ����
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	//������Ϸģʽ��ͬ���ظ����ߵĲ�ͬ�жϷ�ʽ
	virtual void recoverTime();
	//���÷���д���û��ļ�
	virtual void writeScoreToUserData();
protected:
	//��ֵ���ݼ���ʾ
	int _level=1;
	int _life;
	int _score = 0;
	cocos2d::Label* _showlife;
	cocos2d::Label* _showscore;
	cocos2d::Label* _showlevel;
	bool islife = true;
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

