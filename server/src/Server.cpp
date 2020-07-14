#include "Server.h"
#include "UdpSocket.h"
#include "MessageStructs.h"

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
				MSG_TEXT messagePacket(dataBuffer, bufferSize);
				std::cout<<"\tMessage Type: " << messagePacket.messageType <<std::endl;
				std::cout<<"\tNetwork ID: " << messagePacket.networkId <<std::endl;
				std::cout<<"\tText Buffer Length: " << messagePacket.textBufferLength <<std::endl;
				std::cout<<"\tText: " << messagePacket.text << std::endl;

				if (udpSocket.SetSendAddress(fromAddr, fromPort))
				{
					MSG_ACK ack(messagePacket.networkId);
					std::cout<<"\tAcking message" << std::endl;
					if(!udpSocket.Send(&ack, sizeof(MSG_ACK)))
					{
						std::cout<<"Response unsuccessful"<<std::endl;
					}
				}
			}
		}
	}
}
