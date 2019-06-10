#include "MenuSettings.h"


void MenuSettings::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

bool MenuSettings::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MenuSettings::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto setting = Label::createWithTTF("Settings", "fonts/BOD_R.ttf", 96);
	setting->setPosition(visibleSize.width / 2, 800);
	addChild(setting);

	auto bgm = Label::createWithTTF("BGM", "fonts/BOD_R.ttf", 48);
	bgm->setPosition(100, 700);
	addChild(bgm);
	auto bgm_checkbox = CheckBox::create(
		"ui/check_box_normal.png", 
		"ui/check_box_normal_press.png", 
		"ui/check_box_active.png", 
		"ui/check_box_normal_disable.png",
		"ui/check_box_active_disable.png");
	bgm_checkbox->addEventListener(CC_CALLBACK_2(MenuSettings::bgmCheckBox, this));
	bgm_checkbox->setPosition(Vec2(250, 700));
	addChild(bgm_checkbox);
	_bgm_slider = Slider::create();
	_bgm_slider->loadBarTexture("ui/sliderTrack.png");
	_bgm_slider->loadSlidBallTextures("ui/sliderThumb.png");
	_bgm_slider->loadProgressBarTexture("ui/sliderProgress.png");
	_bgm_slider->addEventListener(CC_CALLBACK_2(MenuSettings::bgmSlideBar, this));
	_bgm_slider->setPosition(Vec2(500, 700));
	addChild(_bgm_slider);
	_show_bgm_percent= Label::createWithTTF("", "fonts/BOD_R.ttf", 48);
	_show_bgm_percent->setPosition(750, 700);
	addChild(_show_bgm_percent);

	auto effect = Label::createWithTTF("Effect", "fonts/BOD_R.ttf", 48);
	effect->setPosition(100, 600);
	addChild(effect);
	auto effect_checkbox = CheckBox::create(
		"ui/check_box_normal.png",
		"ui/check_box_normal_press.png",
		"ui/check_box_active.png",
		"ui/check_box_normal_disable.png",
		"ui/check_box_active_disable.png");
	effect_checkbox->addEventListener(CC_CALLBACK_2(MenuSettings::effectCheckBox, this));
	effect_checkbox->setPosition(Vec2(250, 600));
	addChild(effect_checkbox);
	_effect_slider = Slider::create();
	_effect_slider->loadBarTexture("ui/sliderTrack.png");
	_effect_slider->loadSlidBallTextures("ui/sliderThumb.png");
	_effect_slider->loadProgressBarTexture("ui/sliderProgress.png");
	_effect_slider->addEventListener(CC_CALLBACK_2(MenuSettings::effectSlideBar, this));
	_effect_slider->setPosition(Vec2(500, 600));
	addChild(_effect_slider);
	_show_effect_percent = Label::createWithTTF("", "fonts/BOD_R.ttf", 48);
	_show_effect_percent->setPosition(750, 600);
	addChild(_show_effect_percent);

	auto label1= Label::createWithTTF("NormalBrickDropItem", "fonts/BOD_R.ttf", 48);
	label1->setPosition(280, 500);
	addChild(label1);
	auto checkbox = CheckBox::create(
		"ui/check_box_normal.png",
		"ui/check_box_normal_press.png",
		"ui/check_box_active.png",
		"ui/check_box_normal_disable.png",
		"ui/check_box_active_disable.png");
	checkbox->addEventListener(CC_CALLBACK_2(MenuSettings::ItemCheckBox, this));
	checkbox->setPosition(Vec2(580, 500));
	addChild(checkbox);


	bgm_checkbox->setSelected(UserDefault::getInstance()->getBoolForKey("bgm_switch", false));
	effect_checkbox->setSelected(UserDefault::getInstance()->getBoolForKey("effect_switch", false));
	checkbox->setSelected(UserDefault::getInstance()->getBoolForKey("normal_brick_dropitem", false));
	_bgm_slider->setPercent(static_cast<int>(UserDefault::getInstance()->getFloatForKey("bgm_volumn", 0.0f) * 100));
	_show_bgm_percent->setString(StringUtils::format("%d", static_cast<int>(UserDefault::getInstance()->getFloatForKey("bgm_volumn", 0.0f) * 100)));
	_effect_slider->setPercent(static_cast<int>(UserDefault::getInstance()->getFloatForKey("effect_volumn", 0.0f) * 100));
	_show_effect_percent->setString(StringUtils::format("%d", static_cast<int>(UserDefault::getInstance()->getFloatForKey("effect_volumn", 0.0f) * 100)));
	return true;
}
void MenuSettings::bgmCheckBox(cocos2d::Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("bgm_switch", true);
		break;
	}
	case CheckBox::EventType::UNSELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("bgm_switch", false);
		break;
	}
	}
}
void MenuSettings::effectCheckBox(cocos2d::Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("effect_switch", true);
		break;
	}
	case CheckBox::EventType::UNSELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("effect_switch", false);
		break;
	}
	}
}
void MenuSettings::bgmSlideBar(cocos2d::Ref* pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		_show_bgm_percent->setString(StringUtils::format("%d", _bgm_slider->getPercent()));
		UserDefault::getInstance()->setFloatForKey("bgm_volumn", static_cast<float>(_bgm_slider->getPercent()) / 100);
	}
}
void MenuSettings::effectSlideBar(cocos2d::Ref* pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		_show_effect_percent->setString(StringUtils::format("%d", _effect_slider->getPercent()));
		UserDefault::getInstance()->setFloatForKey("effect_volumn", static_cast<float>(_effect_slider->getPercent()) / 100);
	}
}
void MenuSettings::ItemCheckBox(cocos2d::Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("normal_brick_dropitem", true);
		break;
	}
	case CheckBox::EventType::UNSELECTED:
	{
		UserDefault::getInstance()->setBoolForKey("normal_brick_dropitem", false);
		break;
	}
	}
}