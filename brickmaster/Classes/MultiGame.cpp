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

	auto MsgItem=MenuItemLabel::create(Label::createWithTTF("Send Message","fonts/BOD_R.ttf",56), CC_CALLBACK_1(MultiGame::sendMsg, this));
	MsgItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MultiGame::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem,MsgItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_show_info = Label::createWithTTF("Hello World!", "fonts/BOD_R.ttf", 28);
	_show_info->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
	addChild(_show_info);

	_sioclient = SocketIO::connect("http://localhost:3000/", *this);
	_sioclient->setTag("Client1");
	_sioclient->on("callClientEvent", CC_CALLBACK_2(MultiGame::callClientEvent, this));

	return true;
}

void MultiGame::sendMsg(cocos2d::Ref* pSender)
{
	if (_sioclient)
	{
		_sioclient->send("Hello Socket.io!");
		_sioclient->emit("callServerEvent", "{\"message\":\"Hello Server.\"}");
	}
}

void MultiGame::onConnect(SIOClient* client)
{
	_show_info->setString(StringUtils::format("%s connected", client->getTag()));
}
void MultiGame::onMessage(SIOClient* client, const std::string& data)
{
	_show_info->setString(StringUtils::format("%s received message:%s", client->getTag(),data.c_str()));
}
void MultiGame::onClose(SIOClient* client)
{
	_show_info->setString(StringUtils::format("%s closed", client->getTag()));
	if (client == _sioclient)
		_sioclient = nullptr;
}
void MultiGame::onError(SIOClient* client, const std::string& data)
{
	_show_info->setString(StringUtils::format("%s received error:%s", client->getTag(), data.c_str()));
}

void MultiGame::callClientEvent(SIOClient* client, const std::string& data)
{
	_show_info->setString(StringUtils::format("%s server callback:%s", client->getTag(), data.c_str()));
}

void MultiGame::cleanup()
{
	Layer::cleanup();
	if (_sioclient)
		_sioclient->disconnect();
}