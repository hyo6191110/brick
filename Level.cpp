#include "Level.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;
#include "Game.h"

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
	if ((int)_currentmode < MaxMode-1)
	{
		_currentmode = (GameMode)((int)_currentmode + 1);
	
	switch (_currentmode) {
	case Classic: {
		_showmode->setString("Classic");
		break;
	}
	default:break;
	}
	}
}
void Level::minusmode(cocos2d::Ref* pSender)
{
	if ((int)_currentmode !=0)
	{
		_currentmode = (GameMode)((int)_currentmode - 1);
	
	switch (_currentmode) {
	case Classic: {
		_showmode->setString("Classic");
		break;
	}
	default:break;
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
void Level::openlevel(cocos2d::Ref* pSender)
{
	auto scene = Game::createSceneWithLevel(_currentmode, _currentlife, _currentlevel);
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

	_currentlevel = 1;
	_currentlife = 3;
	_currentmode = Classic;

	auto tmode = Label::createWithTTF("Mode", "fonts\\COLONNA.ttf",48,Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto tlife = Label::createWithTTF("Life", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showmode=Label::createWithTTF("Classic", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showlife = Label::createWithTTF("3", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showlevel= Label::createWithTTF("Level:1", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);

	auto modeprev = MenuItemImage::create("button_next_r.png", "button_next_selected_r.png", CC_CALLBACK_1(Level::minusmode, this));
	auto modenext = MenuItemImage::create("button_next.png", "button_next_selected.png", CC_CALLBACK_1(Level::addmode, this));
	auto lifeprev = MenuItemImage::create("button_next_r.png", "button_next_selected_r.png", CC_CALLBACK_1(Level::minuslife, this));
	auto lifenext = MenuItemImage::create("button_next.png", "button_next_selected.png", CC_CALLBACK_1(Level::addlife, this));
	auto startgame = MenuItemImage::create("button_play.png", "button_play_s.png", CC_CALLBACK_1(Level::openlevel, this));

	float x1 = 100;
	float x2 = x1 + 50 + modeprev->getContentSize().width / 2;
	float x3 = x2 + 50 + 150;
	float x4 = x3 + 100 + modenext->getContentSize().width / 2;
	float x5 = visibleSize.width - startgame->getContentSize().width / 2;
	float y1 = visibleSize.height-50;
	float y2 = y1-100;
	float y3 = y1 - startgame->getContentSize().height;
	
	tmode->setPosition(x1, y1);
	modeprev->setPosition(x2, y1);
	_showmode->setPosition(x3, y1);
	modenext->setPosition(x4, y1);
	tlife->setPosition(x1, y2);
	lifeprev->setPosition(x2, y2);
	_showlife->setPosition(x3, y2);
	lifenext->setPosition(x4, y2);
	_showlevel->setPosition(x5, y1);
	startgame->setPosition(x5, y3);

	this->addChild(tmode);
	this->addChild(tlife);
	this->addChild(_showmode);
	this->addChild(_showlife);
	this->addChild(_showlevel);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Level::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem,modeprev,modenext,lifeprev,lifenext,startgame, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//levelselection
	for (int i = 0; i < MaxLevel; i++)
	{
		auto str = StringUtils::format("%d", i + 1);
		auto num = Label::createWithTTF(str, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
		float lx = visibleSize.width / 12 + visibleSize.width / 6 * (i % 6);
		float ly = y3 - 150 - (y3  - 150) / 3 * (i / 6);
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
	return true;

}
