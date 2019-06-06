#include "LevelDesigner.h"
USING_NS_CC;
using namespace rapidjson;
using namespace std;
Scene* LevelDesigner::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelDesigner::create();
	scene->addChild(layer);
	return scene;
}

void LevelDesigner::Closethis(Ref* pSender)
{
	//this->unscheduleUpdate();
	SpriteFrameCache::getInstance()->removeSpriteFrames();
	//SpriteFrameCache::getInstance->destroyInstance();
	//this->_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->popScene();

}

void LevelDesigner::addbrick(int type, float x, float y)
{
	auto brick = Brick::create(StringUtils::format("d_brick_%d.png", type));
	brick->settype(type);
	brick->setPosition(x, y);
	this->addChild(brick);
	vec_brick.push_back(brick);
}

void LevelDesigner::WritetoFile(cocos2d::Ref* pSender)
{
	bool rewrite=false;
	if (FileUtils::getInstance()->isFileExist("D:/1/cocos_project/brickmaster/Resources/levels/user_levels/"+_fileName))
	{
		_showsavelog->setString("Rewrite:"+_fileName);
		rewrite = true;
	}
	Document document;
	document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();
	Document::ValueType json_array(kArrayType);
	for (auto it : vec_brick)
	{
		Document::ValueType json_obj(kObjectType);
		json_obj.AddMember("type", it->gettype(), allocator);
		json_obj.AddMember("x", static_cast<int>(it->getPositionX()), allocator);
		json_obj.AddMember("y", static_cast<int>(it->getPositionY()), allocator);
		json_array.PushBack(json_obj, allocator);
	}
	document.AddMember("bricks", json_array, allocator);
	StringBuffer buffer;
	PrettyWriter<StringBuffer> pretty_writer(buffer);
	document.Accept(pretty_writer);
	ofstream outfile("D:/1/cocos_project/brickmaster/Resources/levels/user_levels/"+ _fileName);
	outfile << buffer.GetString();
	outfile.close();
	if(rewrite==false)
	    _showsavelog->setString("Successfully saved");
	else
		_showsavelog->setString("Successfully rewrited");
}
void LevelDesigner::ReadfromFile(cocos2d::Ref* pSender)
{
	if (!FileUtils::getInstance()->isFileExist("D:/1/cocos_project/brickmaster/Resources/levels/user_levels/" + _fileName))
	{
		_showsavelog->setString("No file,try another name");
		return;
	}
	for (auto it : vec_brick)
	{
		this->removeChild(it);
	}
	vec_brick.clear();

	vector<BrickData*> vec_data;
	getAllBrickWithFile("levels/user_levels/"+_fileName,vec_data);
	for (auto it : vec_data)
	{
		addbrick(it->getType(),it->getX(),it->getY());
	}
	_showsavelog->setString("Successfully read");
}

