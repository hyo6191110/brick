#ifndef CLIENT_H
#define CLIENT_H

#include"cocos2d.h"

#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<thread>
#include<winsock2.h>

#include "scenes/WebGame.h"
#include "MultiGame.h"

using std::cout;
using std::cin;
using std::endl;
using namespace cocos2d;

const int BUF_SIZE = 1024;


class GameClient
{
public:
	GameClient();
	~GameClient();
	void sendthread();
	void recvthread();
	void setMyScore(int s) { _my_score = s; }
	void setMyTime(int t) { _my_time = t; }
	string getOpScore() { return _op_score ; }
	string getOpTime() { return _op_time ; }
private:
	SOCKET ServerSocket;
	SOCKADDR_IN ServerAddr;

	int _current_score = -1;
	int _current_time = -1;

	mutex mt;
	int _my_score = -1;
	int _my_time = -1;
	string _op_score;
	string _op_time ;

};

void startClient(GameClient& gc);

#endif