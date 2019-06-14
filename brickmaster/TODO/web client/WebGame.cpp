#include"WebGame.h"
#include "web client/GameClient.h"
GameClient* p_gc=nullptr;
void WebGame::writeScoreToUserData()
{

}

Scene* WebGame::createScene(int level, int time)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = WebGame::create();
	layer->_life = -1;
	layer->_level = level;
	layer->_time = time;
	layer->_totaltime = time;
	layer->islife = false;
	layer->initAfter();
	layer->setTag(100);
	scene->addChild(layer);
	return scene;
}

void WebGame::createHUDforWeb()
{
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	tlife->setVisible(false);
	_showlife->setVisible(false);
	ttime->setPosition(x, y - 250);
	_showtime->setPosition(x, y - 300);

	auto t_oppo_score = Label::createWithTTF("OP Score:", "fonts/BRITANIC.ttf", 36);
	t_oppo_score->setPosition(x, y - 350);
	t_oppo_score->setColor(Color3B::BLUE);
	addChild(t_oppo_score);
	_show_oppo_score = Label::createWithTTF("--", "fonts/BRITANIC.ttf", 36);
	_show_oppo_score->setPosition(x, y - 400);
	_show_oppo_score->setColor(Color3B::BLUE);
	addChild(_show_oppo_score);

	auto t_oppo_time = Label::createWithTTF("OP Time:", "fonts/BRITANIC.ttf", 36);
	t_oppo_time->setPosition(x, y - 450);
	t_oppo_time->setColor(Color3B::BLUE);
	addChild(t_oppo_time);
	_show_oppo_time = Label::createWithTTF("--", "fonts/BRITANIC.ttf", 36);
	_show_oppo_time->setPosition(x, y - 500);
	_show_oppo_time->setColor(Color3B::BLUE);
	addChild(_show_oppo_time);

	setGameClient();

	this->schedule(schedule_selector(WebGame::updateSend), 1.0f/60);

	this->schedule(schedule_selector(WebGame::updateRecv), 1.0f/60);

}

void WebGame::updateSend(float dt)
{
	p_gc->setMyScore(_score);
	p_gc->setMyTime(_time);
}
void WebGame::updateRecv(float dt)
{
	_show_oppo_score->setString(p_gc->getOpScore());
	_show_oppo_time->setString(p_gc->getOpTime());
}
void WebGame::setGameClient()
{
	p_gc = new GameClient();
	startClient(*p_gc);
}
void WebGame::cleanGameClient()
{
	delete p_gc;
	p_gc = nullptr;
}
void WebGame::closeOthers()
{
	cleanGameClient();
}