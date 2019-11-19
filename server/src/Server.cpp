#include "Server.h"
#include "KSocket.h"

void Server::StartServer()
{
	KSocket ksocket(2222);
	if (ksocket.Initialize())
	{
		//receive messages
		std::cout<<"Listening for messages"<<std::endl;
		while(true)
		{
			unsigned char dataBuffer[255];
			unsigned int bufferSize = sizeof(dataBuffer);

			unsigned int fromAddr;
			unsigned short fromPort;
			bool receivedMessage = ksocket.RecieveMessage((char *)dataBuffer, bufferSize, &fromAddr, &fromPort);

			if (receivedMessage)
			{
				std::cout<<"message recieved from " << fromAddr << ":" << fromPort<< ". Contents: " << dataBuffer <<std::endl;

				if (ksocket.SetSendAddress(fromAddr, fromPort))
				{
					std::string message("We got your message!");
					//std::cout<<"\tReplying with: " << message << std::endl;
					if (!(ksocket.SendMessage(message.c_str, message.size() + 1)))
					{
						std::cout<<"Response unsuccessful"<<std::endl;
					}
				}
			}
		}
	}
}
