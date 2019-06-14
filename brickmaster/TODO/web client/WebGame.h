#ifndef __GAME_WEB_H__
#define __GAME_WEB_H__

#include "cocos2d.h"
#include "GameModeTimelimit.h"

using namespace cocos2d;

class WebGame :public GameModeTimelimit
{
protected:
	cocos2d::Label* _show_oppo_score;
	cocos2d::Label* _show_oppo_time;

public:
	void setOpScore(const std::string& str) { _show_oppo_score->setString(str); }
	void setOpTime(const std::string& str) { _show_oppo_time->setString(str); }
	int getScore() { return _score; }
	int getTime() { return _time; }
	virtual void writeScoreToUserData() override;
	static cocos2d::Scene* createScene(int level, int time);
	CREATE_FUNC(WebGame);
	virtual void createHUDforWeb() override;
	virtual void closeOthers()override;

	void updateSend(float dt);
	void updateRecv(float dt);
	void setGameClient();
	void cleanGameClient();
};

#endif
