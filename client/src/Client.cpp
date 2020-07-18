#include "Client.h"
#include "UdpSocket.h"
#include <string>
#include "MessageStructs.h"

void Client::SendMessage()
{
	std::cout<<"Enter port to receive on: ";
	std::string portToOpenInput;
	std::cin >> portToOpenInput;
	unsigned short portToOpen = std::stoi(portToOpenInput);
	std::cout<<"Listening on port " << portToOpen << std::endl;
	UdpSocket udpSocket(portToOpen);
	if (udpSocket.Initialize())
	{
		std::cout<<"Input IP and Port in aaaa.bbbb.cccc.dddd:xxxx format: ";
		std::string ipInput;
		std::cin >> ipInput;
		std::cin.ignore();

		size_t last = 0;
		size_t next = 0;
		unsigned char partA;
		unsigned char partB;
		unsigned char partC;
		unsigned char partD;
		unsigned short port;

		//IPs have exactly 4 parts, delimited by a '.'
		size_t currentPartIndex = 0;
		size_t lastPartIndex = 0;

		currentPartIndex = ipInput.find('.', lastPartIndex);
		partA = std::stoi(ipInput.substr(lastPartIndex, currentPartIndex-lastPartIndex));

		lastPartIndex = currentPartIndex + 1;
		currentPartIndex = ipInput.find('.', lastPartIndex);
		partB = std::stoi(ipInput.substr(lastPartIndex, currentPartIndex-lastPartIndex));

		lastPartIndex = currentPartIndex + 1;
		currentPartIndex = ipInput.find('.', lastPartIndex);
		partC = std::stoi(ipInput.substr(lastPartIndex, currentPartIndex-lastPartIndex));

		//Last part is ended by a ':', followed by the port
		lastPartIndex = currentPartIndex + 1;
		currentPartIndex = ipInput.find(':', lastPartIndex);
		partD = std::stoi(ipInput.substr(lastPartIndex, currentPartIndex-lastPartIndex));

		lastPartIndex = currentPartIndex + 1;
		port = std::stoi(ipInput.substr(lastPartIndex));

		//if (udpSocket.SetSendAddress(127, 0, 0, 1, 2222))
		if (udpSocket.SetSendAddress(partA, partB, partC, partD, port))
		{

			//Get a connection
			MSG_CONNECT_REQUEST connectionRequest;
			if (udpSocket.Send(&connectionRequest, sizeof(MSG_CONNECT_REQUEST)))
			{

				bool receivedResponse = false;

				unsigned char dataBuffer[255];
				unsigned int bufferSize = sizeof(dataBuffer);

				unsigned int fromAddr;
				unsigned short fromPort;

				unsigned int networkId;
				bool connected = false;

				//try to connect to server
				while(!receivedResponse)
				{
					if (udpSocket.RecieveMessage(dataBuffer, bufferSize, &fromAddr, &fromPort))
					{
						std::cout<<"Message recieved from " << fromAddr << ":" << fromPort <<std::endl;

						MSG_TYPE messageType = MESSAGE::DetermineMessageType(dataBuffer, bufferSize);

						if (messageType == MSG_TYPE::CONNECT_RESPONSE)
						{
							MSG_CONNECT_RESPONSE connectResponse(dataBuffer, bufferSize);
							CONNECT_RESP responseType = connectResponse.response;

							if (responseType == CONNECT_RESP::ACCEPTED)
							{
								networkId = connectResponse.networkId;
								connected = true;
								receivedResponse = true;
								std::cout<<"Connected to server. Network ID: " << networkId << std::endl;
							}
							else
							{
								connected = false;
								receivedResponse = true;
								std::cout<<"Could not connect to server" << std::endl;
							}
						}
					}
				}

				while(connected)
				{
					std::cout<<"Message to send: ";
					std::string message;
					getline(std::cin, message);
					std::cout<<"Sending message..."<<std::endl;

					MSG_TEXT messagePacket(networkId, message);
					int size = sizeof(MSG_TEXT);
					if (udpSocket.Send(&messagePacket, size))
					{
						receivedResponse = false;
						while(!receivedResponse)
						{
							if (udpSocket.RecieveMessage(dataBuffer, bufferSize, &fromAddr, &fromPort))
							{
								std::cout<<"Message recieved from " << fromAddr << ":" << fromPort <<std::endl;

								MSG_TYPE messageType = MESSAGE::DetermineMessageType(dataBuffer, bufferSize);

								switch(messageType)
								{
									case MSG_TYPE::ACK:
									{
										std::cout<<"Message ACK recieved"<<std::endl;
										receivedResponse = true;
										break;
									}
									case MSG_TYPE::DISCONNECT:
									{
										std::cout<<"Disconnected from server"<<std::endl;
										connected = false;
										receivedResponse = true;
										break;
									}
									default:
										break;
								}
							}
						}
					}
					else
					{
						std::cout<<"Failed to send packet"<<std::endl;
					}
				}
			}
			else
			{
				std::cout<<"Failed to send connection packet"<<std::endl;
			}
		}
		else
		{
			std::cout<<"Could not set send address"<<std::endl;
		}
		
	}
	else
	{
		std::cout<<"Could not initialize socket"<<std::endl;
	}
}
