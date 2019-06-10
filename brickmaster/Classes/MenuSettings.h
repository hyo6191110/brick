#ifndef __SETTINGS_LAYER_H__
#define __SETTINGS_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class MenuSettings :public cocos2d::Layer
{
public:
	CREATE_FUNC(MenuSettings);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void bgmCheckBox(cocos2d::Ref* pSender,CheckBox::EventType type);
	void effectCheckBox(cocos2d::Ref* pSender, CheckBox::EventType type);
	void bgmSlideBar(cocos2d::Ref* pSender, Slider::EventType type);
	void effectSlideBar(cocos2d::Ref* pSender, Slider::EventType type);
	void ItemCheckBox(cocos2d::Ref* pSender, CheckBox::EventType type);
protected:
	Label* _show_bgm_percent;
	Label* _show_effect_percent;
	Slider* _bgm_slider;
	Slider* _effect_slider;
};
#endif
