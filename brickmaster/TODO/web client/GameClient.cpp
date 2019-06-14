#include"web client/GameClient.h"

#pragma warning(disable : 4996)

GameClient::GameClient()
{
	int RetVal;
	WSADATA Wsd;
	if (WSAStartup(MAKEWORD(2, 2), &Wsd) != 0)
	{
		cout << "初始化套接字动态库失败" << endl;
		return ;
	}
	
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "创建套接字失败" << endl;
		WSACleanup();
		return ;
	}
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(2345);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	RetVal = connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (RetVal == SOCKET_ERROR)
	{
		cout << "链接服务器失败" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return ;
	}
	
	
}
GameClient::~GameClient()
{
	closesocket(ServerSocket);
	WSACleanup();
}


void GameClient::sendthread()
{
	int RetVal;
	char SendBuff[BUF_SIZE];
	while (true)
	{

		if (_current_score != _my_score)
		{
			_current_score = _my_score;

			ZeroMemory(SendBuff, BUF_SIZE);

			string str = StringUtils::format("score:%d", _my_score);
			for (size_t i = 0; i < str.length(); i++)
			{
				SendBuff[i] = str[i];
			}
			SendBuff[str.length()] = '\0';

			RetVal = send(ServerSocket, SendBuff, strlen(SendBuff), 0);
			if (RetVal == SOCKET_ERROR)
			{
				cout << "发送数据失败" << endl;
				closesocket(ServerSocket);
				WSACleanup();
				return;
			}
		}

		if (_current_time != _my_time)
		{
			_current_score = _my_time;

			ZeroMemory(SendBuff, BUF_SIZE);
			string str = StringUtils::format("time:%d", _my_time);

			for (size_t i = 0; i < str.length(); i++)
			{
				SendBuff[i] = str[i];
			}
			SendBuff[str.length()] = '\0';

			RetVal = send(ServerSocket, SendBuff, strlen(SendBuff), 0);
			if (RetVal == SOCKET_ERROR)
			{
				cout << "发送数据失败" << endl;
				closesocket(ServerSocket);
				WSACleanup();
				return;
			}
		}
	}
}

void GameClient::recvthread()
{
	int RetVal;
	char RECVBuff[BUF_SIZE];
	while (true)
	{
		mt.lock();

		ZeroMemory(RECVBuff, BUF_SIZE);
		RetVal = recv(ServerSocket, RECVBuff, BUF_SIZE, 0);
		if (RetVal == SOCKET_ERROR)
		{
			log("接收数据失败！");
			closesocket(ServerSocket);
			WSACleanup();
			return;
		}
		if (strncmp(RECVBuff, "当前用户已满", strlen("当前用户已满")))
		{
			log("当前用户已满或网络错误，请断开重连！");
				closesocket(ServerSocket);
			WSACleanup();
			return;
		}

		if (RECVBuff[0] == 's')
		{
			string str;
			bool f = false;
			for (size_t i = 0; i < strlen(RECVBuff); i++)
			{
				if (f)
				{
					if (!isalpha(RECVBuff[i]))
						str[i] = RECVBuff[i];
					else
						break;
				}
				else if (RECVBuff[i] == ':')
					f = true;

			}
			_op_score = str;
		}
		if (RECVBuff[0]=='t')
		{
			string str;
			bool f = false;
			for (size_t i = 0; i < strlen(RECVBuff); i++)
			{
				if (f)
				{
					if (!isalpha(RECVBuff[i]))
						str[i] = RECVBuff[i];
					else
						break;
				}
				else if (RECVBuff[i] == ':')
					f = true;

			}
			_op_time = str;
		}
		mt.unlock();
	}
}

void startClient(GameClient& gc)
{
	std::thread st([&]() {gc.sendthread(); });
	std::thread sr([&]() {gc.recvthread(); });
	st.detach();
	sr.detach();
}