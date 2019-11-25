#include "Client.h"
#include "UdpSocket.h"
#include <string>
#include "TestStruct.h"

void Client::SendMessage()
{
	UdpSocket udpSocket(3333);
	if (udpSocket.Initialize())
	{
		//while(true)
		//{

			if (udpSocket.SetSendAddress(127, 0, 0, 1, 2222))
			{
				TestStruct test;
				test.networkId = 500;
				test.integer = -15;
				test.floatingPoint = 14.8495f;
				int size = sizeof(TestStruct);
				
				if (udpSocket.Send(&test, size))
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
			
		//}
	}
	else
	{
		std::cout<<"Could not initialize socket"<<std::endl;
	}
}
