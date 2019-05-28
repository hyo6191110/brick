#include "BrickItem.h"
USING_NS_CC;

const int BrickItem::Max_ItemTypes = (int)(Points)+1;

void BrickItem::initWithData(float x, float y)
{
	this->setPosition(x, y);
	this->setTag(5);
}