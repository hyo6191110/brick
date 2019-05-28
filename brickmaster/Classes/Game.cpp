#include "Game.h"
USING_NS_CC;

Scene*  Game::createScene(int life, int level)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
	this->unscheduleUpdate();
    //SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
    //SpriteFrameCache::getInstance->destroyInstance();
    //this->_eventDispatcher->removeAllEventListeners();
    Director::getInstance()->popScene();

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
				//log("%f", ball->getPhysicsBody()->getVelocity().getLength());
				auto velocity=ball->getPhysicsBody()->getVelocity()/ ball->getPhysicsBody()->getVelocity().getLength()*ball->getSpeed();
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
				break;
			}
			case LifeRecover:
			{
				player->recover();
				_showlife->setString(StringUtils::format("%d", player->getLife()));
				break;
			}
			case Expande:
			{
				player->setScaleX(1.5f);
				break;
			}
			case Shrink:
			{
				player->setScaleX(0.6f);
				break;
			}
			case Points:
			{
				_score += 50;
				_showscore->setString(StringUtils::format("%d", _score));
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

void Game::update(float delta)
{
	//1.通用
	//道具与玩家碰撞检测
	collisionDetection(_player1);
	//更新球速
	updateballspeed();
	//发射前让球紧跟板移动
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
	//2.针对不同模式的不同逻辑判断
	updateMode();
				
}

void Game::gameOver(bool isWin)
{
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
	if (isWin)
	{
		auto show = Label::createWithTTF("You win!","fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		show->setPosition(_centerX, _centerY);
		addChild(show);
		//pop board:back,retry
		//write to user file
	}
	else
	{
		auto show = Label::createWithTTF("You lose!", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		show->setPosition(_centerX, _centerY);
		addChild(show);
		//pop board:back,retry
		//write to user file
	}

}

void Game::createPlayerSide(int side, Plate*& player, Deadzone*& deadzone)
{
	deadzone = Deadzone::create("game\\deadzone.png");
	deadzone->setTextureRect(Rect(0, 0, _gamescreenWidth, 20));
	player = Plate::create("game\\defender_1.png");

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
	auto _ball = Ball::create("game\\ball_L.png");
	float x = origin->getPositionX();
	float y = origin->getPositionY();
	_ball->initWithData(x, y);
	_ball->setTag(1);
	_ball->setPlateBelongto(origin->getPlateBelongto());
	this->addChild(_ball);
	V_balls.push_back(_ball);
	_ball->getPhysicsBody()->setVelocity(origin->getPhysicsBody()->getVelocity());
}

void Game::createballforPlate(Plate* owner)
{
	auto _ball = Ball::create("game\\ball_L.png");
	float x = owner->getPositionX();
	float y = owner->getPositionY() + this->_player1->getContentSize().height / 2 + _ball->getContentSize().height / 2;
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
		auto filename = StringUtils::format("game//brick_%d.png", type);
		if (type <50)
		{
			auto brick = Brick::create(filename,type);
			brick->initWithData(x, y);
			brickcount++;
			this->addChild(brick);
		}
		else 
		{
			if (type == 51)
			{
				auto brick = Brick::create(filename, 1, 10, true, false);
				brick->initWithData(x, y);
				this->addChild(brick);
			}
			else if (type == 52)
			{
				auto brick = Brick::create(filename, 1, 10, false, true);
				brick->initWithData(x, y);
				brick->setbrickcount(false);
				this->addChild(brick);
			}
		}

	}
}

void Game::createBrickItem(float x, float y, int side)
{
	float y_destination,distance;
	if (side == 1)
		y_destination = 0,distance=y;
	else if (side == 2)
		y_destination = _gamescreenHeight,distance=_gamescreenHeight-y;
	else
		return;

	int i_type = random() % BrickItem::getMaxitemtype() + 1;
	auto fileName = StringUtils::format("game\\item_%d.png", i_type);
	auto item = BrickItem::create(fileName);
	item->initWithData(x,y);
	item->setItemType(static_cast<ItemType>(i_type - 1));
	this->addChild(item);
	V_items.push_back(item);
	item->runAction(MoveTo::create(distance / item->getDropspeed(), Vec2(x, y_destination)));
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//1.创建退出按钮
	auto closeItem = MenuItemImage::create(
		"back.png",
		"back_s.png",
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
	//6.根据游戏模式给出特别化的界面创建
	this->initMode(); 
	//7.游戏的主循环、碰撞检测、按键事件检测
	this->scheduleUpdate();

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactPostSolve = CC_CALLBACK_2(Game::onContactPostSolve, this);
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

	auto tlife = Label::createWithTTF("Life:", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	auto str = StringUtils::format("%d", _life);
	_showlife = Label::createWithTTF(str, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tlife->setPosition(x, y - 100);
	_showlife->setPosition(x, y - 200);
	this->addChild(tlife);
	this->addChild(_showlife);

	auto tscore = Label::createWithTTF("Score:", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	auto str2 = StringUtils::format("%d", _score);
	_showscore = Label::createWithTTF(str2, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tscore->setPosition(x, y - 300);
	_showscore->setPosition(x, y - 400);
	this->addChild(tscore);
	this->addChild(_showscore);

	_showlevel = Label::createWithTTF(StringUtils::format("Level %d", _level), "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	_showlevel->setPosition(x, y - 500);
	this->addChild(_showlevel);

	createHUDMode();
}
void Game::initAfter()
{
	createHUD();
	std::string LevelFile = StringUtils::format("levels\\level_%d.json", _level);
	createBricksFromFile(LevelFile);
}
void Game::initMode()
{

}
void Game::updateMode()
{
	if (balltodestroy)
	{
		auto player = balltodestroy->getPlateBelongto();
		destroyball(balltodestroy);
		if (V_balls.empty())
		{
			player->damaged();
			_showlife->setString(StringUtils::format("%d", _player1->getLife()));
			if (_player1->getLife() == 0)
			{
				gameOver(false);
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
		gameOver(true);
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

void Game::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve)
{
	auto SA = contact.getShapeA()->getBody()->getNode();
	auto SB = contact.getShapeB()->getBody()->getNode();
	if (SA&&SB)
	{
		if (SB->getTag() == BRICK_TAG && SA->getTag() == BALL_TAG)
		{
			auto ball= dynamic_cast<Ball*>(SA);
			auto brick = dynamic_cast<Brick*>(SB);
			_score += 1;
			_showscore->setString(StringUtils::format("%d", _score));
			if (!brick->isunbreakable())
			{
				brick->damaged();
				if (brick->gethp() == 0)
				{
					_score += brick->getscore();
					_showscore->setString(StringUtils::format("%d", _score));
					if (brick->isdropitem())
					{
						int side = 1;
						if (ball->getPlateBelongto()->getPositionY() > 500)
							side = 2;
						createBrickItem(brick->getPositionX(), brick->getPositionY(),side);
					}
					if (brick->isbrickcount())
						brickcount--;
					this->removeChild(SB);

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
}