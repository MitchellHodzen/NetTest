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
				//TestStruct test;
				//test.networkId = 500;
				//test.integer = -15;
				//test.floatingPoint = 14.8495f;
				float myFloat = 14.8495f;

				//unsigned char* dataBuffer = reinterpret_cast<unsigned char *>(&myFloat);
				unsigned char *dataBuffer = (unsigned char *)&myFloat;
				printf("Original Hex: %02hhX   %02hhX  %02hhX    %02hhX\n", dataBuffer[0], dataBuffer[1], dataBuffer[2], dataBuffer[3]);

				/*unsigned int test = 0;
				test |= dataBuffer[0] & 0xFF;
				test <<= 8;
				test |= dataBuffer[1] & 0xFF;
				test <<= 8;
				test |= dataBuffer[2] & 0xFF;
				test <<= 8;
				test |= dataBuffer[3] & 0xFF;*/
				//float* myOtherFloat = reinterpret_cast<float*>(&test);
				//float myOtherFloat = dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24);
				unsigned int bytes = dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24);
				float myOtherFloat = *(reinterpret_cast<float*>(&bytes));

				unsigned char *newBuffer = (unsigned char *)&myOtherFloat;
				printf("New Hex: %02hhX   %02hhX  %02hhX    %02hhX\n", newBuffer[0], newBuffer[1], newBuffer[2], newBuffer[3]);

				//memcpy(&networkId, dataBuffer, sizeof(float));
				std::cout<<"Actual Value: " << myFloat<<std::endl;
				std::cout<<"Byte Value: " << myOtherFloat<<std::endl;

				int size = sizeof(TestStruct);
				std::cout<<"size: " << size<<std::endl;
				if (udpSocket.Send(&myFloat, size))
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
