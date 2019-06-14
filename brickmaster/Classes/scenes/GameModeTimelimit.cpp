#include"GameModeTimelimit.h"
using namespace cocos2d;

Scene*  GameModeTimelimit::createSceneTimelimit(int level,int time)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameModeTimelimit::create();
	layer->_life = -1;
	layer->_level = level;
	layer->_time = time;
	layer->_totaltime = time;
	layer->islife = false;
	layer->initAfter();
	scene->addChild(layer);
	return scene;
}

void GameModeTimelimit::initMode()
{	
	this->schedule(schedule_selector(GameModeTimelimit::updateTime), 1.0f);
}
void GameModeTimelimit::updateMode()
{
	if (balltodestroy)
	{
		destroyball(balltodestroy);
		if (_time > 10)
		{
			_time -= 5;
			LOG_INFO("Punishment:-5 s");
		}
		if (V_balls.empty())
		{
			_point_multiple = 10;
			_show_point_multi->setString("10");
			createballforPlate(_player1);
			isRoundStarted = false;				
		}
		balltodestroy = nullptr;
	}
	if (_time <= 0&&isGameOver==false)
	{
		gameOver(false);
		this->unschedule(schedule_selector(GameModeTimelimit::updateTime));
	}
}
void GameModeTimelimit::createHUDMode()
{
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	ttime = Label::createWithTTF("Time:", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	ttime->setColor(Color3B::BLACK);
	auto str = StringUtils::format("%d", _time);
	_showtime = Label::createWithTTF(str, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	ttime->setPosition(x, y - 450);
	_showtime->setPosition(x, y - 500);
	this->addChild(ttime);
	this->addChild(_showtime);

	_showlife->setString("Unlimited");

	createHUDforWeb();
}
void GameModeTimelimit::updateTime(float delta)
{
	if (_time > 0)
	{
		_time--;
		_showtime->setString(StringUtils::format("%d", _time));
		if (_time < 10)
			_showtime->setColor(Color3B::RED);
		else
			_showtime->setColor(Color3B::WHITE);
	}
	if (_generatetime < 10)
		_generatetime++;
	if (_generatetime >= 10||brickcount<allbrickcount/2)
	{
		if (!V_destroyedbricks.empty())
		{
			generateBricks();
			_generatetime = 0;
		}
		
	}

}

void GameModeTimelimit::recordBrick(Brick* brick)
{
	if (brick->gettype() >= 1000)
	{
		int timeplus = brick->gettype() - 999;
		_time += timeplus;
		LOG_INFO(StringUtils::format("Time+%d s", timeplus));
	}
	Vec2 vec(brick->getPositionX(), brick->getPositionY());
	V_destroyedbricks.push_back(vec);
}

void GameModeTimelimit::generateBricks()
{
	if (V_destroyedbricks.empty())
		return;
	auto it = V_destroyedbricks.begin();
	float x = it->x;
	float y = it->y;
	int seed = random() % 10;
	Brick* brick;
	if (seed < 3)
	{
		brick = Brick::create("brick_addPoint.png", 1, 2);
	}
	else if (seed < 6)
	{
		brick = Brick::create("brick_addTime.png", 1, 0);
		brick->settype(1000 + random() % 5);
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

void GameModeTimelimit::recoverTime()
{
	_time += 10;
	_showtime->setString(StringUtils::format("%d", _time));
	LOG_INFO("Time +10s ");

}
void GameModeTimelimit::writeScoreToUserData()
{
	vector<timelimit_data> v_timelimit;
	int finalscore = _score*10 / _totaltime;
	for (int i = 0; i < 10; i++)
	{
		string score = StringUtils::format("timelimit_score_%d", i);
		string time = StringUtils::format("timelimit_time_%d", i);
		string level = StringUtils::format("timelimit_level_%d", i);
		int s;
		if ((s = UserDefault::getInstance()->getIntegerForKey(score.c_str(), -1)) == -1)
			break;
		else
		{
			timelimit_data temp;
			temp.score = s;
			temp.time = UserDefault::getInstance()->getIntegerForKey(time.c_str(), -1);
			temp.level = UserDefault::getInstance()->getIntegerForKey(level.c_str(), -1);
			v_timelimit.push_back(temp);
		}
	}
	v_timelimit.push_back(timelimit_data(finalscore, _totaltime, _level));
	sort(v_timelimit.begin(), v_timelimit.end());
	for (size_t i = 0; i < (v_timelimit.size() < 10 ? v_timelimit.size() : 10); i++)
	{
		string score = StringUtils::format("timelimit_score_%d", i);
		string time = StringUtils::format("timelimit_time_%d", i);
		string level = StringUtils::format("timelimit_level_%d", i);
		UserDefault::getInstance()->setIntegerForKey(score.c_str(), v_timelimit.at(i).score);
		UserDefault::getInstance()->setIntegerForKey(time.c_str(), v_timelimit.at(i).time);
		UserDefault::getInstance()->setIntegerForKey(level.c_str(), v_timelimit.at(i).level);
	}

	int time = UserDefault::getInstance()->getIntegerForKey("timelimit_played", 0);
	int average = UserDefault::getInstance()->getIntegerForKey("timelimit_average", 0);
	int maxscore = UserDefault::getInstance()->getIntegerForKey("timelimit_maxscore", 0);

	average = (average*time + finalscore) / (time + 1);
	time++;
	maxscore = maxscore > _score ? maxscore : _score;
	UserDefault::getInstance()->setIntegerForKey("timelimit_played", time);
	UserDefault::getInstance()->setIntegerForKey("timelimit_average", average);
	UserDefault::getInstance()->setIntegerForKey("timelimit_maxscore", maxscore);
}

void GameModeTimelimit::createHUDforWeb()
{

}