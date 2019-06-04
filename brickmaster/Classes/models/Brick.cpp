#include "Brick.h"
USING_NS_CC;

void Brick::initWithData(float x, float y )
{
	
	auto brick = this;

	auto body1 = PhysicsBody::createEdgeBox(brick->getContentSize());
	body1->getShapes().at(0)->setRestitution(1);
	body1->setGroup(1);
	body1->setContactTestBitmask(0x01);
	body1->setCategoryBitmask(0x01);
	body1->setCollisionBitmask(0x01);
	brick->setPhysicsBody(body1);

	brick->setPosition(x,y);
	brick->setTag(2);
	_max_hp = _hp;
}

void Brick::damaged()
{
	_hp -= 1;
	if (_hp > 0)
	{
		setSpriteFrame(StringUtils::format("brick_%d.png", _hp));
	}
}