#include "Client.h"
#include "UdpSocket.h"
#include <string>
#include "MessageStructs.h"

void Client::SendMessage()
{
	UdpSocket udpSocket(3333);
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
			while(true)
			{
				std::cout<<"Message to send: ";
				std::string message;
				getline(std::cin, message);
				std::cout<<"Sending message..."<<std::endl;

				//MSG_TEXT messagePacket(0, message.size() + 1, message.c_str());
				MSG_TEXT messagePacket(0, message);
				int size = sizeof(MSG_TEXT);
				//if (udpSocket.Send(&test, size))
				//if (udpSocket.Send(message.c_str(), message.size() + 1))
				if (udpSocket.Send(&messagePacket, size))
				{
					bool receivedResponse = false;

					unsigned char dataBuffer[255];
					unsigned int bufferSize = sizeof(dataBuffer);

					unsigned int fromAddr;
					unsigned short fromPort;
					
					while(!receivedResponse)
					{

						if (udpSocket.RecieveMessage(dataBuffer, bufferSize, &fromAddr, &fromPort))
						{
							std::cout<<"Message recieved from " << fromAddr << ":" << fromPort<< ". Contents: " << dataBuffer <<std::endl;
							receivedResponse = true;
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
			std::cout<<"Could not set send address"<<std::endl;
		}
		
	}
	else
	{
		std::cout<<"Could not initialize socket"<<std::endl;
	}
}
