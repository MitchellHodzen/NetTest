#include "Client.h"
#include "UdpSocket.h"
#include <string>
void Client::SendMessage()
{
	UdpSocket udpSocket(3333);
	if (udpSocket.Initialize())
	{
		while(true)
		{

			if (udpSocket.SetSendAddress(127, 0, 0, 1, 2222))
			{
				std::string message;
				std::getline(std::cin, message);
				if (udpSocket.Send(message.c_str(), message.size() + 1))
				{
					bool receivedResponse = false;

					while(!receivedResponse)
					{
						unsigned char dataBuffer[255];
						unsigned int bufferSize = sizeof(dataBuffer);

						unsigned int fromAddr;
						unsigned short fromPort;

						if (udpSocket.RecieveMessage((char *)dataBuffer, bufferSize, &fromAddr, &fromPort))
						{
							std::cout<<"message recieved from " << fromAddr << ":" << fromPort<< ". Contents: " << dataBuffer <<std::endl;
							receivedResponse = true;
						}
					}
				}
				else
				{
					std::cout<<"Failed to send packet"<<std::endl;
				}
			}
			else
			{
				std::cout<<"Could not set send address"<<std::endl;
			}
			
		}
	}
	else
	{
		std::cout<<"Could not initialize socket"<<std::endl;
	}
}
