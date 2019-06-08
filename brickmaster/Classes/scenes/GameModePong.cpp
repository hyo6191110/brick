#include"GameModePong.h"
using namespace cocos2d;

Scene*  GameModePong::createScenePong(int life, int level, int difficulty)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameModePong::create();
	layer->_life = life;
	layer->_level = level;
	layer->_player1->setLife(life);
	layer->_player_opposite->setLife(life);
	layer->_player_opposite->setAIDifficulty(difficulty);
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

	if (!V_balls.empty())
		if (!isRoundStarted&&_ball_side== _player_opposite)
		{
			if (V_balls.back()->getPositionX() != _player_opposite->getPositionX())
			{
				V_balls.back()->setPositionX(_player_opposite->getPositionX());
			}
		}

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
			_player_opposite->stopMoving();
			gameOver(true,false);
		}	
		else if (_player_opposite->getLife() == 0)
		{
			this->unschedule(schedule_selector(GameModePong::updateTime));
			this->unschedule(schedule_selector(GameModePong::oppositeAI));
			_player_opposite->stopMoving();
			gameOver(true,true);
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
		brick = Brick::create("brick_addPoint.png", 1, 2);
	}
	else if (seed < 7)
	{
		brick = Brick::create("brick_1.png", 1, 1);
	}
	else
	{
		brick = Brick::create("brick_52.png", 1, 1, false, true);
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
	{
		return;
	}
	if (_ball_side == _player_opposite && !isRoundStarted)
	if(!V_balls.empty())
	{
		if (_shoot_waittime == 0)
		{
			Vec2 velocity(random() % 300 - random() % 300, -500);
			velocity.normalize();
			velocity *= V_balls.back()->getSpeed();
			V_balls.back()->getPhysicsBody()->setDynamic(true);
			V_balls.back()->getPhysicsBody()->setVelocity(velocity);
			isRoundStarted = true;
			_shoot_waittime = 24;
			return;
		}
		else
			_shoot_waittime--;
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
		if (destinition < _gamescreen.getMinX())
		{
			float x1 = pos.x - _gamescreen.getMinX();
			float y1 = x1 * velo.y / (-velo.x);
			float y2 = _gamescreenHeight - pos.y - y1;
			destinition = _gamescreen.getMinX() + y2 * x1 / y1;
		}
		if (destinition > _gamescreen.getMaxX())
		{
			float x1 = -pos.x + _gamescreen.getMaxX();
			float y1 = x1 * velo.y / (velo.x);
			float y2 = _gamescreenHeight - pos.y - y1;
			destinition = _gamescreen.getMaxX() - y2 * x1 / y1;
		}
		if (destinition < _gamescreen.getMinX() + _player_opposite->getContentSize().width / 2 * _player_opposite->getScaleX())
			destinition = _gamescreen.getMinX() + _player_opposite->getContentSize().width / 2 * _player_opposite->getScaleX();
		if (destinition > _gamescreen.getMaxX() - _player_opposite->getContentSize().width / 2 * _player_opposite->getScaleX())
			destinition = _gamescreen.getMaxX() - _player_opposite->getContentSize().width / 2 * _player_opposite->getScaleX();

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
void GameModePong::writeScoreToUserData()
{
	vector<pong_data> v_pong;
	int finalscore = _score ;
	for (int i = 0; i < 10; i++)
	{
		string score = StringUtils::format("pong_score_%d", i);
		string mygoal = StringUtils::format("pong_mygoal_%d", i);
		string opgoal = StringUtils::format("pong_opgoal_%d", i);
		string level = StringUtils::format("pong_level_%d", i);
		int s;
		if ((s = UserDefault::getInstance()->getIntegerForKey(score.c_str(), -1)) == -1)
			break;
		else
		{
			pong_data temp;
			temp.score = s;
			temp.my_goal = UserDefault::getInstance()->getIntegerForKey(mygoal.c_str(), -1);
			temp.op_goal = UserDefault::getInstance()->getIntegerForKey(opgoal.c_str(), -1);
			temp.level = UserDefault::getInstance()->getIntegerForKey(level.c_str(), -1);
			v_pong.push_back(temp);
		}
	}
	v_pong.push_back(pong_data(finalscore, _life - _player_opposite->getLife(), _life-_player1->getLife(),  _level));
	sort(v_pong.begin(), v_pong.end());
	for (size_t i = 0; i < (v_pong.size() < 10 ? v_pong.size() : 10); i++)
	{
		string score = StringUtils::format("pong_score_%d", i);
		string mygoal = StringUtils::format("pong_mygoal_%d", i);
		string opgoal = StringUtils::format("pong_opgoal_%d", i);
		string level = StringUtils::format("pong_level_%d", i);
		UserDefault::getInstance()->setIntegerForKey(score.c_str(), v_pong.at(i).score);
		UserDefault::getInstance()->setIntegerForKey(mygoal.c_str(), v_pong.at(i).my_goal);
		UserDefault::getInstance()->setIntegerForKey(opgoal.c_str(), v_pong.at(i).op_goal);
		UserDefault::getInstance()->setIntegerForKey(level.c_str(), v_pong.at(i).level);
	}

	int time = UserDefault::getInstance()->getIntegerForKey("pong_played", 0);
	int average = UserDefault::getInstance()->getIntegerForKey("pong_average", 0);
	int win= UserDefault::getInstance()->getIntegerForKey("pong_win", 0);
	int maxscore = UserDefault::getInstance()->getIntegerForKey("pong_maxscore", 0);

	average = (average*time + finalscore) / (time + 1);
	time++;
	maxscore = maxscore > _score ? maxscore : _score;
	if (_player1->getLife() > _player_opposite->getLife())
		win++;
	UserDefault::getInstance()->setIntegerForKey("pong_played", time);
	UserDefault::getInstance()->setIntegerForKey("pong_win", win);
	UserDefault::getInstance()->setIntegerForKey("pong_average", average);
	UserDefault::getInstance()->setIntegerForKey("pong_maxscore", maxscore);
}