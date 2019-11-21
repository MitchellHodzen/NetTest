#include "Server.h"
#include "UdpSocket.h"

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
				unsigned char byte1 = dataBuffer[0];
				unsigned char byte2 = dataBuffer[1];
				unsigned char byte3 = dataBuffer[2];
				unsigned char byte4 = dataBuffer[3];
				int number = byte1 | (byte2<<8) | (byte3<<16) | (byte4<<24);
				std::cout<<"message recieved from " << fromAddr << ":" << fromPort<< ". Contents: " << number <<std::endl;

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
