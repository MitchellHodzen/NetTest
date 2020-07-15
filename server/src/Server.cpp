#include "Server.h"
#include "UdpSocket.h"
#include "MessageStructs.h"

Server::Server()
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		availableNetworkIds.push(i);
	}
}
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


bool Server::AttemptNewConnection(unsigned int address, unsigned int* networkId)
{
	//If the address is already connected, use that network ID
	if (addressNetworkIdMap.find(address) != addressNetworkIdMap.end())
	{
		//If not, map the address to the network ID
		*networkId = addressNetworkIdMap.at(address);
		return true;
	}

	//If there are no available network IDs, reject connection
	if (availableNetworkIds.empty())
	{
		std::cout<<"Connection from address " << address << " refused, no more network IDs available"<<std::endl;
		return false;
	}

	//If there are available network IDs, get the next available network ID and map it to the address
	unsigned int nextNetworkId = availableNetworkIds.top();
	availableNetworkIds.pop();
	addressNetworkIdMap.insert_or_assign(address, nextNetworkId);
	*networkId = nextNetworkId;
	return true;
}
void Server::DisconnectSession(unsigned int address)
{

	//Only disconnect if the address isn't connected
	if (addressNetworkIdMap.find(address) != addressNetworkIdMap.end())
	{
		//Get the network ID associated with that address
		unsigned int networkId = addressNetworkIdMap.at(address);
		//Remove value from address network ID map
		addressNetworkIdMap.erase(address);
		//Add the network ID back to the available network IDs
		availableNetworkIds.push(networkId);

		//Send a disconnect packet
	}

}