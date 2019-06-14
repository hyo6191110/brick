#include "MultiGame.h"
USING_NS_CC;

Scene* MultiGame::createScene()
{
	auto scene = Scene::create();
	auto layer = MultiGame::create();
	scene->addChild(layer);
	return scene;
}

void MultiGame::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

bool MultiGame::init()
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

	auto MsgItem=MenuItemLabel::create(Label::createWithTTF("Send Message","fonts/BOD_R.ttf",56), CC_CALLBACK_1(MultiGame::sendMsg, this));
	MsgItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

	auto StartItem = MenuItemLabel::create(Label::createWithTTF("Start", "fonts/BOD_R.ttf", 56), CC_CALLBACK_1(MultiGame::startGame, this));
	StartItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 300);

	auto closeItem = MenuItemImage::create(
		"ui/CloseNormal.png",
		"ui/CloseSelected.png",
		CC_CALLBACK_1(MultiGame::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem,MsgItem,StartItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_show_info = Label::createWithTTF("Hello World!", "fonts/BOD_R.ttf", 28);
	_show_info->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
	addChild(_show_info);


	return true;
}
void MultiGame::sendMsg(Ref* pSender)
{

}

void MultiGame::startGame(Ref* pSender)
{
	//auto scene = WebGame::createScene(9, 90);
	//Director::getInstance()->pushScene(scene);
}

