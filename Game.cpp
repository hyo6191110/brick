#include "Game.h"
USING_NS_CC;

Scene*  Game::createSceneWithLevel(Level::GameMode mode, int life, int level)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Game::create();
	layer->_mode = mode;
	layer->_life = life;
	layer->_level = level;
	auto str = StringUtils::format("%d", layer->_life);
	layer->_showlife->setString(str);
	auto str2 = StringUtils::format("%d", layer->_score);
	layer->_showscore->setString(str2);
	layer->_showlevel->setString(StringUtils::format("Level %d", layer->_level));
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

void Game::update(float delta)
{
	
	
	if (!isRoundStarted)
	{
		if (V_balls.back()->getPositionX() != _player1->getPositionX())
		{
			V_balls.back()->setPositionX(_player1->getPositionX());
		}
	}
	switch (_mode)
	{
	case Level::GameMode::Classic:
		{
		if (balltodestroy)
		{
			destroyball(balltodestroy);

			if (V_balls.empty())
			{
				_life--;
				_showlife->setString(StringUtils::format("%d", _life));
				if (_life == 0)
				{
					gameOver(false);
				}
				else
				{
					createball();
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
			break;
		}
	default:
			break;
	}
	
}

void Game::gameOver(bool isWin)
{
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

void Game::createball(float x, float y)
{
	
	auto _ball = Ball::create("game\\ball_LL.png");

	if (x == 0 && y == 0)
	{
		x = this->_player1->getPositionX();
		y = this->_player1->getPositionY() + this->_player1->getContentSize().height / 2 + _ball->getContentSize().height / 2;
		
	}

	_ball->initWithData(x,y);
	_ball->setTag(1);

	this->addChild(_ball);
	V_balls.push_back(_ball);
	//V_balls.back()->getPhysicsBody()->setDynamic(false);
	//ball->getPhysicsBody()->setRotationEnable(false);
}
void Game::destroyball(Ball* ball)
{
	this->removeChild(ball);
	V_balls.erase(std::find(V_balls.begin(), V_balls.end(), ball));
}

void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode code, Event* event)
{

	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_A:
	{
		_player1->_startPosition = _player1->getPositionX();

		auto action = MoveTo::create((_player1->getPositionX() - (_centerX - _gamescreenWidth / 2))/_player1->_velocity,
			Vec2(_centerX - _gamescreenWidth / 2+ _player1->getContentSize().width / 2, _deadzone1->getContentSize().height + _player1->getContentSize().height / 2));
		action->setTag(1);
		_player1->runAction(action);

		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		_player1->_startPosition = _player1->getPositionX();

		auto action = MoveTo::create((-_player1->getPositionX() + (_centerX + _gamescreenWidth / 2)) / _player1->_velocity,
			Vec2(_centerX + _gamescreenWidth / 2- _player1->getContentSize().width/2, _deadzone1->getContentSize().height + _player1->getContentSize().height / 2));
		action->setTag(2);
		_player1->runAction(action);

		break;
	}
	case EventKeyboard::KeyCode::KEY_SPACE:
	{
		if (!isRoundStarted)
		{
			V_balls.back()->stopActionByTag(1);
			V_balls.back()->stopActionByTag(2);
			float hori_velo = (_player1->getPositionX() - _player1->_startPosition)*1.4 ;
			if (hori_velo < 0)
			{
				hori_velo -= random() % 100;
			}
			else if(hori_velo > 0)
			{
				hori_velo += random() % 100;
			}
			else
			{
				hori_velo += random() % 300;
				hori_velo -= random() % 300;
			}
			Vec2 velocity(hori_velo, V_balls.back()->_Vertical_velo);
			velocity.normalize();
			velocity *= V_balls.back()->speed;
			V_balls.back()->getPhysicsBody()->setVelocity(velocity);
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
		_player1->stopActionByTag(1);

		if (!isRoundStarted)
			V_balls.back()->stopActionByTag(1);

		_player1->_startPosition = _player1->getPositionX();
		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		_player1->stopActionByTag(2);

		if (!isRoundStarted)
			V_balls.back()->stopActionByTag(2);

		_player1->_startPosition = _player1->getPositionX();
		break;
	}
	default:
		break;
	}
}
bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

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

	auto info = Sprite::create("game\\status_board.png");
	info->setPosition(info->getContentSize().width / 2, visibleSize.height / 2);
	this->addChild(info);

	_gamescreenWidth = visibleSize.width-info->getContentSize().width;
	_gamescreenHeight = visibleSize.height;
	_centerX = info->getContentSize().width + _gamescreenWidth / 2;
	_centerY = _gamescreenHeight / 2;

	auto str = StringUtils::format("%d", _life);
	_showlife = Label::createWithTTF(str, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto str2 = StringUtils::format("%d", _score);
	_showscore = Label::createWithTTF(str2, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto tlife = Label::createWithTTF("Life:", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	auto tscore = Label::createWithTTF("Score:", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	_showlevel = Label::createWithTTF(StringUtils::format("Level %d", _level), "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	tlife->setPosition(info->getContentSize().width / 2, visibleSize.height - 100);
	_showlife->setPosition(info->getContentSize().width / 2, visibleSize.height - 200);
	tscore->setPosition(info->getContentSize().width / 2, visibleSize.height - 300);
	_showscore->setPosition(info->getContentSize().width / 2, visibleSize.height - 400);
	_showlevel->setPosition(info->getContentSize().width / 2, visibleSize.height - 500);
	this->addChild(tlife);
	this->addChild(_showlife);
	this->addChild(tscore);
	this->addChild(_showscore);
	this->addChild(_showlevel);

	//create wall
	Vec2 u_l(_centerX - _gamescreenWidth / 2, _centerY - _gamescreenHeight / 2);
	Vec2 u_r(_centerX + _gamescreenWidth / 2, _centerY - _gamescreenHeight / 2);
	Vec2 d_l(_centerX - _gamescreenWidth / 2, _centerY + _gamescreenHeight / 2);
	Vec2 d_r(_centerX + _gamescreenWidth / 2, _centerY + _gamescreenHeight / 2);
	Vec2 vec2[4] = { u_l,u_r,d_r,d_l };
	auto node = Node::create();
	auto body = PhysicsBody::createEdgePolygon(vec2, 4);
	body->getShapes().at(0)->setRestitution(1);
	body->setGroup(1);
	node->setPhysicsBody(body);
	node->setPosition(Vec2::ZERO);
	this->addChild(node);

	switch (_mode) {
	case Level::GameMode::Classic:this->createClassicLevel(); break;
	}
	//游戏的主循环
	this->scheduleUpdate();
	return true;
}

void Game::createClassicLevel()
{
	isGameOver = false;
	isRoundStarted = false;

	_deadzone1 = Sprite::create("game\\deadzone.png",Rect(0,0,_gamescreenWidth,20));
	auto body_1 = PhysicsBody::createEdgeBox(_deadzone1->getContentSize());
	body_1->setCategoryBitmask(0x01);
	body_1->setContactTestBitmask(0x01);
	body_1->setCollisionBitmask(0x01);
	_deadzone1->setPhysicsBody(body_1);
	
	_deadzone1->setPosition(_centerX ,0);
	_deadzone1->setTag(4);
	this->addChild(_deadzone1);

	_player1 = Plate::create("game\\defender_1.png");
	_player1->initWithData(_centerX, _deadzone1->getContentSize().height+ _player1->getContentSize().height/2);
	_player1->setTag(3);
	_player1->_startPosition = _centerX;
	this->addChild(_player1);

	createball();

	std::vector<BrickData*> V_brickdata;
	getAllBrickWithFile("levels\\exp.json", V_brickdata);

	for(int i=0;i<V_brickdata.size();i++)
	{
		int type = V_brickdata.at(i)->getType();
		int x = V_brickdata.at(i)->getX();
		int y = V_brickdata.at(i)->getY();
		if (type == 1)
		{
			auto brick = Brick::create("game\\brick_blue_L.png");
			brick->initWithData(_centerX + x * brick->getContentSize().width, _centerY + y * brick->getContentSize().height);
			brickcount++;
			this->addChild(brick);
		}

	}

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactPostSolve = [=](PhysicsContact& contact,const PhysicsContactPostSolve& solve) {
		auto SA = contact.getShapeA()->getBody()->getNode();
		auto SB = contact.getShapeB()->getBody()->getNode();
		if(SA&&SB){//very important
			if (SB->getTag() == 2 && SA->getTag() == 1) 
			{
				auto brick = dynamic_cast<Brick*>(SB);
				_score += brick->getscore();
				_showscore->setString(StringUtils::format("%d", _score));
				if (!brick->isunbreakable())
				{
					brick->damaged();
					if (brick->gethp() == 0)
					{
						if (brick->isitem())
						{
							//drop item
						}
						this->removeChild(SB);
						brickcount--;

					}

				}				

			}
			else
			if (SB->getTag() == 4 && SA->getTag() == 1)
			{
				balltodestroy= dynamic_cast<Ball*>(SA);
				
			}
			
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);
	
}