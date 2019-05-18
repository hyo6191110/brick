#include "SingleGame.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
#include "Level.h"
#include "LevelDesigner.h"

Scene* SingleGame::createScene()
{
	auto scene = Scene::create();
	auto layer = SingleGame::create();
	scene->addChild(layer);
	return scene;
}

void SingleGame::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

void SingleGame::SwitchToFreeSelete(cocos2d::Ref* pSender)
{	
	auto scene = Level::createScene();
	Director::getInstance()->pushScene(scene);
}

void SingleGame::SwitchToDesigner(cocos2d::Ref* pSender)
{
	auto scene = LevelDesigner::createScene();
	Director::getInstance()->pushScene(scene);
}

bool SingleGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(SingleGame::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	
	auto freemode = MenuItemImage::create(
		"free_selectlevel.png",
		"free_selectlevel_s.png", CC_CALLBACK_1(SingleGame::SwitchToFreeSelete, this));
	freemode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
 	
	auto designer = MenuItemImage::create(
		"level_design.png",
		"level_design_s.png", CC_CALLBACK_1(SingleGame::SwitchToDesigner, this));
	designer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1 / 4));

    auto menu = Menu::create(closeItem,freemode,designer, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}