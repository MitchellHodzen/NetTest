#include "Server.h"

void Server::StartServer()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		std::cout<<"Error starting WSA"<<std::endl;
		return;
	}

	int socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (socketHandle < 0)
	{
		std::cout<<"Error creating socket"<<std::endl;
		return;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(2222);

	if (bind(socketHandle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
	{
		std::cout<<"Error binding socket"<<std::endl;
		return;
	}

	//Make socket non-blocking
	DWORD nonBlocking = 1;
	if (ioctlsocket(socketHandle, FIONBIO, &nonBlocking) != 0)
	{
		std::cout<<"Failed to make socket non-blocking"<<std::endl;
		return;
	}

	//receive messages
	std::cout<<"Listening for messages"<<std::endl;
	while(true)
	{
		unsigned char data[255];
		unsigned int maxPacketSize = sizeof(data);

		sockaddr_in packetFrom;
		int fromLength = sizeof(packetFrom);
		int bytes = recvfrom(socketHandle, (char*) data, maxPacketSize, 0, (sockaddr*)&packetFrom, &fromLength);

		if (bytes <= 0)
		{
			//std::cout<<"no message recieved"<<std::endl;
			continue;
		}
		unsigned int fromAddress = ntohl(packetFrom.sin_addr.s_addr);
		unsigned int fromPort = ntohs(packetFrom.sin_port);

		std::cout<<"message recieved from " << fromAddress << ":" << fromPort<< ". Contents: " << data<<std::endl;

		sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(fromAddress);
		server.sin_port = htons(fromPort);

		std::string message("We got your message!");
			std::cout<<"\tReplying with: " << message <<std::endl;

		int sent_size = sendto(socketHandle, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(sockaddr_in));
		if (sent_size != message.size() + 1)
		{
			std::cout<<"failed to send response packet"<<std::endl;
			return;
		}

	}

	closesocket(socketHandle);


	WSACleanup();
}
