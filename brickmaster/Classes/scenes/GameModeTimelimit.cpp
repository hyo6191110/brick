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
	if (_time <= 0)
	{
		isGameOver = true;
		for (auto it = V_items.begin(); it != V_items.end();)
		{
			this->removeChild(*it);
			it = V_items.erase(it);
		}
		for (auto it = V_balls.begin(); it != V_balls.end();)
		{
			this->removeChild(*it);
			it = V_balls.erase(it);
		}
		auto board = Sprite::create("game\\winlose_board.png");
		board->setPosition(_centerX, _centerY);
		addChild(board);
		auto show = Label::createWithTTF(StringUtils::format("Score:%d",_score), 
			"fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		show->setPosition(_centerX, _centerY);
		addChild(show);

		this->unschedule(schedule_selector(GameModeTimelimit::updateTime));
	}
}
void GameModeTimelimit::createHUDMode()
{
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	auto ttime = Label::createWithTTF("Time:", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	ttime->setColor(Color3B::BLACK);
	auto str = StringUtils::format("%d", _time);
	_showtime = Label::createWithTTF(str, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	ttime->setPosition(x, y - 450);
	_showtime->setPosition(x, y - 500);
	this->addChild(ttime);
	this->addChild(_showtime);

	_showlife->setString("Unlimited");
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