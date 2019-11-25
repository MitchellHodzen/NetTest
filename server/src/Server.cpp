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

				unsigned int bytes = dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24);
				//unsigned int networkId = byte1 | (byte2<<8) | (byte3<<16) | (byte4<<24);
				float myOtherFloat = *(reinterpret_cast<float*>(&bytes));

				std::cout<<"\tNetwork ID: " << myOtherFloat <<std::endl;

				/*unsigned char byte5 = dataBuffer[4];
				unsigned char byte6 = dataBuffer[5];
				unsigned char byte7 = dataBuffer[6];
				unsigned char byte8 = dataBuffer[7];
				int integer = byte5 | (byte6<<8) | (byte7<<16) | (byte8<<24);
				std::cout<<"\tInteger: " << integer <<std::endl;

				unsigned char byte9 = dataBuffer[0];
				unsigned char byte10 = dataBuffer[1];
				unsigned char byte11 = dataBuffer[2];
				unsigned char byte12 = dataBuffer[3];
				float floatingPoint = byte9 | (byte10<<8) | (byte11<<16) | (byte12<<24);
				std::cout<<"\tFloating Point: " << floatingPoint <<std::endl;
*/

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
