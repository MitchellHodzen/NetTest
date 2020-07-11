#include "Server.h"
#include "UdpSocket.h"
#include "TestStruct.h"

void Server::StartServer()
{
	UdpSocket udpSocket(2222);
	if (udpSocket.Initialize())
	{
		//receive messages
		std::cout<<"Listening for messages"<<std::endl;

		unsigned char dataBuffer[255];
		unsigned int bufferSize = sizeof(dataBuffer);

		unsigned int fromAddr;
		unsigned short fromPort;

		while(true)
		{
			bool receivedMessage = udpSocket.RecieveMessage(dataBuffer, bufferSize, &fromAddr, &fromPort);

			if (receivedMessage)
			{
				std::cout<<"Message recieved from " << fromAddr << std::endl;

				TestStruct testStruct(dataBuffer, bufferSize);

				std::cout<<"\tNetwork ID: " << testStruct.networkId <<std::endl;
				std::cout<<"\tInteger: " << testStruct.integer <<std::endl;
				std::cout<<"\tFloat: " << testStruct.floatingPoint <<std::endl;

				if (udpSocket.SetSendAddress(fromAddr, fromPort))
				{
					std::string message("We got your message!");
					std::cout<<"\tReplying with: " << message << std::endl;
					if(!udpSocket.Send(message.c_str(), message.size() + 1))
					{
						std::cout<<"Response unsuccessful"<<std::endl;
					}
				}
			}
		}
	}
}
