#include "UserLevelSelection.h"
#include<fstream>
USING_NS_CC;
using namespace std;
#include "Game.h"
#include "GameModePong.h"
#include "GameModeTimelimit.h"

Scene* UserLevelSelection::createScene()
{
	auto scene = Scene::create();
	auto layer = UserLevelSelection::create();
	scene->addChild(layer);
	return scene;
}
void UserLevelSelection::createLevelSelection(float y_start)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_currentlevel = 10000+UserDefault::getInstance()->getIntegerForKey("user_select_level",1);
	_showlevel->setString(StringUtils::format("Level\nUser\n%d", _currentlevel - 10000));

	_show_info=Label::createWithTTF("Existed File", "fonts\\BOD_R.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	std::string filename = FileUtils::getInstance()->getWritablePath() + StringUtils::format("user_level_%d.json", _currentlevel - 10000);
	if (!FileUtils::getInstance()->isFileExist(filename))
		_show_info->setString("No File");

	auto book = Sprite::create("menu/menu_book.png");
	auto levelprev = MenuItemImage::create("ui/button_next_r.png", "ui/button_next_selected_r.png", CC_CALLBACK_1(UserLevelSelection::minuslevel, this));
	auto levelnext = MenuItemImage::create("ui/button_next.png", "ui/button_next_selected.png", CC_CALLBACK_1(UserLevelSelection::addlevel, this));
	float y1 = book->getContentSize().height / 2;
	float y2 = 100;
	float x3 = visibleSize.width / 2;
	float x2 = x3 - book->getContentSize().width / 4;
	float x4 = x3 + book->getContentSize().width / 4;
	float x1 = x3 - book->getContentSize().width / 2-levelnext->getContentSize().width/2;
	float x5 = x3 + book->getContentSize().width / 2 + levelnext->getContentSize().width / 2;
	book->setPosition(x3, y1); book->setLocalZOrder(-1);
	_showlevel->setPosition(x2, y1); _showlevel->setColor(Color3B::BLACK);
	_show_info->setPosition(x4, y1); _show_info->setColor(Color3B::BLACK);
	levelprev->setPosition(x1, y1);
	levelnext->setPosition(x5, y1);
	auto menu = Menu::create(levelnext,levelprev, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	this->addChild(book);
	this->addChild(_show_info);
}
void UserLevelSelection::addlevel(Ref* pSender)
{
	_currentlevel++;
	_showlevel->setString(StringUtils::format("Level\nUser\n%d", _currentlevel - 10000));
	std::string filename = FileUtils::getInstance()->getWritablePath() + StringUtils::format("user_level_%d.json", _currentlevel - 10000);
	if (FileUtils::getInstance()->isFileExist(filename))
	{
		_show_info->setString("Existed File");
	}
	else
	{
		_show_info->setString("No File");
	}
	UserDefault::getInstance()->setIntegerForKey("user_select_level", _currentlevel - 10000);
}
void UserLevelSelection::minuslevel(Ref* pSender)
{
	if (_currentlevel != 10001)
	{
		_currentlevel--;
		_showlevel->setString(StringUtils::format("Level\nUser\n%d", _currentlevel - 10000));
		std::string filename = FileUtils::getInstance()->getWritablePath() + StringUtils::format("user_level_%d.json", _currentlevel - 10000);
		if (FileUtils::getInstance()->isFileExist(filename))
		{
			_show_info->setString("Existed File");
		}
		else
		{
			_show_info->setString("No File");
		}
		UserDefault::getInstance()->setIntegerForKey("user_select_level", _currentlevel - 10000);
	}


}
void UserLevelSelection::openlevel(cocos2d::Ref* pSender)
{
	if (_show_info->getString() == "Existed File")
	{
		Scene* scene;
		switch (_currentmode)
		{
		case GameMode::Classic:
			scene = Game::createScene(_currentlife, _currentlevel); break;
		case GameMode::Pong:
			scene = GameModePong::createScenePong(_currentlife, _currentlevel, _currentdifficulty); break;
		case GameMode::Timelimit:
			scene = GameModeTimelimit::createSceneTimelimit(_currentlevel, _currenttime); break;
		}
		Director::getInstance()->pushScene(scene);
	}
}