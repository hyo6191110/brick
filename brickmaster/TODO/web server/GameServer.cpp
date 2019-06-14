#include "D:/1/cocos_project/brickmaster/Classes/web server/GameServer.h"

ClientInformation GameServer::AcceptSocket[MAX_NUM] = { ClientInformation(),ClientInformation()};

GameServer::GameServer()
{
	int RetVal;
	WORD SocketVersion = MAKEWORD(2, 2);
	WSADATA wsd;
	if (WSAStartup(SocketVersion, &wsd) != 0)
	{
		cout << "绑定Socket库失败" << endl;
	}

	
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "创建服务器套接字失败" << endl;
		WSACleanup();
		return ;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(2345);
	ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	RetVal = bind(ServerSocket, (SOCKADDR *)&ServerAddr, sizeof(SOCKADDR_IN));
	if (RetVal == SOCKET_ERROR)
	{
		cout << "套接字绑定失败" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return ;
	}

	RetVal = listen(ServerSocket, 2);
	if (RetVal == SOCKET_ERROR)
	{
		cout << "套接字监听失败" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return ;
	}
	
	for (int i = 0; i < MAX_NUM; i++)
	{
		AcceptSocket[i].ClientSock = INVALID_SOCKET;
	}
	/*int ClientAddrLen = sizeof(ClientAddr);
	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrLen);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "接收客户端请求失败" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return ;
	}*/


}
GameServer::~GameServer()
{
	closesocket(ServerSocket);
	for (int i = 0; i < MAX_NUM; i++)
	{
		if (AcceptSocket[i].ClientSock != INVALID_SOCKET)
		{
			CleanSocket(i);
		}
	}
	WSACleanup();
	return ;
}

/*int GameServer::progress()
{
	int RetVal;
	char ReceiveBuff[BUFSIZ];
	char SendBuff[BUFSIZ];
	while (true)
	{
		ZeroMemory(ReceiveBuff, BUFSIZ);
		RetVal = recv(ClientSocket, ReceiveBuff, BUFSIZ, 0);
		if (RetVal == SOCKET_ERROR)
		{
			cout << "接收数据失败" << endl;
			closesocket(ServerSocket);
			closesocket(ClientSocket);
			WSACleanup();
			return -1;
		}
		cout << "接收自客户端数据：" << ReceiveBuff << endl;
		cout << "向客户端发送数据：";
		cin >> SendBuff;
		send(ClientSocket, SendBuff, strlen(SendBuff), 0);
	}
}*/

