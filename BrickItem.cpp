#include "BrickItem.h"
USING_NS_CC;

const int BrickItem::Max_ItemTypes = (int)(Points)+1;

void BrickItem::initWithData(float x, float y)
{
	auto body1 = PhysicsBody::createEdgeBox(this->getContentSize());
	body1->setGroup(1);
	body1->setContactTestBitmask(0x01);
	body1->setCategoryBitmask(0x00);
	body1->setCollisionBitmask(0x00);
	this->setPhysicsBody(body1);
	this->setTag(5);
	int seed = random() + (int)(x + y);
	this->type = (ItemType)(seed%Max_ItemTypes);

}