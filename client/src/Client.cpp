#include "Client.h"
#include <string>
void Client::SendMessage()
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
	address.sin_port = htons(3333);

	if (bind(socketHandle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
	{
		std::cout<<"Error binding socket"<<std::endl;
		return;
	}

	//ty gaffer on games https://gafferongames.com/post/sending_and_receiving_packets/
	unsigned int a = 127;
	unsigned int b = 0;
	unsigned int c = 0;
	unsigned int d = 1;

	unsigned int serverAddr = ( a << 24) | (b << 16) | (c << 8) | d;

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(serverAddr);
	server.sin_port = htons(2222);

	std::string message("Hello World!");

	int sent_size = sendto(socketHandle, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(sockaddr_in));

	if (sent_size != message.size() + 1)
	{
		std::cout<<"failed to send packet"<<std::endl;
		return;
	}

	
	//receive messages
	unsigned char data[255];
	unsigned int maxPacketSize = sizeof(data);

	sockaddr_in packetFrom;
	int fromLength = sizeof(packetFrom);
	int bytes = recvfrom(socketHandle, (char*) data, maxPacketSize, 0, (sockaddr*)&packetFrom, &fromLength);

	unsigned int fromAddress = ntohl(packetFrom.sin_addr.s_addr);
	unsigned int fromPort = ntohs(packetFrom.sin_port);

	std::cout<<"message recieved from " << fromAddress << ":" << fromPort<< ". Contents: " << data<<std::endl;

	closesocket(socketHandle);


	WSACleanup();
}
