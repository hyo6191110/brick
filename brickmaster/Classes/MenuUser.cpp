#include "MenuUser.h"
USING_NS_CC;


void MenuUser::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	//SpriteFrameCache::getInstance->removeUnusedSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

bool MenuUser::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background/background_menu_02.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	initData();
	setclassic();
	_showmode = Label::createWithTTF("Classic", "fonts/BOD_R.ttf", 48);
	_showmode->setPosition(300, 800);
	addChild(_showmode);

	for (int i = 0; i < 3; i++)
	{
		auto button1 = Sprite::create("ui/frame.png");
		button1->setPosition(1350, 680 - 300 * i);
		addChild(button1);
	}
	
	auto clearclassic = MenuItemLabel::create(Label::createWithTTF("ClearClassic","fonts/BRITANIC.ttf",24), 
		CC_CALLBACK_1(MenuUser::clearClassic, this));
	clearclassic->setPosition(1350, 680);
	auto clearpong = MenuItemLabel::create(Label::createWithTTF("ClearPong", "fonts/BRITANIC.ttf", 24),
		CC_CALLBACK_1(MenuUser::clearPong, this));
	clearpong->setPosition(1350, 380);
	auto cleartimelimit = MenuItemLabel::create(Label::createWithTTF("ClearTimelimit", "fonts/BRITANIC.ttf", 24),
		CC_CALLBACK_1(MenuUser::clearTimelimit, this));
	cleartimelimit->setPosition(1350, 80);

	auto modeprev = MenuItemImage::create("ui/button_next_r.png", "ui/button_next_selected_r.png", CC_CALLBACK_1(MenuUser::prevMode, this));
	modeprev->setPosition(50, 800);
	auto modenext = MenuItemImage::create("ui/button_next.png", "ui/button_next_selected.png", CC_CALLBACK_1(MenuUser::nextMode, this));
	modenext->setPosition(550, 800);

	auto closeItem = MenuItemImage::create(
		"ui/CloseNormal.png",
		"ui/CloseSelected.png",
		CC_CALLBACK_1(MenuUser::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(closeItem,clearclassic,clearpong,cleartimelimit,modeprev,modenext, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	int time = UserDefault::getInstance()->getIntegerForKey("classic_played", 0);
	int average = UserDefault::getInstance()->getIntegerForKey("classic_average", 0);
	int max = UserDefault::getInstance()->getIntegerForKey("classic_maxscore", 0);
	auto showclassic = Label::createWithTTF(StringUtils::format("Classic Mode: player %d times\n average %d points\n max totalscore %d",time,average,max), "fonts/BOD_R.ttf", 48);
	showclassic->setPosition(1000, 800);
	time = UserDefault::getInstance()->getIntegerForKey("pong_played", 0);
	average = UserDefault::getInstance()->getIntegerForKey("pong_average", 0);
	int wintime = UserDefault::getInstance()->getIntegerForKey("pong_win", 0);
	max = UserDefault::getInstance()->getIntegerForKey("pong_maxscore", 0);
	auto showpong = Label::createWithTTF(StringUtils::format("Pong Mode: player %d times\n average %d points\n win %d times; lose %d times\n max totalscore %d", time, average, wintime,time-wintime ,max), "fonts/BOD_R.ttf", 48);
	showpong->setPosition(1000, 500);
	time = UserDefault::getInstance()->getIntegerForKey("timelimit_played", 0);
	average = UserDefault::getInstance()->getIntegerForKey("timelimit_average", 0);
	max = UserDefault::getInstance()->getIntegerForKey("timelimit_maxscore", 0);
	auto showtimelimit = Label::createWithTTF(StringUtils::format("Timelimit Mode: player %d times\n average %d points\n max totalscore %d", time, average, max), "fonts/BOD_R.ttf", 48);
	showtimelimit->setPosition(1050, 200);
	addChild(showclassic);
	addChild(showpong);
	addChild(showtimelimit);
	return true;
}

void MenuUser::initData()
{
	bool flag_classic = true;
	bool flag_pong = true;
	bool flag_timelimit = true;
	for (int i = 0; i < 10; i++)
	{
		if (flag_classic)
		{
			classic_data temp_c;
			temp_c.score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("classic_score_%d", i).c_str(), -1);
			if (temp_c.score != -1)
			{
				temp_c.brick = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("classic_brick_%d", i).c_str(), -1);
				temp_c.level = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("classic_level_%d", i).c_str(), -1);
				_v_classic.push_back(temp_c);
			}
			else
			{
				flag_classic = false;
			}
			
		}
		if (flag_pong)
		{
			pong_data temp_p;
			temp_p.score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("pong_score_%d", i).c_str(), -1);
			if (temp_p.score != -1)
			{
				temp_p.my_goal = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("pong_mygoal_%d", i).c_str(), -1);
				temp_p.op_goal = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("pong_opgoal_%d", i).c_str(), -1);
				temp_p.level = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("pong_level_%d", i).c_str(), -1);
				_v_pong.push_back(temp_p);
			}
			else
			{
				flag_pong = false;
			}

		}
		if (flag_timelimit)
		{
			timelimit_data temp_t;
			temp_t.score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("timelimit_score_%d", i).c_str(), -1);
			if (temp_t.score != -1)
			{
				temp_t.time = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("timelimit_time_%d", i).c_str(), -1);
				temp_t.level = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("timelimit_level_%d", i).c_str(), -1);
				_v_timelimit.push_back(temp_t);
			}
			else
			{
				flag_timelimit = false;
			}

		}
	}
	float start_x=100;
	float start_y=700;
	float gap_x=200;
	float gap_y=50;
	for (int i = 0; i < 11; i++)
	{
		string score("--");
		string special("--");
		string level("--");
		if (i == 0)
		{
			score = "Score";
			special = "Special";
			level = "Level";
		}
		auto showscore = Label::createWithTTF(score, "fonts/BOD_R.ttf", 48);
		showscore->setPosition(start_x + gap_x*0, start_y - gap_y * i);
		addChild(showscore);
		_v_showscore.push_back(showscore);

		auto showspecial = Label::createWithTTF(special, "fonts/BOD_R.ttf", 48);
		showspecial->setPosition(start_x + gap_x*1, start_y - gap_y * i);
		addChild(showspecial);
		_v_showspecial.push_back(showspecial);

		auto showlevel = Label::createWithTTF(level, "fonts/BOD_R.ttf", 48);
		showlevel->setPosition(start_x + gap_x*2, start_y - gap_y * i);
		addChild(showlevel);
		_v_showlevel.push_back(showlevel);

	}
}
void MenuUser::prevMode(cocos2d::Ref* pSender)
{
	if (_current_gamemode != 1)
	{
		_current_gamemode--;
		switch (_current_gamemode)
		{
		case 1:
		{
			_showmode->setString("Classic");
			setclassic();
			break;
		}
		case 2:
		{
			_showmode->setString("Pong");
			setpong();
			break;
		}
		case 3:
		{
			_showmode->setString("Timelimit");
			settimelimit();
			break;
		}
		}
	}
}
void MenuUser::nextMode(cocos2d::Ref* pSender)
{
	if (_current_gamemode != 3)
	{
		_current_gamemode++;
		switch (_current_gamemode)
		{
		case 1:
		{
			_showmode->setString("Classic");
			setclassic();
			break;
		}
		case 2:
		{
			_showmode->setString("Pong");
			setpong();
			break;
		}
		case 3:
		{
			_showmode->setString("Timelimit");
			settimelimit();
			break;
		}
		}
	}
}
void MenuUser::setclassic()
{
	_v_showspecial.at(0)->setString("Brick");
	for (int i = 1; i < 11; i++)
	{
		_v_showscore.at(i)->setString("--");
		_v_showspecial.at(i)->setString("--");
		_v_showlevel.at(i)->setString("--");
	}
	for (size_t i = 0; i < _v_classic.size(); i++)
	{
		size_t j = i + 1;
		_v_showscore.at(j)->setString(StringUtils::format("%d", _v_classic.at(i).score));
		_v_showspecial.at(j)->setString(StringUtils::format("%d", _v_classic.at(i).brick));
		if(_v_classic.at(i).level>10000)
			_v_showlevel.at(j)->setString(StringUtils::format("User-%d", _v_classic.at(i).level-10000));
		else
			_v_showlevel.at(j)->setString(StringUtils::format("%d", _v_classic.at(i).level));
		
	}
}
void MenuUser::setpong()
{
	_v_showspecial.at(0)->setString("ME:OP");
	for (int i = 1; i < 11; i++)
	{
		_v_showscore.at(i)->setString("--");
		_v_showspecial.at(i)->setString("--");
		_v_showlevel.at(i)->setString("--");
	}
	for (size_t i = 0; i < _v_pong.size(); i++)
	{
		size_t j = i + 1;
		_v_showscore.at(j)->setString(StringUtils::format("%d", _v_pong.at(i).score));
		_v_showspecial.at(j)->setString(StringUtils::format("%d:%d", _v_pong.at(i).my_goal, _v_pong.at(i).op_goal));
		if (_v_pong.at(i).level > 10000)
			_v_showlevel.at(j)->setString(StringUtils::format("User-%d", _v_pong.at(i).level - 10000));
		else
			_v_showlevel.at(j)->setString(StringUtils::format("%d", _v_pong.at(i).level));
	}
}
void MenuUser::settimelimit()
{
	_v_showspecial.at(0)->setString("Time");
	for (int i = 1; i < 11; i++)
	{
		_v_showscore.at(i)->setString("--");
		_v_showspecial.at(i)->setString("--");
		_v_showlevel.at(i)->setString("--");
	}
	for (size_t i = 0; i < _v_timelimit.size(); i++)
	{
		size_t j = i + 1;
		_v_showscore.at(j)->setString(StringUtils::format("%d", _v_timelimit.at(i).score));
		_v_showspecial.at(j)->setString(StringUtils::format("%d", _v_timelimit.at(i).time));
		if (_v_timelimit.at(i).level > 10000)
			_v_showlevel.at(j)->setString(StringUtils::format("User-%d", _v_timelimit.at(i).level - 10000));
		else
			_v_showlevel.at(j)->setString(StringUtils::format("%d", _v_timelimit.at(i).level));
	}
}
void MenuUser::clearClassic(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setIntegerForKey("classic_played", 0);
	UserDefault::getInstance()->setIntegerForKey("classic_average", 0);
	UserDefault::getInstance()->setIntegerForKey("classic_maxscore", 0);
	for (int i = 0; i < 10; i++)
	{
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("classic_score_%d",i).c_str(), -1);
	}
}
void MenuUser::clearPong(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setIntegerForKey("pong_played", 0);
	UserDefault::getInstance()->setIntegerForKey("pong_average", 0);
	UserDefault::getInstance()->setIntegerForKey("pong_win", 0);
	UserDefault::getInstance()->setIntegerForKey("pong_maxscore", 0);
	for (int i = 0; i < 10; i++)
	{
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("pong_score_%d", i).c_str(), -1);
	}
}
void MenuUser::clearTimelimit(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setIntegerForKey("timelimit_played", 0);
	UserDefault::getInstance()->setIntegerForKey("timelimit_average", 0);
	UserDefault::getInstance()->setIntegerForKey("timelimit_maxscore", 0);
	for (int i = 0; i < 10; i++)
	{
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("timelimit_score_%d", i).c_str(), -1);
	}
}