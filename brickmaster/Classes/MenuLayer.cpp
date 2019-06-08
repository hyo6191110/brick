#include "MenuLayer.h"
#include "SingleGame.h"
#include "MultiGame.h"
#include "MenuSettings.h"
#include "MenuUser.h"
#include "MenuStore.h"
USING_NS_CC;
#include"SimpleAudioEngine.h"
using namespace CocosDenshion;

Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}

void MenuLayer::Closethis(Ref* pSender)
{
	unloadResources();
	this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->end();
}

void MenuLayer::SwitchtoSingle(cocos2d::Ref* pSender)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/effect/click.wav");
	auto scene = SingleGame::createScene();
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::SwitchtoMulti(cocos2d::Ref* pSender)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/effect/click.wav");
	auto scene = MultiGame::createScene();
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoSettings(cocos2d::Ref* pSender)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/effect/click.wav");
	auto layer = MenuSettings::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoUser(cocos2d::Ref* pSender)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/effect/click.wav");
	auto layer = MenuUser::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void MenuLayer::PoptoStore(cocos2d::Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/effect/click.wav");
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

	auto bg = Sprite::create("background/background_menu_02.png");
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
void MenuLayer::preloadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("designer/design_bricks.plist", "designer/design_bricks.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/game_sprite_default.plist", "game/game_sprite_default.png");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/ball_deadzone.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/ball_plate.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/ball_wall.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/brick_damage.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/brick_destroy.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/shoot_ball.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/newItem.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/getItem.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/win.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/lose.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/click.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/pop.wav");
}
void MenuLayer::unloadResources()
{
	SpriteFrameCache::getInstance()->removeSpriteFrames();
	SpriteFrameCache::getInstance()->destroyInstance();
	SimpleAudioEngine::getInstance()->end();
}
void MenuLayer::onEnter()
{
	Layer::onEnter();
	preloadResources();
}