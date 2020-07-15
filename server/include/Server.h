#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>

class Server
{
public:
	Server();
	void StartServer();
private:
	const unsigned int MAX_CONNECTIONS = 2;
	std::stack<unsigned int> availableNetworkIds;
	std::unordered_map<unsigned int, unsigned int> addressNetworkIdMap;

	bool AttemptNewConnection(unsigned int address, unsigned int* networkId);
	void DisconnectSession(unsigned int address);

};