bool LevelDesigner::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("designer/design_bricks.plist", "designer/design_bricks.png");
	auto info = Sprite::create("game\\status_board.png");
	info->setPosition(info->getContentSize().width / 2, visibleSize.height / 2);
	this->addChild(info);
	_designscreenWidth = visibleSize.width - info->getContentSize().width;
	_designscreenHeight = visibleSize.height-200;
	_centerX = info->getContentSize().width + _designscreenWidth / 2;
	_centerY = visibleSize.height / 2;

	auto bg = Sprite::create("designer\\design_back.png");
	auto area = Sprite::create("designer\\design_area.png");
	bg->setPosition(_centerX, _centerY);
	area->setPosition(_centerX, _centerY);
	bg->setLocalZOrder(-5);
	this->addChild(bg);
	this->addChild(area);
	_designarea = area->getBoundingBox();

	_fileName = StringUtils::format("level_%d.json", _levelNum);
	_showfileName = Label::createWithTTF(StringUtils::format("level_%d", _levelNum), "fonts\\BRITANIC.ttf", 36, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showfileName->setPosition(info->getContentSize().width / 2, 300);
	this->addChild(_showfileName);
	_showsavelog= Label::createWithTTF("(No File Saved)", "fonts\\BRITANIC.ttf", 24, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	_showsavelog->setPosition(info->getContentSize().width / 2, 75);
	this->addChild(_showsavelog);

	auto add= Label::createWithTTF("+", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto addItem = MenuItemLabel::create(add, [=](Ref* pSender){
		_levelNum++;
		_fileName = StringUtils::format("level_%d.json", _levelNum);
		_showfileName->setString(StringUtils::format("level_%d", _levelNum));
	});
	addItem->setPosition(250, 300);
	auto minus = Label::createWithTTF("-", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto minusItem = MenuItemLabel::create(minus, [=](Ref* pSender) {
		if (_levelNum == 1)return;
		_levelNum--;
		_fileName = StringUtils::format("level_%d.json", _levelNum);
		_showfileName->setString(StringUtils::format("level_%d", _levelNum));
	});
	minusItem->setPosition(50, 300);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(LevelDesigner::Closethis, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));

	auto loadlable = Label::createWithTTF("Load", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto loadItem = MenuItemLabel::create(loadlable,
		CC_CALLBACK_1(LevelDesigner::ReadfromFile, this));
	loadItem->setPosition(info->getContentSize().width / 2, 225);

	auto savelable= Label::createWithTTF("Save", "fonts\\BRITANIC.ttf", 48, Size::ZERO, cocos2d::TextHAlignment::CENTER);
	auto saveItem = MenuItemLabel::create(savelable,
		CC_CALLBACK_1(LevelDesigner::WritetoFile,this));
	saveItem->setPosition(info->getContentSize().width/2,150);

	auto menu = Menu::create(closeItem,loadItem,saveItem,addItem,minusItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	float starterX = 50 ;
	float starterY = visibleSize.height - 80 ;
	int iToTag[7] = { 1,2,3,5,8,51,52 };
	for (int i = 0; i < 7; i++)
	{
		float x = starterX + (i % 4)*_typegapWidth;
		float y = starterY - (i / 4)*_typegapHeight;
		int tag = iToTag[i];
		auto sprite = Sprite::create(StringUtils::format("designer\\button_%d.png", tag));
		sprite->setTag(tag);
		sprite->setPosition(x, y);
		this->addChild(sprite);
	}
	starterX = info->getContentSize().width / 2 - _funcgapWidth;
	starterY = _centerY;
	for (int i = 0; i < 4; i++)
	{
		float x = starterX + (i % 3)*_funcgapWidth;
		float y = starterY - (i / 3)*_funcgapHeight;
		int tag = i+101;
		auto sprite = Sprite::create(StringUtils::format("designer\\button_%d.png", tag));
		sprite->setTag(tag);
		sprite->setPosition(x, y);
		this->addChild(sprite);
	}
	_selectframe1 = Sprite::create("designer\\select_1.png");
	_selectframe2 = Sprite::create("designer\\select_2.png");
	_selectframe1->setPosition(this->getChildByTag(currenttag)->getPosition());
	_selectframe2->setPosition(this->getChildByTag(101)->getPosition());
	this->addChild(_selectframe1, 2);
	this->addChild(_selectframe2, -2);

	for(int i=-7;i<8;i++)
		for (int j = -8; j < 9; j++)
		{
			auto frame = Sprite::create("designer\\brick_frame.png");
			float rx = _centerX + i * frame->getContentSize().width;
			float ry = _centerY + j * frame->getContentSize().height;
			frame->setPosition(rx, ry);
			frame->setLocalZOrder(-2);
			_brickframes.push_back(frame);
			this->addChild(frame);
		}

	auto listener_func = EventListenerTouchOneByOne::create();
	listener_func->onTouchBegan = CC_CALLBACK_2(LevelDesigner::FuncButtonTouchBegan, this);
	auto listener_screen = EventListenerTouchOneByOne::create();
	listener_screen->onTouchBegan = CC_CALLBACK_2(LevelDesigner::ScreenTouchBegan, this);
	listener_screen->onTouchMoved = CC_CALLBACK_2(LevelDesigner::ScreenTouchMoved, this);
	listener_screen->onTouchEnded = CC_CALLBACK_2(LevelDesigner::ScreenTouchEnd, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_func, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_screen, this);
	return true;

}

bool LevelDesigner::FuncButtonTouchBegan(Touch* touch, Event* event)
{
	Vec2 nodeLoca = this->convertToNodeSpace(touch->getLocation());
	int iToTag[11] = { 1, 2, 3, 5, 8, 51, 52, 101, 102, 103, 104};
	for (size_t i = 0; i < 11; i++)
	{
		int tag = iToTag[i];
		if (this->getChildByTag(tag)->getBoundingBox().containsPoint(nodeLoca))
		{
			if (tag < 100)
			{
				if (!iscurrenttype)
				{
					_selectframe2->setLocalZOrder(-2);
					_selectframe1->setPosition(this->getChildByTag(tag)->getPosition());
					_selectframe1->setLocalZOrder(2);
					iscurrenttype = true;
					currenttag = tag;
				}
				else
				{
					_selectframe1->setPosition(this->getChildByTag(tag)->getPosition());
					currenttag = tag;
				}
			}
			else if (tag >= 100)
			{
				if (iscurrenttype)
				{
					_selectframe1->setLocalZOrder(-2);
					_selectframe2->setPosition(this->getChildByTag(tag)->getPosition());
					_selectframe2->setLocalZOrder(2);
					iscurrenttype = false;
					currenttag = tag;
				}
				else
				{
					_selectframe2->setPosition(this->getChildByTag(tag)->getPosition());
					currenttag = tag;
				}

				if (currenttag == 101)
				{
					auto format = this->getChildByTag(101);
					if (isformatmode == false)
					{
						isformatmode = true;
						for (auto it : _brickframes)
						{
							it->setLocalZOrder(2);
						}
					}
					else
					{
						isformatmode = false;
						for (auto it : _brickframes)
						{
							it->setLocalZOrder(-2);
						}
					}
					
				}

				if (tag == 104)
				{
					for (auto it : vec_brick)
					{
						this->removeChild(it);
					}
					vec_brick.clear();
				}
			}
		}
	}
	return false;
}
bool LevelDesigner::ScreenTouchBegan(Touch* touch, Event* event)
{
	Vec2 nodeLoca = this->convertToNodeSpace(touch->getLocation());
	switch (currenttag)
	{
	case 101:
	{
		break;
	}
	case 102:
	{
		for (auto it=vec_brick.begin();it!=vec_brick.end();it++)
		{
			if ((*it)->getBoundingBox().containsPoint(nodeLoca))
			{
				this->removeChild(*it);
				vec_brick.erase(it);
				break;
			}
		}
		break;
	}
	case 103:
	{
		return true;
	}
	default:
	{
		if (isformatmode)
		{
			for (auto it : _brickframes)
			{
				if (it->getBoundingBox().containsPoint(nodeLoca))
				{
					bool isintersects = false;
					for (auto ia : vec_brick)
					{
						if (ia->getPosition() == it->getPosition())
							isintersects = true;
					}
					if (isintersects == true)
						break;
					addbrick(currenttag, it->getPositionX(), it->getPositionY());
					break;
				}
			}
		}
		else
		{
			Rect brick_rect(nodeLoca.x - 40, nodeLoca.y - 20, 80, 40);
			if (_designarea.getMinX() <= brick_rect.getMinX() &&
				_designarea.getMinY() <= brick_rect.getMinY() &&
				_designarea.getMaxX() >= brick_rect.getMaxX() &&
				_designarea.getMaxY() >= brick_rect.getMaxY())
			{
				bool isintersects = false;
				for (auto brick : vec_brick)
				{
					if (brick->getBoundingBox().intersectsRect(brick_rect))
					{
						isintersects = true;
						break;
					}
				}
				if (isintersects == false)
					addbrick(currenttag, nodeLoca.x, nodeLoca.y);
			}

		}

			
		
		break;
	}
	}
	return false;
}
void LevelDesigner::ScreenTouchMoved(Touch* touch, Event* event)
{
	Vec2 nodeLoca = this->convertToNodeSpace(touch->getLocation());
	if(!isformatmode)
	for (auto it : vec_brick)
	{
		if (it->getBoundingBox().containsPoint(nodeLoca))
		{
			bool isintersects = false;
			Rect rect(it->getBoundingBox().getMinX() + touch->getDelta().x,
				it->getBoundingBox().getMinY() + touch->getDelta().y,
				it->getContentSize().width, it->getContentSize().height);
			if (_designarea.getMinX() > rect.getMinX() ||
				_designarea.getMinY() > rect.getMinY() ||
				_designarea.getMaxX() < rect.getMaxX() ||
				_designarea.getMaxY() < rect.getMaxY())
				break;
			for (auto brick : vec_brick)
			{
				if (it == brick)
					continue;
				if (brick->getBoundingBox().intersectsRect(rect))
				{
					isintersects = true;
					break;
				}
			}
			if(isintersects==false)
				it->setPosition(it->getPosition() + touch->getDelta());
			break;
		}
	}
}
void LevelDesigner::ScreenTouchEnd(Touch* touch, Event* event)
{

}