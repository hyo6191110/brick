#include "D:/1/cocos_project/brickmaster/Classes/web server/GameServer.h"
#include <iostream>

using std::cout;
GameServer gc;

int main()
{
	//gc.progress();
	gc.WaitForClient();
	system("pause");
	return 0;
}