void GameServer::WaitForClient()
{
     while (true)
     {
		 int index = CheckSocket();
		 if (index != -1)
		 {
			 AcceptSocket[index].ClientSock = ::accept(ServerSocket, (SOCKADDR*)&AcceptSocket[index].Client, &addr_len);
			 if (AcceptSocket[index].ClientSock == INVALID_SOCKET)
			 {
				 cerr << "Failed to accept client!Error code: " << ::WSAGetLastError() << "\n";
				 ::WSACleanup();
				 system("pause");
				 exit(1);
			 }
			 AcceptSocket[index].ID = index;

			 ::InetNtop(AcceptSocket[index].Client.sin_family, &AcceptSocket[index].Client, buf_ip, IP_BUF_SIZE);
			 cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(AcceptSocket[index].Client.sin_port) << endl;
			 h_thread = ::CreateThread(nullptr, 0, GameServer::CreateClientThread, (LPVOID)&AcceptSocket[index], 0, nullptr);
			 if (h_thread == NULL)
			 {
				 cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << "\n";
				 ::WSACleanup();
				 system("pause");
				 exit(1);
			 }
			 cout << "新玩家" << index << "的接受线程创建成功\n";
			 //::CloseHandle(h_thread);
		 }
		 else
		 {
			 SOCKADDR_IN Client;
			 int ClntLen = sizeof(Client);
			 SOCKET Accept = accept(ServerSocket,(struct sockaddr*)&Client, &ClntLen);

			 if (Accept == INVALID_SOCKET)
			 {
				 cout << "连接错误\n";
				 fflush(stdout);
				 break;
			 }

			 fflush(stdout);
			 send(Accept, "当前用户已满", strlen("当前用户已满") + sizeof(char), 0);

			 closesocket(Accept);

			 cout << "非法连接玩家已断开\n";
			 fflush(stdout);
			 //break;
		 }
        
     }
 }

 DWORD WINAPI GameServer::CreateClientThread(LPVOID lpParameter)
 {
     auto client_info = (ClientInformation*)lpParameter;
	 auto sock_clt = client_info->ClientSock;

     char buf_msg[MSG_BUF_SIZE];
     int ret_val = 0;
     int snd_result = 0;
     do
     {
         memset(buf_msg, 0, MSG_BUF_SIZE);
         ret_val = ::recv(sock_clt, buf_msg, MSG_BUF_SIZE, 0);
         if (ret_val > 0)
         {
             if (strcmp(buf_msg, "exit") == 0)
             {
                 cout << "Client requests to close the connection..." << endl;
                break;
             }
             cout << "Message received: " << buf_msg << endl;
			 SendMessageToAllClient(buf_msg, client_info->ID);
             /*snd_result = ::send(sock_clt, buf_msg, MSG_BUF_SIZE, 0);
             if (snd_result == SOCKET_ERROR)
             {
                 cerr << "Failed to send message to client!Error code: " << ::GetLastError() << "\n";
                 ::closesocket(sock_clt);
                 system("pause");
                 return 1;
             }*/
         }
         else if (ret_val == 0)
         {
             cout << "connection closed..." << endl;
			 CleanSocket(client_info->ID);
			 system("pause");
			 return 1;
         }
         else
         {
             cerr << "Failed to receive message from client!Error code: " << ::GetLastError() << "\n";
			 CleanSocket(client_info->ID);
             system("pause");
             return 1;
         }
     } 
	 while (ret_val > 0);

     ret_val = ::shutdown(sock_clt, SD_SEND);
     if (ret_val == SOCKET_ERROR)
     {
         cerr << "Failed to shutdown the client socket!Error code: " << ::GetLastError() << "\n";
         //::closesocket(sock_clt);
		 CleanSocket(client_info->ID);
         system("pause");
         return 1;
     }
     return 0;
 }

 int GameServer::SendMessageToOneClient(int ID, const std::string  str)
 {
	 if (ID < 0)
		 return 0;

	 int oneSend = send(AcceptSocket[ID].ClientSock,
		 str.c_str(), str.length() + sizeof(char), 0);
	 cout << "向玩家" << ID << "发送了" << oneSend << "个bits的消息\n";
	 fflush(stdout);
	 if (oneSend == SOCKET_ERROR)
	 {
		 cout << "向玩家" << ID << "发送消息失败\n";
		 fflush(stdout);
		 return 0;
	 }

	 return 1;
 }


 void GameServer::SendMessageToAllClient(const std::string  str, int ID)
 {

	 bool flag = true;
	 for (int i = 0; i < MAX_NUM; i++)
	 {
		 if (ID != i)
		 {
			 if (AcceptSocket[i].ClientSock != INVALID_SOCKET &&
				 !SendMessageToOneClient(i, str))
				 flag = false;
		 }
	 }
	 if (flag)
		 cout << "服务器向全体(除了玩家" << ID << ")发送了消息:"
		 << str << "\n";
	 else
		 cout << "发送存在问题\n";
	 fflush(stdout);
 }

 int GameServer::CheckSocket()
 {
	 for (int i = 0; i < MAX_NUM; i++)
	 {
		 if (AcceptSocket[i].ClientSock == INVALID_SOCKET)
			 return i;
	 }
	 return -1;
 }

 void GameServer::CleanSocket(int ID)
 {
	 if (ID < 0)
		 return;

	 cout << " 玩家" << ID << "退出游戏\n";
	 fflush(stdout);

	 AcceptSocket[ID].Active = false;
	 closesocket(AcceptSocket[ID].ClientSock);
	 AcceptSocket[ID].ClientSock = INVALID_SOCKET;

	 fflush(stdout);
	 ::CloseHandle(AcceptSocket[ID].h_thread);

	 cout << "关闭成功!\n";
	 fflush(stdout);
 }