#include "Plate.h"
USING_NS_CC;

void Plate::initWithData(float x, float y)
{
	this->setPosition(x, y);
	this->setStartPosition(x);
	auto body1 = cocos2d::PhysicsBody::createBox(this->getContentSize());
	
	body1->getShapes().at(0)->setRestitution(1);
	body1->getShapes().at(0)->setFriction(0);
	body1->getShapes().at(0)->setDensity(15.0);
	body1->setRotationEnable(false);

	body1->setGroup(0);
	body1->setCategoryBitmask(0x01);
	body1->setContactTestBitmask(0x01);
	body1->setCollisionBitmask(0x01);

	this->setPhysicsBody(body1);
}

void Plate::moveLeft()
{
	setStartPosition(getPositionX());
	getPhysicsBody()->setVelocity(Vec2(-1 * getVelocity(), 0));
	setMovingLeft(true);
}
void Plate::stopLeft()
{
	if (isMovingRight())
		getPhysicsBody()->setVelocity(Vec2(getVelocity(), 0));
	else
		getPhysicsBody()->setVelocity(Vec2::ZERO);
	setStartPosition(getPositionX());
	setMovingLeft(false);
}
void Plate::moveRight()
{
	setStartPosition(getPositionX());
	getPhysicsBody()->setVelocity(Vec2(getVelocity(), 0));
	setMovingRight(true);
}
void Plate::stopRight()
{
	if (isMovingLeft())
		getPhysicsBody()->setVelocity(Vec2(-1 * getVelocity(), 0));
	else
		getPhysicsBody()->setVelocity(Vec2::ZERO);
	setStartPosition(getPositionX());
	setMovingRight(false);
}

void Deadzone::initWithData(float x, float y)
{
	this->setPosition(x, y);
	auto body_1 = PhysicsBody::createEdgeBox(this->getContentSize());
	body_1->setCategoryBitmask(0x01);
	body_1->setContactTestBitmask(0x01);
	body_1->setCollisionBitmask(0x01);
	this->setPhysicsBody(body_1);
}