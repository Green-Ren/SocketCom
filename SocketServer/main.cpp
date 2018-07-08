#include <iostream>
#include "SocketServer.h"


int main()
{
	std::cout << "hello world!" << std::endl;

	CSocketServer server;
	server.Start();

	std::getchar();

	return 0;
}