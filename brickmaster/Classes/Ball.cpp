#include "Ball.h"
USING_NS_CC;

void Ball::initWithData(float x, float y)
{
	this->setPosition(x, y);

	auto body_ball = cocos2d::PhysicsBody::createCircle(this->getContentSize().width / 2);
	
	body_ball->getShapes().at(0)->setRestitution(1);
	body_ball->getShapes().at(0)->setFriction(0);
	body_ball->getShapes().at(0)->setDensity(0.1f);

	body_ball->setGroup(0);
	body_ball->setCategoryBitmask(0x01);
	body_ball->setContactTestBitmask(0x01);
	body_ball->setCollisionBitmask(0x01);
	
	this->setPhysicsBody(body_ball);
	

}

void  Ball::shootBall(Plate* shooter)
{
	if (!getPhysicsBody()->isDynamic())
	if (shooter == _plate_belongto)
	{
		float verti_velo = _vertical_velo;
		if (shooter->getPositionY() > 500)
		   verti_velo *= -1;

		float hori_velo = (shooter->getPositionX() - shooter->getStartPosition());
		if (hori_velo < 0)
		{
			hori_velo -= random() % 100;
		}
		else if (hori_velo > 0)
		{
			hori_velo += random() % 100;
		}
		else
		{
			hori_velo += random() % 300;
			hori_velo -= random() % 300;
		}

		Vec2 velocity(hori_velo, _vertical_velo);
		velocity.normalize();
		velocity *= _speed;
		this->getPhysicsBody()->setDynamic(true);
		this->getPhysicsBody()->setVelocity(velocity);
	}
}