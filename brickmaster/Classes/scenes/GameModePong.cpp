#include"GameModePong.h"
using namespace cocos2d;

Scene*  GameModePong::createScenePong(int life, int level)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameModePong::create();
	layer->_life = life;
	layer->_level = level;
	layer->_player1->setLife(life);
	layer->_player_opposite->setLife(life);
	layer->initAfter();
	scene->addChild(layer);
	return scene;
}

void GameModePong::initMode()
{
	createPlayerSide(2, _player_opposite, _deadzone_opposite);
	_ball_side = _player1;
	this->schedule(schedule_selector(GameModePong::updateTime), 1.0f);
	this->schedule(schedule_selector(GameModePong::oppositeAI), 1.0f/24);
}
void GameModePong::updateMode()
{
	collisionDetection(_player_opposite);
	if (balltodestroy)
	{
		auto player = balltodestroy->getPlateBelongto();
		destroyball(balltodestroy);
		balltodestroy = nullptr;
		player->damaged();
		_showlife->setString(StringUtils::format("%d", _player1->getLife()));
		_showlife_opposite->setString(StringUtils::format("%d", _player_opposite->getLife()));

		if (_player1->getLife() == 0)
		{		
			this->unschedule(schedule_selector(GameModePong::updateTime));
			this->unschedule(schedule_selector(GameModePong::oppositeAI));
			gameOver(false);
		}	
		else if (_player_opposite->getLife() == 0)
		{
			this->unschedule(schedule_selector(GameModePong::updateTime));
			this->unschedule(schedule_selector(GameModePong::oppositeAI));
			gameOver(true);
		}
		else if (V_balls.empty())
		{
			_point_multiple = 10;
			_show_point_multi->setString("10");
			if (_ball_side == _player1)
				_ball_side = _player_opposite;
			else
				_ball_side = _player1;
			createballforPlate(_ball_side);
			isRoundStarted = false;
		}
	}
}
void GameModePong::createHUDMode()
{
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	auto tlife = Label::createWithTTF("Oppo. Life:", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	tlife->setColor(Color3B::BLACK);
	auto str = StringUtils::format("%d", _life);
	_showlife_opposite = Label::createWithTTF(str, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tlife->setPosition(x, y - 450);
	_showlife_opposite->setPosition(x, y - 500);
	this->addChild(tlife);
	this->addChild(_showlife_opposite);
}
void GameModePong::recordBrick(Brick* brick)
{
	Vec2 vec(brick->getPositionX(), brick->getPositionY());
	V_destroyedbricks.push_back(vec);
}
void GameModePong::updateTime(float delta)
{
	if (_generatetime < 10)
		_generatetime++;
	if (_generatetime >= 10 || brickcount < allbrickcount / 2)
	{
		if (!V_destroyedbricks.empty())
		{
			generateBricks();
			_generatetime = 0;
		}

	}
}
void GameModePong::generateBricks()
{
	if (V_destroyedbricks.empty())
		return;

	auto it = V_destroyedbricks.begin();
	float x = it->x;
	float y = it->y;
	int seed = random() % 10;
	Brick* brick;
	if (seed < 2)
	{
		brick = Brick::create("game/brick_addPoint.png", 1, 2);
	}
	else if (seed < 7)
	{
		brick = Brick::create("game/brick_1.png", 1, 1);
	}
	else
	{
		brick = Brick::create("game/brick_52.png", 1, 1, false, true);
	}
	LOG_INFO("Bonus Brick!");
	brickcount++;
	brick->initWithData(x, y);
	this->addChild(brick);
	V_destroyedbricks.erase(it);
}
void GameModePong::oppositeAI(float delta)
{
	if (isGameOver)
		return;

	if (_ball_side == _player_opposite && !isRoundStarted)
	if(!V_balls.empty())
	{
		Vec2 velocity(random()%300- random() % 300, -500);
		velocity.normalize();
		velocity *= V_balls.back()->getSpeed();
		V_balls.back()->getPhysicsBody()->setDynamic(true);
		V_balls.back()->getPhysicsBody()->setVelocity(velocity);
		isRoundStarted = true;
		return;
	}
	bool flag = false;
	float op_pos = _player_opposite->getPositionX();
	for (auto ball : V_balls)
	{
		Vec2 pos = ball->getPosition();
		Vec2 velo = ball->getPhysicsBody()->getVelocity();
		if (velo.y < 0)
			continue;

		float destinition = pos.x + velo.x / velo.y*(_gamescreenHeight - pos.y-50);
		flag = true;
		if (op_pos > destinition+20)
		{
			_player_opposite->stopRight();
			_player_opposite->moveLeft();
		}
		else if ((op_pos < destinition-20))
		{
			_player_opposite->stopLeft();
			_player_opposite->moveRight();
		}
		else
		{
			_player_opposite->stopMoving();
		}

		if(flag==true)
		   break;
	}
	if (flag == false)
	{
		if (op_pos > _centerX + 20)
		{
			_player_opposite->stopRight();
			_player_opposite->moveLeft();
		}
		else if ((op_pos < _centerX - 20))
		{
			_player_opposite->stopLeft();
			_player_opposite->moveRight();
		}
		else
		{
			_player_opposite->stopMoving();
		}
	}
}