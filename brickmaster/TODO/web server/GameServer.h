#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<string>
#include<thread>
#include<winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <array>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

constexpr auto IP_BUF_SIZE = 129;
constexpr auto MSG_BUF_SIZE = 1024;
constexpr auto MAX_NUM = 2;

struct  ClientInformation  
{
	SOCKET ClientSock;       //Ì×½Ó×Ö
	sockaddr_in Client;      //Clinet IP port
	int ID;                   //Server·ÖÅäµÄIDºÅÂë
	HANDLE h_thread;
	bool Active;
};

class GameServer
{
public:
	GameServer();
	~GameServer();
	//int progress();
	void WaitForClient();

	static int SendMessageToOneClient(int ID, const std::string  str);  
	static void SendMessageToAllClient(const std::string  str, int ID = -1);
	int CheckSocket();     
	static void CleanSocket(int ID); 
	static DWORD WINAPI CreateClientThread(LPVOID lpParameter);
private:
	SOCKET ServerSocket;
	SOCKADDR_IN ServerAddr;
	//SOCKET ClientSocket;
	//SOCKADDR_IN ClientAddr;
	HANDLE h_thread;

	int addr_len  = sizeof(SOCKADDR_IN);
	char buf_ip[IP_BUF_SIZE];
	static ClientInformation AcceptSocket[MAX_NUM];  
};



#endif