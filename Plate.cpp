#include "Plate.h"
USING_NS_CC;

void Plate::initWithData(float x, float y)
{
	this->setPosition(x, y);

	auto body1 = cocos2d::PhysicsBody::createEdgeBox(this->getContentSize());

	body1->getShapes().at(0)->setRestitution(1);
	body1->getShapes().at(0)->setFriction(0);
	body1->getShapes().at(0)->setDensity(1);

	body1->setGroup(0);
	body1->setCategoryBitmask(0x01);
	body1->setContactTestBitmask(0x01);
	body1->setCollisionBitmask(0x01);

	this->setPhysicsBody(body1);

	
}