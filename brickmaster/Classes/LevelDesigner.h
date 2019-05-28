#ifndef __DESIGNER_H__
#define __DESIGNER_H__

#include "cocos2d.h"

#include"json/writer.h"
#include"json/rapidjson.h"
#include"json/document.h"
#include"json/filewritestream.h"
#include"json/prettywriter.h"
#include"json/stringbuffer.h"

#include"Brick.h"

class LevelDesigner :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(LevelDesigner);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void WritetoFile(cocos2d::Ref* pSender);
private:
	float _designscreenWidth, _designscreenHeight;
	float _centerX;
	float _centerY;
	cocos2d::Rect _designarea;
	float _typegapWidth=70, _typegapHeight=80;
	float _funcgapWidth=90, _funcgapHeight=80;
private:
	std::vector<Brick*> vec_brick;
	int _levelNum=1;
	std::string _fileName;
	cocos2d::Label* _showfileName;
	cocos2d::Label* _showsavelog;
private:
	bool isformatmode = false;
	std::vector<cocos2d::Sprite*> _brickframes;
	bool iscurrenttype = true;
	int currenttag = 1;
	cocos2d::Sprite* _selectframe1;
	cocos2d::Sprite* _selectframe2;
public:
	void addbrick(int type, float x, float y);

	bool FuncButtonTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	bool ScreenTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void ScreenTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void ScreenTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);
	
};
#endif


