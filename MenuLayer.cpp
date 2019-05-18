#include "MenuLayer.h"
#include "SingleGame.h"
#include "MultiGame.h"
#include "MenuSettings.h"
#include "MenuUser.h"
#include "MenuStore.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}

void MenuLayer::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->end();

}

void MenuLayer::SwitchtoSingle(cocos2d::Ref* pSender)
{
	auto scene = SingleGame::createScene();
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::SwitchtoMulti(cocos2d::Ref* pSender)
{
	auto scene = MultiGame::createScene();
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoSettings(cocos2d::Ref* pSender)
{
	auto layer = MenuSettings::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoUser(cocos2d::Ref* pSender)
{
	auto layer = MenuUser::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoStore(cocos2d::Ref* pSender)
{
	auto layer = MenuStore::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	auto title = Sprite::create("menu\\title.png");
	title->setPosition(Vec2(visibleSize.width/2, visibleSize.height-title->getContentSize().height/2));
	this->addChild(title);

	auto single = Sprite::create("menu\\menu_01.png");
	auto single_s = Sprite::create("menu\\menu_01_s.png");

	auto multi = Sprite::create("menu\\menu_02.png");
	auto multi_s = Sprite::create("menu\\menu_02_s.png");

	auto setting = Sprite::create("menu\\menu_03.png");
	auto setting_s = Sprite::create("menu\\menu_03_s.png");

	auto user = Sprite::create("menu\\menu_04.png");
	auto user_s = Sprite::create("menu\\menu_04_s.png");

	auto store = Sprite::create("menu\\menu_05.png");
	auto store_s = Sprite::create("menu\\menu_05_s.png");

	auto singleItem = MenuItemSprite::create(single, single_s, CC_CALLBACK_1(MenuLayer::SwitchtoSingle, this));
	singleItem->setPosition(Vec2(visibleSize.width / 2 - single->getContentSize().width * 3 / 2,
		visibleSize.height - title->getContentSize().height - single->getContentSize().height / 2));

	auto multiItem = MenuItemSprite::create(multi, multi_s, CC_CALLBACK_1(MenuLayer::SwitchtoMulti, this));
	multiItem->setPosition(Vec2(visibleSize.width / 2 - multi->getContentSize().width  / 2,
		visibleSize.height - title->getContentSize().height - multi->getContentSize().height / 2));

	auto settingItem = MenuItemSprite::create(setting, setting_s, CC_CALLBACK_1(MenuLayer::PoptoSettings, this));
	settingItem->setPosition(Vec2(visibleSize.width / 2 + setting->getContentSize().width / 2,
		visibleSize.height - title->getContentSize().height - setting->getContentSize().height / 2));

	auto userItem = MenuItemSprite::create(user, user_s, CC_CALLBACK_1(MenuLayer::PoptoUser, this));
	userItem->setPosition(Vec2(visibleSize.width / 2 + user->getContentSize().width * 3 / 2,
		visibleSize.height - title->getContentSize().height - user->getContentSize().height / 2));

	auto storeItem = MenuItemSprite::create(store, store_s, CC_CALLBACK_1(MenuLayer::PoptoStore, this));
	storeItem->setPosition(Vec2(visibleSize.width / 2 + user->getContentSize().width * 3 / 2,
		store->getContentSize().height / 2));
	storeItem->setScale(0.8f);

	auto menu = Menu::create(singleItem, multiItem, settingItem, userItem, storeItem, NULL);
	menu->setPosition(Vec2::ZERO);

	this->addChild(menu);

	return true;
	
}