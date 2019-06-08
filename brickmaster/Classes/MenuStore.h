#ifndef __STORE_LAYER_H__
#define __STORE_LAYER_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class MenuStore :public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuStore);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void initIterator();
	void prevBackground(cocos2d::Ref* pSender);
	void nextBackground(cocos2d::Ref* pSender);
	void prevPlate(cocos2d::Ref* pSender);
	void nextPlate(cocos2d::Ref* pSender);
	void writeBackground(cocos2d::Ref* pSender);
	void writePlate(cocos2d::Ref* pSender);
private:
	const std::vector<std::string> background_texture =
	{
		"background/background_1.jpg",
		"background/background_2.jpg",
		"background/background_3.jpg",
		"background/background_4.jpg",
		"background/background_5.jpg",
		"background/background_6.jpg",
		"background/background_7.jpg"
	};
	const std::vector<std::string> plate_texture =
	{
		"game/defender_0.png",
		"game/defender_1.png",
		"game/defender_2.png",
		"game/defender_3.png",
		"game/defender_4.png",
		"game/defender_5.png"
	};
	vector<string>::const_iterator it_background;
	vector<string>::const_iterator it_plate;
	Sprite* _show_background;
	Sprite* _show_plate;
};

#endif

