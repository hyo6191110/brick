#include "Ball.h"
USING_NS_CC;

void Ball::initWithData(float x, float y)
{
	this->setPosition(x, y);

	auto body_ball = cocos2d::PhysicsBody::createCircle(this->getContentSize().width / 2);
	
	body_ball->getShapes().at(0)->setRestitution(1);
	body_ball->getShapes().at(0)->setFriction(0);
	body_ball->getShapes().at(0)->setDensity(1);

	body_ball->setGroup(0);
	body_ball->setCategoryBitmask(0x01);
	body_ball->setContactTestBitmask(0x01);
	body_ball->setCollisionBitmask(0x01);
	
	this->setPhysicsBody(body_ball);
	

}