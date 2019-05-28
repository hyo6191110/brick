#include"GameModePong.h"
using namespace cocos2d;

Scene*  GameModePong::createScenePong(int life, int level)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameModePong::create();
	layer->_life = life;
	layer->_level = level;
	layer->_player1->setLife(life);
	layer->_player_opposite->setLife(life);
	layer->initAfter();
	scene->addChild(layer);
	return scene;
}

void GameModePong::initMode()
{
	createPlayerSide(2, _player_opposite, _deadzone_opposite);
}
void GameModePong::updateMode()
{
	collisionDetection(_player_opposite);
	if (balltodestroy)
	{
		auto player = balltodestroy->getPlateBelongto();
		destroyball(balltodestroy);
		player->damaged();
		_showlife->setString(StringUtils::format("%d", _player1->getLife()));
		_showlife_opposite->setString(StringUtils::format("%d", _player_opposite->getLife()));

		if (_player1->getLife() == 0)
			gameOver(false);
		else if (_player_opposite->getLife() == 0)
			gameOver(true);
		
		if (V_balls.empty())
		{
			createballforPlate(_player1);
			isRoundStarted = false;
		}
		balltodestroy = nullptr;
	}
}
void GameModePong::createHUDMode()
{
	float x = _gamescreen.getMinX() / 2;
	float y = _gamescreen.getMaxY();

	auto tlife = Label::createWithTTF("Opposite Life:", "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::LEFT);
	auto str = StringUtils::format("%d", _life);
	_showlife_opposite = Label::createWithTTF(str, "fonts\\COLONNA.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tlife->setPosition(x, y - 600);
	_showlife_opposite->setPosition(x, y - 700);
	this->addChild(tlife);
	this->addChild(_showlife_opposite);
}