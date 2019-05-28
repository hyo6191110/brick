#include"GameModeTimelimit.h"
using namespace cocos2d;

Scene*  GameModeTimelimit::createSceneTimelimit(int life, int level)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameModeTimelimit::create();
	layer->_life = life;
	layer->_level = level;
	layer->_player1->setLife(life);
	layer->initAfter();
	scene->addChild(layer);
	return scene;
}

void GameModeTimelimit::initMode()
{

}
void GameModeTimelimit::updateMode()
{
	if (balltodestroy)
	{
		auto player = balltodestroy->getPlateBelongto();
		destroyball(balltodestroy);
		if (V_balls.empty())
		{
			player->damaged();
			_showlife->setString(StringUtils::format("%d", _player1->getLife()));
			if (_player1->getLife() == 0)
			{
				gameOver(false);
			}
			else
			{
				createballforPlate(_player1);
				isRoundStarted = false;
			}

		}
		balltodestroy = nullptr;
	}
	if (brickcount == 0 && isRoundStarted == true)
	{
		for (auto dball : V_balls)
		{
			destroyball(dball);
		}
		gameOver(true);
	}
}
void GameModeTimelimit::createHUDMode()
{

}