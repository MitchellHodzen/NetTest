#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>
#include "Address.h"

class UdpSocket;

class Server
{
public:
	Server(unsigned short port);
	void StartServer();
private:
	const unsigned int MAX_CONNECTIONS = 2;
	std::stack<unsigned int> availableNetworkIds;
	std::unordered_map<Address, unsigned int> addressNetworkIdMap;

	UdpSocket* udpSocket = nullptr;

	

	bool AttemptNewConnection(Address address, unsigned int* networkId);
	bool UserConnected(Address address, unsigned int networkId);
	void DisconnectSession(Address address);
	void AcceptConnection(Address address, unsigned int networkId);
	void RejectConnection(Address address);
	void FreeNetworkId(Address address);

};

