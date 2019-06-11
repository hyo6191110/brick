#include "MenuStore.h"



void MenuStore::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

bool MenuStore::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initIterator();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background/background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg->setLocalZOrder(-1);
	this->addChild(bg);

	auto closeItem = MenuItemImage::create(
		"ui/CloseNormal.png",
		"ui/CloseSelected.png",
		CC_CALLBACK_1(MenuStore::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto prevbg = MenuItemImage::create(
		"ui/button_next_r.png",
		"ui/button_next_selected_r.png",
		CC_CALLBACK_1(MenuStore::prevBackground, this));
	prevbg->setPosition(visibleSize.width / 4, visibleSize.height*3 / 4);

	auto nextbg = MenuItemImage::create(
		"ui/button_next.png",
		"ui/button_next_selected.png",
		CC_CALLBACK_1(MenuStore::nextBackground, this));
	nextbg->setPosition(visibleSize.width*3 / 4, visibleSize.height*3 / 4);

	auto prevp = MenuItemImage::create(
		"ui/button_next_r.png",
		"ui/button_next_selected_r.png",
		CC_CALLBACK_1(MenuStore::prevPlate, this));
	prevp->setPosition(visibleSize.width / 4, visibleSize.height / 4);

	auto nextp = MenuItemImage::create(
		"ui/button_next.png",
		"ui/button_next_selected.png",
		CC_CALLBACK_1(MenuStore::nextPlate, this));
	nextp->setPosition(visibleSize.width * 3 / 4, visibleSize.height / 4);

	auto savebg = MenuItemImage::create(
		"ui/ok.png",
		"ui/ok_s.png",
		CC_CALLBACK_1(MenuStore::writeBackground, this));
	savebg->setPosition(visibleSize.width * 7 / 8, visibleSize.height*3 / 4);

	auto savep = MenuItemImage::create(
		"ui/ok.png",
		"ui/ok_s.png",
		CC_CALLBACK_1(MenuStore::writePlate, this));
	savep->setPosition(visibleSize.width * 7 / 8, visibleSize.height / 4);

	auto menu = Menu::create(closeItem,prevbg,nextbg,prevp,nextp,savebg,savep, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void MenuStore::initIterator()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	string user_background = UserDefault::getInstance()->getStringForKey("background_texture", "");
	if (user_background != "")
	{
		it_background = find(background_texture.begin(), background_texture.end(), user_background);
	}
	else
	{
		it_background = background_texture.begin();
	}
	_show_background = Sprite::create(*it_background);
	_show_background->setPosition(visibleSize.width / 2, visibleSize.height * 3 / 4);
	_show_background->setScale(0.3f);
	addChild(_show_background);

	string user_plate = UserDefault::getInstance()->getStringForKey("plate_texture", "");
	if (user_plate != "")
	{
		it_plate = find(plate_texture.begin(), plate_texture.end(), user_plate);
	}
	else
	{
		it_plate = plate_texture.begin();
	}
	_show_plate = Sprite::create(*it_plate);
	_show_plate->setPosition(visibleSize.width / 2, visibleSize.height * 1 / 4);
	addChild(_show_plate);
}
void MenuStore::prevBackground(cocos2d::Ref* pSender)
{
	if (it_background != background_texture.begin())
	{
		it_background--;
		auto bg= Sprite::create(*it_background);
		auto bg_tex = bg->getTexture();
		_show_background->setTexture(bg_tex);
	}	
}
void MenuStore::nextBackground(cocos2d::Ref* pSender)
{
	if (it_background != background_texture.end()-1)
	{
		it_background++;
		auto bg = Sprite::create(*it_background);
		auto bg_tex = bg->getTexture();
		_show_background->setTexture(bg_tex);
	}
}
void MenuStore::prevPlate(cocos2d::Ref* pSender)
{
	if (it_plate != plate_texture.begin())
	{
		it_plate--;
		auto p = Sprite::create(*it_plate);
		auto p_tex = p->getTexture();
		_show_plate->setTexture(p_tex);
	}
}
void MenuStore::nextPlate(cocos2d::Ref* pSender)
{
	if (it_plate != plate_texture.end()-1)
	{
		it_plate++;
		auto p = Sprite::create(*it_plate);
		auto p_tex = p->getTexture();
		_show_plate->setTexture(p_tex);
	}
}

void MenuStore::writeBackground(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setStringForKey("background_texture", *it_background);
}
void MenuStore::writePlate(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setStringForKey("plate_texture", *it_plate);
}
