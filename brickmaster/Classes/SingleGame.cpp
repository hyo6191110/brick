#include "SingleGame.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
#include "scenes/Level.h"
#include "scenes/LevelDesigner.h"
#include "scenes/UserlevelSelection.h"

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
void SingleGame::SwitchToSelector(cocos2d::Ref* pSender)
{
	auto scene = UserLevelSelection::createScene();
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

	auto bg = Sprite::create("background/background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(SingleGame::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	
	auto freemode = MenuItemImage::create(
		"menu/free_selectlevel.png",
		"menu/free_selectlevel_s.png", CC_CALLBACK_1(SingleGame::SwitchToFreeSelete, this));
	freemode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
 	
	auto designer = MenuItemImage::create(
		"menu/level_design.png",
		"menu/level_design_s.png", CC_CALLBACK_1(SingleGame::SwitchToDesigner, this));
	designer->setPosition(Vec2(freemode->getBoundingBox().getMinX()+designer->getContentSize().width/2, visibleSize.height * 2 / 4));

	auto selector = MenuItemImage::create(
		"menu/level_select.png",
		"menu/level_select_s.png", CC_CALLBACK_1(SingleGame::SwitchToSelector, this));
	selector->setPosition(Vec2(freemode->getBoundingBox().getMinX() + designer->getContentSize().width +selector->getContentSize().width/2, visibleSize.height * 2 / 4));

    auto menu = Menu::create(closeItem,freemode,designer,selector ,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}