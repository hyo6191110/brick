#include "Level.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;
#include "Game.h"
#include "GameModePong.h"
#include "GameModeTimelimit.h"

Scene* Level::createScene()
{
	auto scene = Scene::create();
	auto layer = Level::create();
	scene->addChild(layer);
	return scene;
}

void Level::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

void Level::addmode(cocos2d::Ref* pSender)
{
	if ((int)_currentmode < MaxMode - 1)
	{
		_currentmode = (GameMode)((int)_currentmode + 1);
	}
	switch (_currentmode) 
	{
	case Classic:
	{
		_showmode->setString("Classic"); 
		setAIVisible(false); break;
	}
	case Pong:
	{
		_showmode->setString("Pong"); 
		setAIVisible(true); break;
	}	
	case Timelimit:
	{
		_showmode->setString("Timelimit"); 
		setAIVisible(false); break;
	}	
	}
}
void Level::minusmode(cocos2d::Ref* pSender)
{
	if ((int)_currentmode != 0)
	{
		_currentmode = (GameMode)((int)_currentmode - 1);
	}
	switch (_currentmode)
	{
	case Classic:
	{
		_showmode->setString("Classic");
		setAIVisible(false); break;
	}
	case Pong:
	{
		_showmode->setString("Pong");
		setAIVisible(true); break;
	}
	case Timelimit:
	{
		_showmode->setString("Timelimit");
		setAIVisible(false); break;
	}
	}
}
void Level::addlife(cocos2d::Ref* pSender)
{
	if (_currentlife < MaxLife)
	{
		_currentlife++;

		_showlife->setString(StringUtils::format("%d", _currentlife));
	}
}
void Level::minuslife(cocos2d::Ref* pSender)
{
	if (_currentlife !=1)
	{
		_currentlife--;

		_showlife->setString(StringUtils::format("%d", _currentlife));
	}
}
void Level::addtime(cocos2d::Ref* pSender)
{
	if (_currenttime < MaxTime)
	{
		_currenttime += 30;

		_showtime->setString(StringUtils::format("%d", _currenttime));
	}
}
void Level::minustime(cocos2d::Ref* pSender)
{
	if (_currenttime != 30)
	{
		_currenttime-=30;

		_showtime->setString(StringUtils::format("%d", _currenttime));
	}
}
void Level::openlevel(cocos2d::Ref* pSender)
{
	Scene* scene;
	switch (_currentmode)
	{
	case GameMode::Classic:
		scene = Game::createScene(_currentlife, _currentlevel); break;
	case GameMode::Pong:
		scene = GameModePong::createScenePong(_currentlife, _currentlevel,_currentdifficulty); break;
	case GameMode::Timelimit:
		scene = GameModeTimelimit::createSceneTimelimit(_currentlevel,_currenttime); break;
	}
	Director::getInstance()->pushScene(scene);

}
void Level::setlevel(int level)
{
	this->_currentlevel = level;
	this->_showlevel->setString(StringUtils::format("Level:%d", level));
}
bool Level::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background/background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg->setLocalZOrder(-5);
	this->addChild(bg);

	_currentlevel = 1;
	_currentlife = 3;
	_currentmode = Classic;
	_currenttime = 90;
	_currentdifficulty = 2;

	auto tmode = Label::createWithTTF("Mode", "fonts\\BOD_R.ttf",48,Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto tlife = Label::createWithTTF("Life", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto ttime = Label::createWithTTF("Time", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showmode=Label::createWithTTF("Classic", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showlife = Label::createWithTTF("3", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showtime = Label::createWithTTF("90", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showlevel= Label::createWithTTF("Level:1", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);

	auto modeprev = MenuItemImage::create("button_next_r.png", "button_next_selected_r.png", CC_CALLBACK_1(Level::minusmode, this));
	auto modenext = MenuItemImage::create("button_next.png", "button_next_selected.png", CC_CALLBACK_1(Level::addmode, this));
	auto lifeprev = MenuItemImage::create("button_next_r.png", "button_next_selected_r.png", CC_CALLBACK_1(Level::minuslife, this));
	auto lifenext = MenuItemImage::create("button_next.png", "button_next_selected.png", CC_CALLBACK_1(Level::addlife, this));
	auto timeprev = MenuItemImage::create("button_next_r.png", "button_next_selected_r.png", CC_CALLBACK_1(Level::minustime, this));
	auto timenext = MenuItemImage::create("button_next.png", "button_next_selected.png", CC_CALLBACK_1(Level::addtime, this));
	auto startgame = MenuItemImage::create("button_play.png", "button_play_s.png", CC_CALLBACK_1(Level::openlevel, this));

	float x1 = 100;
	float x2 = x1 + 100 + modeprev->getContentSize().width / 2;
	float x3 = x2 + 175;
	float x4 = x3 + 100 + modenext->getContentSize().width / 2;
	float x6 = x4 + 150;
	float x7 = x6 + 175;
	float x5 = visibleSize.width - startgame->getContentSize().width / 2;
	float y1 = visibleSize.height-50;
	float y_ai = y1 - 10;
	float y2 = y1-100;
	float y4 = y2 - 100;
	float y3 = y1 - startgame->getContentSize().height;
	
	tmode->setPosition(x1, y1);
	modeprev->setPosition(x2, y1);
	_showmode->setPosition(x3, y1);
	modenext->setPosition(x4, y1);
	tlife->setPosition(x1, y2);
	lifeprev->setPosition(x2, y2);
	_showlife->setPosition(x3, y2);
	lifenext->setPosition(x4, y2);
	ttime->setPosition(x1, y4);
	timeprev->setPosition(x2, y4);
	_showtime->setPosition(x3, y4);
	timenext->setPosition(x4, y4);
	_showlevel->setPosition(x5, y1);
	startgame->setPosition(x5, y3);

	this->addChild(tmode);
	this->addChild(tlife);
	this->addChild(ttime);
	this->addChild(_showmode);
	this->addChild(_showlife);
	this->addChild(_showtime);
	this->addChild(_showlevel);

	_change_difficulty = MenuItemImage::create("ui/button_AI.png", "ui/button_AI_s.png", 
		[=](Ref* psender)
	    {
		if (_currentdifficulty != 5)
			_currentdifficulty++;
		else
			_currentdifficulty = 1;
		switch (_currentdifficulty)
		{
		case 1:
			_showdifficulty->setString("Easy"); break;
		case 2:
			_showdifficulty->setString("Average"); break;
		case 3:
			_showdifficulty->setString("Normal"); break;
		case 4:
			_showdifficulty->setString("Hard"); break;
		case 5:
			_showdifficulty->setString("Master"); break;
		}

	    });
	_change_difficulty->setPosition(x7, y_ai);
	_title_difficulty = Label::createWithTTF("AI", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_title_difficulty->setPosition(x6,y_ai);
	this->addChild(_title_difficulty);
	_showdifficulty = Label::createWithTTF("Average", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showdifficulty->setPosition(x7, y_ai);
	_showdifficulty->setLocalZOrder(5);
	_showdifficulty->setColor(Color3B::BLACK);
	this->addChild(_showdifficulty);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Level::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem,modeprev,modenext,lifeprev,lifenext, timeprev, timenext, startgame,_change_difficulty, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	setAIVisible(false);
	createLevelSelection(y3);
	
	return true;

}
void Level::createLevelSelection(float y_start)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (int i = 0; i < MaxLevel; i++)
	{
		auto str = StringUtils::format("%d", i + 1);
		auto num = Label::createWithTTF(str, "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		float lx = visibleSize.width / 12 + visibleSize.width / 6 * (i % 6);
		float ly = y_start - 150 - (y_start - 150) / 3 * (i / 6);
		num->setPosition(lx, ly);
		this->addChild(num, 2);
		auto imagename = "level_blue.png";
		auto level = Sprite::create(imagename);
		level->setTag(i + 1);
		level->setPosition(lx, ly);
		this->addChild(level, 1);
	}

	auto gameListener = EventListenerTouchOneByOne::create();
	gameListener->onTouchBegan = [=](Touch* touch, Event* event) {
		Vec2 nodeLoca = this->convertToNodeSpace(touch->getLocation());

		for (int i = 0; i < MaxLevel; i++)
		{
			if (this->getChildByTag(i + 1)->getBoundingBox().containsPoint(nodeLoca))
			{
				this->setlevel(i + 1);
			}
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);
}