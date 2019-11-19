#pragma once
#include <iostream>
#include <winsock2.h>

class UdpSocket
{
public:
	UdpSocket(unsigned short listenPort)
	{
		this->listenPort = listenPort;
		listenAddress.sin_family = AF_INET;
		listenAddress.sin_addr.s_addr = INADDR_ANY;
		listenAddress.sin_port = htons(listenPort);
	}

	~UdpSocket()
	{
		closesocket(socketHandle);
		WSACleanup();
	}

	bool Initialize()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
		{
			std::cout<<"Error starting WSA"<<std::endl;
			return false;
		}

		socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (socketHandle < 0)
		{
			std::cout<<"Error creating socket"<<std::endl;
			return false;
		}

		if (bind(socketHandle, (const sockaddr*) &listenAddress, addressSize) < 0)
		{
			std::cout<<"Error binding socket"<<std::endl;
			return false;
		}

		//Make socket non-blocking
		DWORD nonBlocking = 1;
		if (ioctlsocket(socketHandle, FIONBIO, &nonBlocking) != 0)
		{
			std::cout<<"Failed to make socket non-blocking"<<std::endl;
			return false;
		}

		return true;
	}

	//Sets the socket's send address to the address defined by {ipPartA}.{ipPartB}.{ipPartC}.{ipPartD}:{port}
	bool SetSendAddress(unsigned char ipPartA, unsigned char ipPartB, unsigned char ipPartC, unsigned char ipPartD, unsigned short port)
	{
		//ty gaffer on games https://gafferongames.com/post/sending_and_receiving_packets/ for algorithm
		unsigned int address = ( ipPartA << 24) | (ipPartB << 16) | (ipPartC << 8) | ipPartD;

		return SetSendAddress(address, port);
	}

	//Sets the socket's send address to the address defined by the address integer and port
	bool SetSendAddress(unsigned int address, unsigned short port)
	{
		sendPort = port;
		sendAddress.sin_family = AF_INET;
		sendAddress.sin_addr.s_addr = htonl(address);
		sendAddress.sin_port = htons(port);

		return true;
	}

	//Receive a message which will be written to the given databuffer. Returns true if a message was recieved, false otherwise
	bool RecieveMessage(char * dataBuffer, unsigned int bufferSize, unsigned int* fromAddress, unsigned short* fromPort)
	{
		sockaddr_in packetFrom;
		int fromLength = sizeof(packetFrom);

		int bytes = recvfrom(socketHandle, dataBuffer, bufferSize, 0, (sockaddr*)&packetFrom, &fromLength);

		if (bytes <= 0)
		{
			//std::cout<<"no message recieved"<<std::endl;
			return false;
		}

		unsigned int from = ntohl(packetFrom.sin_addr.s_addr);
		unsigned short port = ntohs(packetFrom.sin_port);

		if (fromAddress != nullptr)
		{
			*fromAddress = ntohl(packetFrom.sin_addr.s_addr);
		}
		if (fromPort != nullptr)
		{
			*fromPort = ntohs(packetFrom.sin_port);
		}

		return true;
	}

	//Send a message to the set send address. Returns true if message was successfully sent, false otherwise
	bool Send(const char * message, unsigned int messageLength)
	{
		int sent_size = sendto(socketHandle, message, messageLength, 0, (sockaddr*)&sendAddress, addressSize);
		if (sent_size != messageLength)
		{
			std::cout<<"failed to send response packet"<<std::endl;
			return false;
		}
		return true;
	}
	
private:
	int socketHandle = 0;
	const int addressSize = sizeof(sockaddr_in);

	sockaddr_in listenAddress;
	unsigned short listenPort = 0;

	sockaddr_in sendAddress;
	unsigned short sendPort = 0;
};

