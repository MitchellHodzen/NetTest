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

				unsigned int networkId = dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24);

				unsigned int intBytes = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
				int myInt = *(reinterpret_cast<int*>(&intBytes));

				unsigned int floatBytes = dataBuffer[8] | (dataBuffer[9]<<8) | (dataBuffer[10]<<16) | (dataBuffer[11]<<24);
				float myFloat = *(reinterpret_cast<float*>(&floatBytes));

				std::cout<<"\tNetwork ID: " << networkId <<std::endl;
				std::cout<<"\tInteger: " << myInt <<std::endl;
				std::cout<<"\tFloat: " << myFloat <<std::endl;

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
