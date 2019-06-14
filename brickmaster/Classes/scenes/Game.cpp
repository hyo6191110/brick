#include "Game.h"

Scene*  Game::createScene(int life, int level)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Game::create();

	layer->_life = life;
	layer->_level = level;
	layer->_player1->setLife(life);
	layer->initAfter();

	scene->addChild(layer);
	return scene;
}

void Game::Closethis(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	this->unscheduleAllCallbacks();
    //SpriteFrameCache::getInstance()->removeSpriteFrames();
    //SpriteFrameCache::getInstance->destroyInstance();
    //this->_eventDispatcher->removeAllEventListeners();
	closeOthers();
    Director::getInstance()->popScene();
}

void Game::update(float delta)
{
	//道具与玩家碰撞检测
	collisionDetection(_player1);
	//更新球速
	updateballspeed();
	//发射前让球紧跟板移动
	if (!V_balls.empty())
	if (!isRoundStarted)
	{
		if (V_balls.back()->getPositionX() != _player1->getPositionX())
		{
			V_balls.back()->setPositionX(_player1->getPositionX());
		}
	}
	//使板不反弹
	if (_player1->isMovingLeft())
		if (_player1->getPhysicsBody()->getVelocity().x > 0)
		{
			_player1->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
	if (_player1->isMovingRight())
		if (_player1->getPhysicsBody()->getVelocity().x < 0)
		{
			_player1->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
	//针对不同模式的不同逻辑判断
	updateMode();		
}

void Game::gameOver(bool isWinMode, bool isWin)
{	
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	if (isGameOver == true)
		return;
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
	board->setLocalZOrder(5);
	addChild(board);
	if (isWinMode)
	{
		auto show = Label::createWithTTF("You win!", "fonts\\BRITANIC.ttf", 72, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		show->setPosition(_centerX, _centerY + 120);
		show->setLocalZOrder(6);
		show->setColor(Color3B::BLACK);
		addChild(show);
		if (!isWin)
		{
			PLAY_EFFECT("effect/lose.wav");
			show->setString("You lose!");
		}
		else
		{
			PLAY_EFFECT("effect/win.wav");
		}
			
	}
	else
	{
		PLAY_EFFECT("effect/win.wav");
	}
	auto shows = Label::createWithTTF(StringUtils::format("Score:%d", _score),
		"fonts\\BRITANIC.ttf", 72, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	shows->setPosition(_centerX, _centerY+20);
	shows->setColor(Color3B::BLACK);
	shows->setLocalZOrder(6);
	addChild(shows);
	
	writeScoreToUserData();

	auto showi = Label::createWithTTF(StringUtils::format("Successfully saved score!", _score),
		"fonts\\BRITANIC.ttf", 36, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	showi->setPosition(_centerX, _centerY-100);
	showi->setLocalZOrder(6);
	addChild(showi);
}

void Game::createPlayerSide(int side, Plate*& player, Deadzone*& deadzone)
{
	deadzone = Deadzone::create("game\\deadzone.png");
	deadzone->setTextureRect(Rect(0, 0, _gamescreenWidth, 20));
	if (side == 1)
		player = Plate::create(UserDefault::getInstance()->getStringForKey("plate_texture", "game/defender_0.png"));
	else
		player = Plate::create(StringUtils::format("game/defender_%d.png", UserDefault::getInstance()->getIntegerForKey("default_difficulty",2)));

	float x= _centerX, 
		  y_deadzone=deadzone->getContentSize().height/2,
		  y_player=y_deadzone+ deadzone->getContentSize().height / 2+ player->getContentSize().height / 2;
	if (side == 1)
		 ;
	else if (side == 2)
		y_deadzone = _gamescreenHeight - y_deadzone,
	    y_player = _gamescreenHeight - y_player;
	else
		return;

	deadzone->initWithData(x,y_deadzone);
	deadzone->setTag(DEADZONE_TAG);
	deadzone->setOwner(player);
	this->addChild(deadzone);
	player->initWithData(x,y_player);
	player->setTag(PLATE_TAG);
	this->addChild(player);
}
void Game::createballwithBall(Ball* origin)
{
	auto _ball = Ball::create("ball_M.png");
	float x = origin->getPositionX();
	float y = origin->getPositionY();
	_ball->initWithData(x, y);
	_ball->setTag(1);
	_ball->setPlateBelongto(origin->getPlateBelongto());
	_ball->setRadius(origin->getRadius());
	_ball->setScale(_ball->getRadius());
	this->addChild(_ball);
	V_balls.push_back(_ball);
	_ball->getPhysicsBody()->setVelocity(origin->getPhysicsBody()->getVelocity());
}

void Game::createballforPlate(Plate* owner)
{
	auto _ball = Ball::create("ball_M.png");
	float x = owner->getPositionX();
	float y;
	if(owner->getPositionY()<500)
	    y = owner->getPositionY() + this->_player1->getContentSize().height / 2 + _ball->getContentSize().height / 2;
	else
		y = owner->getPositionY() - this->_player1->getContentSize().height / 2 - _ball->getContentSize().height / 2;
	_ball->initWithData(x, y);
	_ball->setTag(1);
	_ball->getPhysicsBody()->setDynamic(false);
	_ball->setPlateBelongto(owner);
	this->addChild(_ball);
	V_balls.push_back(_ball);
}

void Game::destroyball(Ball* ball)
{
	this->removeChild(ball);
	V_balls.erase(std::find(V_balls.begin(), V_balls.end(), ball));
}

void Game::createBricksFromFile(const std::string& fileName)
{

	std::vector<BrickData*> V_brickdata;
	getAllBrickWithFile(fileName, V_brickdata);

	for (size_t i = 0; i < V_brickdata.size(); i++)
	{
		int type = V_brickdata.at(i)->getType();
		int x = V_brickdata.at(i)->getX();
		int y = V_brickdata.at(i)->getY();
		auto filename = StringUtils::format("brick_%d.png", type);
		if (type < 50)
		{
			auto brick = Brick::create(filename, type);
			brick->initWithData(x, y);
			brickcount++;
			this->addChild(brick);
		}
		else
		{
			if (type == 51)
			{
				auto brick = Brick::create(filename, 1, 0, true, false);
				brick->initWithData(x, y);
				this->addChild(brick);
			}
			else if (type == 52)
			{
				auto brick = Brick::create(filename, 1, 1, false, true);
				brick->initWithData(x, y);
				brick->setbrickcount(false);
				this->addChild(brick);
			}
		}

	}
}

void Game::createBrickItem(float x, float y, int side,bool createdbynormalbrick)
{
	float y_destination,distance;
	if (side == 1)
		y_destination = 0,distance=y;
	else if (side == 2)
		y_destination = _gamescreenHeight,distance=_gamescreenHeight-y;
	else
		return;

	int i_type = random() % BrickItem::getMaxitemtype() + 1;
	auto fileName = StringUtils::format("item_%d.png", i_type);
	auto item = BrickItem::create(fileName);
	item->initWithData(x,y);
	item->setItemType(static_cast<ItemType>(i_type - 1));
	this->addChild(item);
	V_items.push_back(item);
	item->runAction(MoveTo::create(distance / item->getDropspeed(), Vec2(x, y_destination)));
	LOG_INFO("Item created!");
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PLAY_BGM("bgm/bgm_piano.wav");
	
	auto bg = Sprite::create(UserDefault::getInstance()->getStringForKey("background_texture","background/background_1.jpg" ));
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(bg);

	//1.创建退出按钮
	auto closeItem = MenuItemImage::create(
		"ui/back.png",
		"ui/back_s.png",
		CC_CALLBACK_1(Game::Closethis, this));
	float x = origin.x + closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//2.创建显示区，划分区域
	auto info = Sprite::create("game\\status_board.png");
	info->setPosition(info->getContentSize().width / 2, visibleSize.height / 2);
	this->addChild(info);

	_gamescreenWidth = visibleSize.width-info->getContentSize().width;
	_gamescreenHeight = visibleSize.height;
	_centerX = info->getContentSize().width + _gamescreenWidth / 2;
	_centerY = _gamescreenHeight / 2;
	_gamescreen = Rect(_centerX - _gamescreenWidth / 2, _centerY - _gamescreenHeight / 2,
		_gamescreenWidth, _gamescreenHeight);
	
	//4.创建游戏边界区域
	Vec2 up_left(_centerX - _gamescreenWidth / 2, _centerY - _gamescreenHeight / 2);
	Vec2 up_right(_centerX + _gamescreenWidth / 2, _centerY - _gamescreenHeight / 2);
	Vec2 down_left(_centerX - _gamescreenWidth / 2, _centerY + _gamescreenHeight / 2);
	Vec2 down_right(_centerX + _gamescreenWidth / 2, _centerY + _gamescreenHeight / 2);
	Vec2 link_point[4] = { up_left,up_right,down_right,down_left };
	auto node = Node::create();
	auto body = PhysicsBody::createEdgePolygon(link_point, 4);
	body->getShapes().at(0)->setRestitution(1);
	body->setGroup(1);
	node->setPhysicsBody(body);
	node->setPosition(Vec2::ZERO);
	this->addChild(node);
	//5.初始化游戏逻辑，创建基本的游戏元素：
	//玩家及对应的危险区，和一个由玩家发射的球。
	isGameOver = false;
	isRoundStarted = false;
	createPlayerSide(1, _player1, _deadzone1);
	createballforPlate(_player1);
	_ball_side = _player1;
	//6.根据游戏模式给出特别化的界面创建
	this->initMode(); 
	//7.游戏的主循环、碰撞检测、按键事件检测
	this->scheduleUpdate();

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);

	return true;
}


void Game::createHUD()
{
	//显示数值数据
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	std::string level_str;
	if (_level > 10000)
		level_str = StringUtils::format("User's Level\n%d", _level-10000);
	else
		level_str = StringUtils::format("Level %d", _level);
	_showlevel = Label::createWithTTF(level_str, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	_showlevel->setPosition(x, y - 75);
	this->addChild(_showlevel);

	tscore = Label::createWithTTF("Score:", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	tscore->setColor(Color3B::BLACK);
	auto str2 = StringUtils::format("%d", _score);
	_showscore = Label::createWithTTF(str2, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tscore->setPosition(x, y - 150);
	_showscore->setPosition(x, y - 200);
	this->addChild(tscore);
	this->addChild(_showscore);

	tlife = Label::createWithTTF("Life:", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	tlife->setColor(Color3B::BLACK);
	auto str = StringUtils::format("%d", _life);
	_showlife = Label::createWithTTF(str, "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tlife->setPosition(x, y - 300);
	_showlife->setPosition(x, y - 350);
	this->addChild(tlife);
	this->addChild(_showlife);

	auto news = Sprite::create("game/game_info.png");
	news->setPosition(x, y - 575);
	addChild(news);
	_show_game_info = Label::createWithTTF("Nothing special", "fonts\\BRITANIC.ttf", 24, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_show_game_info->setPosition(x, y - 575);
	_show_game_info->setColor(Color3B::BLACK);
	addChild(_show_game_info);

	auto ball = Sprite::create("game/ball_score.png");
	_show_point_multi = Label::createWithTTF(StringUtils::format("%d", _point_multiple),
		"fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	ball->setPosition(x, y - 680);
	_show_point_multi->setPosition(x, y - 680);
	_show_point_multi->setColor(Color3B::BLACK);
	addChild(ball);
	addChild(_show_point_multi);

	auto brick = Sprite::create("game/brick_count.png");
	_show_brickcount = Label::createWithTTF(StringUtils::format("%d/%d", brickcount,allbrickcount),
		"fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	brick->setPosition(x, y - 800);
	_show_brickcount->setPosition(x, y - 800);
	_show_brickcount->setColor(Color3B::BLACK);
	addChild(brick);
	addChild(_show_brickcount);

	createHUDMode();
}
void Game::initAfter()
{
	std::string LevelFile;
	if(_level>10000)
		LevelFile = FileUtils::getInstance()->getWritablePath() + StringUtils::format("user_level_%d.json", _level-10000);
	else
	    LevelFile = StringUtils::format("levels/level_%d.json", _level);
	createBricksFromFile(LevelFile);
	allbrickcount = brickcount;

	createHUD();
}

void Game::updateMode()
{
	if (balltodestroy)
	{
		auto player = balltodestroy->getPlateBelongto();
		destroyball(balltodestroy);
		if (V_balls.empty())
		{
			_point_multiple = 10;
			_show_point_multi->setString("10");
			player->damaged();
			_showlife->setString(StringUtils::format("%d", _player1->getLife()));
			if (_player1->getLife() == 0)
			{
				gameOver(true,false);
			}
			else
			{
				createballforPlate(_player1);
				isRoundStarted = false;
			}

		}
		balltodestroy = nullptr;
	}
	if (brickcount == 0 && isRoundStarted == true)
	{
		for (auto dball : V_balls)
		{
			destroyball(dball);
		}
		gameOver(true,true);
	}
}
void Game::createHUDMode()
{

}


void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode code, Event* event)
{

	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_A:
	{
		_player1->moveLeft();
		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		_player1->moveRight();
		break;
	}
	case EventKeyboard::KeyCode::KEY_SPACE:
	{
		if (!isRoundStarted)
		if(_ball_side==_player1)
		{
			V_balls.back()->shootBall(_player1);
			isRoundStarted = true;
		}
		break;
	}
	default:
		break;
	}
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, Event* event)
{
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_A:
	{
		_player1->stopLeft();
		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		_player1->stopRight();
		break;
	}
	default:
		break;
	}
}

bool Game::onContactBegin(PhysicsContact& contact)
{
	auto SA = contact.getShapeA()->getBody()->getNode();
	auto SB = contact.getShapeB()->getBody()->getNode();
	if (SA&&SB)
	{
		
		if (SB->getTag() == BRICK_TAG && SA->getTag() == BALL_TAG)
		{
			auto ball= dynamic_cast<Ball*>(SA);
			auto brick = dynamic_cast<Brick*>(SB);
			
			if (!brick->isunbreakable())
			{
				brick->damaged();
				if (brick->gethp() == 0)
				{
					_score += brick->getscore()*_point_multiple;
					_point_multiple += 2;
					_show_point_multi->setString(StringUtils::format("%d", _point_multiple));
					_showscore->setString(StringUtils::format("%d", _score));

					if (brick->isdropitem())
					{
						int side = 1;
						if (ball->getPlateBelongto()->getPositionY() > 500)
							side = 2;
						createBrickItem(brick->getPositionX(), brick->getPositionY(), side);
					}
					else
					{
						if (normal_brick_dropitem)
						{
							int seed = random() % 100;
							if (seed >= 90)
							{
								int side = 1;
								if (ball->getPlateBelongto()->getPositionY() > 500)
									side = 2;
								createBrickItem(brick->getPositionX(), brick->getPositionY(), side,true);
							}
						}
					}
		
					if (brick->isbrickcount())
					{
						brickcount--;
						_show_brickcount->setString(StringUtils::format("%d/%d", brickcount, allbrickcount));
					}	

					recordBrick(brick);

					this->removeChild(brick);
					LOG_INFO("Brick destoyed!");
					
				}
				else
				{
					LOG_INFO(StringUtils::format("Brick HP:%d/%d", brick->gethp(),brick->getMaxhp()));
				}
			}

		}
		else if (SB->getTag() == DEADZONE_TAG && SA->getTag() == BALL_TAG)
		{
			balltodestroy = dynamic_cast<Ball*>(SA);
			balltodestroy->setPlateBelongto(dynamic_cast<Deadzone*>(SB)->getOwner());
		}
		else if (SB->getTag() == PLATE_TAG && SA->getTag() == BALL_TAG)
		{
			dynamic_cast<Ball*>(SA)->setPlateBelongto(dynamic_cast<Plate*>(SB));
		}
		else if (SA->getTag() == PLATE_TAG && SB->getTag() == BALL_TAG)
		{
			dynamic_cast<Ball*>(SB)->setPlateBelongto(dynamic_cast<Plate*>(SA));
		}
	}
	return true;
}

void Game::updateballspeed()
{
	//1.保持球的速度
	//2.更新球的速度实现动态难度
	if (isRoundStarted)
		for (auto ball : V_balls)
		{
			if (ball->getPhysicsBody()->getVelocity().getLength() != ball->getSpeed())
			{
				auto velocity = ball->getPhysicsBody()->getVelocity() / ball->getPhysicsBody()->getVelocity().getLength()*ball->getSpeed();
				ball->getPhysicsBody()->setVelocity(velocity);
			}
			if (!_gamescreen.containsPoint(ball->getPosition()))
			{
				balltodestroy = ball;
			}
		}
}

void Game::collisionDetection(Plate* player)
{
	for (auto iter = V_items.begin(); iter != V_items.end(); )
	{
		auto it = *iter;
		if (it->getPositionY() <= 0 || it->getPositionY() >= _gamescreenHeight)
		{
			iter = V_items.erase(iter);
			this->removeChild(it);
		}
		else if (player->getBoundingBox().intersectsRect(it->getBoundingBox()))
		{
			switch (it->getItemtype())
			{
			case BallPlus:
			{
				if (V_balls.size() > 2 || !isRoundStarted)
					break;
				createballwithBall(V_balls.back());
				LOG_INFO(StringUtils::format("Ball:%d", V_balls.size()));
				break;
			}
			case LifeRecover:
			{
				if (islife)
				{
					player->recover();
					if (player == _player1)
						_showlife->setString(StringUtils::format("%d", player->getLife()));
					
					LOG_INFO("Life Recover:+1");
					break;
				}
				else
				{
					recoverTime();
					break;
				}
					
			}
			case Expande:
			{
				player->expand();
				LOG_INFO(StringUtils::format("Expand:length %.1f",player->getScaleX()));
				break;
			}
			case Shrink:
			{
				player->shrink();
				LOG_INFO(StringUtils::format("Shrink:length %.1f", player->getScaleX()));
				break;
			}
			case BallSpeedUp:
			{
				for (auto it : V_balls)
				{
					it->speedUp();
				}
				LOG_INFO(StringUtils::format("Ball SpeedUP\n %.1f", V_balls.back()->getSpeed()));
				break;
			}
			case BallSpeedDown:
			{
				for (auto it : V_balls)
				{
					it->speedDown();
				}
				LOG_INFO(StringUtils::format("Ball SpeedDOWN\n %.1f", V_balls.back()->getSpeed()));
				break;
			}
			case PlateSpeedUp:
			{
				player->speedUp();
				LOG_INFO(StringUtils::format("Player SpeedUP\n %.1f", player->getVelocity()));
				break;
			}
			case PlateSpeedDown:
			{
				player->speedDown();
				LOG_INFO(StringUtils::format("Player SpeedDOWN\n %.1f", player->getVelocity()));
				break;
			}
			case BallLarger:
			{
				for (auto it : V_balls)
				{
					it->larger();
				}
				LOG_INFO(StringUtils::format("Bigger ball: %.1f", V_balls.back()->getScale()));
				break;
			}
			case BallSmaller:
			{
				for (auto it : V_balls)
				{
					it->smaller();
				}
				LOG_INFO(StringUtils::format("Smaller ball: %.1f", V_balls.back()->getScale()));
				break;
			}
			case Points:
			{
				_score += 50;
				_showscore->setString(StringUtils::format("%d", _score));
				LOG_INFO("Bonus:Point+50");
				break;
			}
			}

			iter = V_items.erase(iter);
			this->removeChild(it);
		}
		else
		{
			iter++;
		}
	}
}

void Game::recordBrick(Brick* brick)
{

}
void Game::initMode()
{

}
void Game::recoverTime()
{

}
void Game::writeScoreToUserData()
{
	vector<classic_data> v_classic;
	int finalscore = _score*10 / (allbrickcount-brickcount);
	for (int i = 0; i < 10; i++)
	{
		string score = StringUtils::format("classic_score_%d", i);
		string brick = StringUtils::format("classic_brick_%d", i);
		string level = StringUtils::format("classic_level_%d", i);
		int s;
		if((s = UserDefault::getInstance()->getIntegerForKey(score.c_str(),-1))==-1)
			break;
		else
		{
			classic_data temp;
			temp.score = s;
			temp.brick = UserDefault::getInstance()->getIntegerForKey(brick.c_str(),-1);
			temp.level = UserDefault::getInstance()->getIntegerForKey(level.c_str(), -1);
			v_classic.push_back(temp);
		}
	}
	v_classic.push_back(classic_data(finalscore, allbrickcount-brickcount, _level));
	sort(v_classic.begin(), v_classic.end());
	for (size_t i = 0; i < (v_classic.size() < 10 ? v_classic.size() : 10); i++)
	{
		string score = StringUtils::format("classic_score_%d", i);
		string brick = StringUtils::format("classic_brick_%d", i);
		string level = StringUtils::format("classic_level_%d", i);
		UserDefault::getInstance()->setIntegerForKey(score.c_str(), v_classic.at(i).score);
		UserDefault::getInstance()->setIntegerForKey(brick.c_str(), v_classic.at(i).brick);
		UserDefault::getInstance()->setIntegerForKey(level.c_str(), v_classic.at(i).level);
	}

	int time= UserDefault::getInstance()->getIntegerForKey("classic_played",0 );
	int average=UserDefault::getInstance()->getIntegerForKey("classic_average",0 );
	int maxscore=UserDefault::getInstance()->getIntegerForKey("classic_maxscore",0 );
	
	average = (average*time + finalscore) / (time + 1);
	time++;
	maxscore = maxscore > _score ? maxscore : _score;
	UserDefault::getInstance()->setIntegerForKey("classic_played",time );
	UserDefault::getInstance()->setIntegerForKey("classic_average",average );
	UserDefault::getInstance()->setIntegerForKey("classic_maxscore",maxscore );
}


void Game::closeOthers()
{

}