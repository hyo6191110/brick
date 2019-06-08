#ifndef __USER_LAYER_H__
#define __USER_LAYER_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
struct classic_data
{
	int score;
	int brick;
	int level;
	classic_data(int s, int b, int l) :score(s), brick(b), level(l) {}
	classic_data(){}
	bool operator<(classic_data b) { return this->score > b.score; }
};
struct pong_data
{
	int score;
	int my_goal;
	int op_goal;
	int level;
	pong_data(int s, int m,int o, int l) :score(s), my_goal(m),op_goal(o), level(l) {}
	pong_data() {}
	bool operator<(pong_data b) { return this->score > b.score; }
}; 
struct timelimit_data
{
	int score;
	int time;
	int level;
	timelimit_data(int s, int t, int l) :score(s), time(t), level(l) {}
	timelimit_data() {}
	bool operator<(timelimit_data b) { return this->score > b.score; }
};
class MenuUser :public cocos2d::Layer
{
public:
	CREATE_FUNC(MenuUser);
	virtual bool init();
	void Closethis(cocos2d::Ref* pSender);
	void initData();

	void clearClassic(cocos2d::Ref* pSender);
	void clearPong(cocos2d::Ref* pSender);
	void clearTimelimit(cocos2d::Ref* pSender);
	void prevMode(cocos2d::Ref* pSender);
	void nextMode(cocos2d::Ref* pSender);
	void setclassic();
	void setpong();
	void settimelimit();
private:
	int _current_gamemode=1;
	Label* _showmode;
	vector<classic_data> _v_classic;
	vector<pong_data> _v_pong;
	vector<timelimit_data> _v_timelimit;
	vector<Label*> _v_showscore;
	vector<Label*> _v_showspecial;
	vector<Label*> _v_showlevel;
};
#endif
