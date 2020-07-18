#include <iostream>
#include "Server.h"

int main(int argc, char* args[])
{
	Server server(2222);
	server.StartServer();
	return 0;
}