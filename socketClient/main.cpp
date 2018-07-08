#include <iostream>
#include "SocketHandle.h"
#include "SocketClientImpl.h"
#include "SocketClient.h"

int main()
{
	std::cout << "hello world!" << std::endl;
	CSocketClient client;
	client.Start();
	client.Send();

	std::getchar();

	return 0;
}