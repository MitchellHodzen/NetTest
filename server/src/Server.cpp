#include "Server.h"
#include "UdpSocket.h"
#include "MessageStructs.h"

Server::Server(unsigned short port)
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		availableNetworkIds.push(i);
	}
	udpSocket = new UdpSocket(port);
}
void Server::StartServer()
{
	if (udpSocket->Initialize())
	{
		//receive messages
		std::cout<<"Listening for messages"<<std::endl;

		unsigned char dataBuffer[255];
		unsigned int bufferSize = sizeof(dataBuffer);

		unsigned int fromAddr;
		unsigned short fromPort;

		while(true)
		{
			bool receivedMessage = udpSocket->RecieveMessage(dataBuffer, bufferSize, &fromAddr, &fromPort);

			if (receivedMessage)
			{

				MSG_TYPE messageType = MESSAGE::DetermineMessageType(dataBuffer, bufferSize);

				Address address;
				address.address = fromAddr;
				address.port = fromPort;

				switch(messageType)
				{
					case MSG_TYPE::ACK:
					{
						std::cout<<"Ack packet recieved from " << fromAddr << ":" << fromPort <<std::endl;
						break;
					}
					case MSG_TYPE::DISCONNECT:
					{
						std::cout<<"Disconnected packet recieved from " << fromAddr << ":" << fromPort <<std::endl;
						break;
					}
					case MSG_TYPE::CONNECT_REQUEST:
					{
						std::cout<<"Connect Request packet recieved from " << fromAddr << ":" << fromPort <<std::endl;
						unsigned int networkId;
						bool failedConnection = false;

						if (AttemptNewConnection(address, &networkId))
						{
							AcceptConnection(address, networkId);
						}
						else
						{
							RejectConnection(address);
						}
						break;
					}
					case MSG_TYPE::TEXT:
					{
						MSG_TEXT messagePacket(dataBuffer, bufferSize);
						std::cout<<"Message packet recieved from " << fromAddr << ":" << fromPort << " with ID " << messagePacket.networkId <<std::endl;
						if (UserConnected(address, messagePacket.networkId))
						{
							std::cout<<"\tText Buffer Length: " << messagePacket.textBufferLength <<std::endl;
							std::cout<<"\tText: " << messagePacket.text << std::endl;

							if (udpSocket->SetSendAddress(fromAddr, fromPort))
							{
								MSG_ACK ack(messagePacket.networkId);
								std::cout<<"\tAcking message" << std::endl;
								if(!udpSocket->Send(&ack, sizeof(MSG_ACK)))
								{
									std::cout<<"\tResponse unsuccessful"<<std::endl;
								}
							}
						}
						else
						{
							std::cout<<"Message rejected, user not connected or has incorrect message ID"<<std::endl;
						}
						break;
					}
					default:
						break;
				}
			}
		}
	}
}


bool Server::AttemptNewConnection(Address address, unsigned int* networkId)
{
	//If the address is already connected, use that network ID
	std::unordered_map<Address, unsigned int>::iterator idIterator = addressNetworkIdMap.find(address);
	if (idIterator != addressNetworkIdMap.end())
	{
		//If not, map the address to the network ID
		*networkId = idIterator->second;
		return true;
	}

	//If there are no available network IDs, reject connection
	if (availableNetworkIds.empty())
	{
		std::cout<<"Connection from address " << address.address << ":" << address.port << " refused, no more network IDs available"<<std::endl;
		return false;
	}

	//If there are available network IDs, get the next available network ID and map it to the address
	unsigned int nextNetworkId = availableNetworkIds.top();
	availableNetworkIds.pop();
	addressNetworkIdMap.insert_or_assign(address, nextNetworkId);
	*networkId = nextNetworkId;
	return true;
}

bool Server::UserConnected(Address address, unsigned int networkId)
{
	//If the address is already connected, use that network ID
	std::unordered_map<Address, unsigned int>::iterator idIterator = addressNetworkIdMap.find(address);
	if (idIterator == addressNetworkIdMap.end())
	{
		//If the address isn't connected, return false
		return false;
	}
	//If not, map the address to the network ID
	unsigned int addressNetworkId = idIterator->second;

	if (networkId != addressNetworkId)
	{
		//If address is connected, but has a different network ID, reject it
		return false;
	}

	//If the address is connected, and the network ID is the same, accept
	return true;
}

void Server::AcceptConnection(Address address, unsigned int networkId)
{
	if (udpSocket->SetSendAddress(address.address, address.port))
	{
		MSG_CONNECT_RESPONSE connectResponse(networkId, CONNECT_RESP::ACCEPTED);
		if(udpSocket->Send(&connectResponse, sizeof(MSG_CONNECT_RESPONSE)))
		{
			std::cout<<"Successfully connected " << address.address << ":" << address.port << " with network ID " << networkId << std::endl;
		}
		else
		{
			std::cout<<"Failed to send connection response"<<std::endl;
		}
	}
}
void Server::RejectConnection(Address address)
{
	FreeNetworkId(address);
	//Send a rejection packet
	if (udpSocket->SetSendAddress(address.address, address.port))
	{
		std::cout<<"\tSending reject connection packet" << std::endl;
		MSG_CONNECT_RESPONSE connectRejectResponse(0, CONNECT_RESP::REJECTED);
		if(!udpSocket->Send(&connectRejectResponse, sizeof(MSG_CONNECT_RESPONSE)))
		{
			std::cout<<"Unable to send rejection packet"<<std::endl;
		}
	}
}
void Server::DisconnectSession(Address address)
{
	FreeNetworkId(address);
	//Send a disconnect packet
	if (udpSocket->SetSendAddress(address.address, address.port))
	{
		MSG_DISCONNECT disconnectPacket;
		std::cout<<"\tSending disconnect packet" << std::endl;
		if(!udpSocket->Send(&disconnectPacket, sizeof(MSG_DISCONNECT)))
		{
			std::cout<<"Response unsuccessful"<<std::endl;
		}
	}

}

void Server::FreeNetworkId(Address address)
{
	//Only disconnect if the address isn't connected
	std::unordered_map<Address, unsigned int>::iterator idIterator = addressNetworkIdMap.find(address);
	if (idIterator != addressNetworkIdMap.end())
	{
		//Get the network ID associated with that address
		unsigned int networkId = idIterator->second;
		//Remove value from address network ID map
		addressNetworkIdMap.erase(address);
		//Add the network ID back to the available network IDs
		availableNetworkIds.push(networkId);
	}
}