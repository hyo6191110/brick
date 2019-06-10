#ifndef __MULTI_SCENE_H__
#define __MULTI_SCENE_H__

#include "cocos2d.h"
#include "network/SocketIO.h"
using namespace cocos2d;
using namespace cocos2d::network;

class MultiGame :public Layer,public SocketIO::SIODelegate
{
public:
	CREATE_FUNC(MultiGame);
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void cleanup();
	void Closethis(cocos2d::Ref* pSender);
	void sendMsg(cocos2d::Ref* pSender);

	virtual void onConnect(SIOClient* client)override;
	virtual void onMessage(SIOClient* client,const std::string& data)override;
	virtual void onClose(SIOClient* client)override;
	virtual void onError(SIOClient* client, const std::string& data)override;

	void callClientEvent(SIOClient* client, const std::string& data);
private:
	SIOClient* _sioclient;
	Label* _show_info;

};
#endif

