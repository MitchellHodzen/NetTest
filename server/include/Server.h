#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>
#include "Address.h"

class Server
{
public:
	Server();
	void StartServer();
private:
	const unsigned int MAX_CONNECTIONS = 2;
	std::stack<unsigned int> availableNetworkIds;
	std::unordered_map<Address, unsigned int> addressNetworkIdMap;
	

	bool AttemptNewConnection(Address address, unsigned int* networkId);
	void DisconnectSession(Address address);

};

