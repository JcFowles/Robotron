//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CServer.cpp
// Description : Server functionality //TO DO
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "Server.h"

CServer::CServer()
{
}

CServer::~CServer()
{
	closesocket(m_hServerSocket);

	delete m_pMapClients;
	m_pMapClients = 0;
	delete m_HostClientInfo;
	m_HostClientInfo = 0;

}

bool CServer::Initialise()
{
	//Initialise member variables
	m_bWaitOnRecieve = false;
	//Initialise the map of client addresses
	m_pMapClients = new std::map < std::string, sockaddr_in > ;
	m_HostClientInfo = new std::pair < std::string, sockaddr_in > ;

	//Free up the memory
	ZeroMemory(&m_ServerSocketAddress, sizeof(m_ServerSocketAddress));
	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		int error = WSAGetLastError();
		return false;
	}

	bool bSuccess = false;

	//Loop Through set amount of ports until socket successfully bound
	for (unsigned int uiPort = NetworkValues::DEFAULT_SERVER_PORT;
		uiPort <= NetworkValues::MAX_SERVER_PORT;
		uiPort++)
	{
		//Create a socket object
		m_hServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		//check if its been correctly 
		if (INVALID_SOCKET == m_hServerSocket)
		{
			//Failure to create socket 
			int error = WSAGetLastError();
			//return failure
			bSuccess =  false;
			break;
		}

		//setup address structure
		m_ServerSocketAddress.sin_family = AF_INET;
		m_ServerSocketAddress.sin_port = htons(uiPort);
		m_ServerSocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

		//Try and bind the server socket
		if (bind(m_hServerSocket, reinterpret_cast<sockaddr*>(&m_ServerSocketAddress), sizeof(m_ServerSocketAddress)) == 0)
		{
			//Set server socket to be able to broadcast
			bool bBroadcastVal = true;
			if (setsockopt(m_hServerSocket,
				SOL_SOCKET,
				SO_BROADCAST,
				(char*)&bBroadcastVal,
				sizeof(bBroadcastVal)) == -1)
			{
				//Error Setting server socket to be able to broadcast
				//Close created socket
				closesocket(m_hServerSocket);
				//Try another port number
				continue;
			}

			//Set success to true
			bSuccess = true;
			break;
		}
		else
		{
			//failed to bind close the created socket
			closesocket(m_hServerSocket);
			//and then return failure
		}
		
		//if we got to this point it meant we have failed to bind
		//Therefore try another port number
	}

	//Return success
	m_bIsActive = true;
	return bSuccess;
}

bool CServer::SendData(ClientDataPacket* _pDataToSend)
{
	//Send to all clients 
	std::map< std::string, sockaddr_in>::iterator iterClient = m_pMapClients->begin();
	std::map< std::string, sockaddr_in>::iterator iterClientEnd = m_pMapClients->end();
	while (iterClient != iterClientEnd)
	{
		//Create and get size of the packet to be sent
		ClientDataPacket packetToSend = *_pDataToSend;
		int iBytesToSend = sizeof(packetToSend) + 1;

		//Convert server data packet to a char pointer so that it is able to be sent
		char* cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

		//Send data
		int iNumSendBytes = sendto( m_hServerSocket,										// socket to send through.
									cpPacketToSend,											// data to send
									iBytesToSend,											// number of bytes to send
									0,														// flags
									reinterpret_cast<sockaddr*>(&iterClient->second),		// address to be filled with packet target
									sizeof(iterClient->second)								// size of the above address struct.
									);

		//check if data has been sent
		if (iBytesToSend != iNumSendBytes)
		{
			//There was an error sending data from client to server
			int iError = WSAGetLastError();
			return false;
		}

		//Get next client
		iterClient++;
	}

	return true;
}

bool CServer::SendData(std::string _strUser, ClientDataPacket* _pDataToSend)
{
	//Find the client in the map of clients
	std::map< std::string, sockaddr_in>::iterator iterClient;
	iterClient = m_pMapClients->find(_strUser);

	//If found user found, then iterClients wont be the end of the map
	if (iterClient != m_pMapClients->end())
	{
		//Create and get size of the packet to be sent
		ClientDataPacket packetToSend = *_pDataToSend;
		int iBytesToSend = sizeof(packetToSend) + 1;

		//Convert server data packet to a char pointer so that it is able to be sent
		char* cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

		//Send data
		int iNumBytes = sendto(	m_hServerSocket,									// socket to send through.
								cpPacketToSend,										// data to send
								iBytesToSend,										// number of bytes to send
								0,													// flags
								reinterpret_cast<sockaddr*>(&iterClient->second),	// address to be filled with packet target
								sizeof(iterClient->second)							// size of the above address struct.
								);

		//check if data has been sent
		if (iBytesToSend != iNumBytes)
		{
			//There was an error sending data from client to server
			int iError = WSAGetLastError();
			return false;
		}

		//client was found and message was send
		return true;
	}
	else
	{
		//client was not found in the map
		return false;
	}
}

bool CServer::SendData(ClientDataPacket* _pDataToSend, sockaddr_in _socAddrToSendTo)
{
		//Create and get size of the packet to be sent
		ClientDataPacket packetToSend = *_pDataToSend;
		int iBytesToSend = sizeof(packetToSend) + 1;

		//Convert server data packet to a char pointer so that it is able to be sent
		char* cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

		//Send data
		int iNumBytes = sendto(m_hServerSocket,									// socket to send through.
			cpPacketToSend,										// data to send
			iBytesToSend,										// number of bytes to send
			0,													// flags
			reinterpret_cast<sockaddr*>(&_socAddrToSendTo),	// address to be filled with packet target
			sizeof(_socAddrToSendTo)							// size of the above address struct.
			);

		//check if data has been sent
		if (iBytesToSend != iNumBytes)
		{
			//There was an error sending data from client to server
			int iError = WSAGetLastError();
			return false;
		}

		//client was found and message was send
		return true;

}

bool CServer::ReceiveData(ServerDataPacket* _pReceivedData)
{
	int iBytesToReceive = sizeof(ServerDataPacket) + 1;

	//Convert server data packet to a char pointer so that it is able to be sent
	char* cpReceivedData = new char[iBytesToReceive];
	
	//calculate the size of client address
	int iSizeOfClientAddress = sizeof(m_ClientSocketAddress);

	struct timeval stucTimeValue;
	if (m_bWaitOnRecieve) //Wait till you receive something
	{
		// No timeout
		stucTimeValue.tv_sec = 0;
		stucTimeValue.tv_usec = 0;
		setsockopt(m_hServerSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&stucTimeValue, sizeof(stucTimeValue));
	}
	else  //Time out after 1 seconds
	{
		// 1 sec timeout
		stucTimeValue.tv_sec = 1;
		stucTimeValue.tv_usec = 0000;
		setsockopt(m_hServerSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&stucTimeValue, sizeof(stucTimeValue));
	}

	int iNumReceivedBytes = recvfrom(																// pulls a packet from a single source...
										m_hServerSocket,											// client-end socket being used to read from
										cpReceivedData,												// incoming packet to be filled
										iBytesToReceive,											// length of incoming packet to be filled
										0,															// flags
										reinterpret_cast<sockaddr*>(&(m_ClientSocketAddress)),		// address to be filled with packet source
										&(iSizeOfClientAddress)										// size of the above address struct.
										);

	if (iNumReceivedBytes < 0)
	{
		//No data was received
		int iError = WSAGetLastError();

		//Clean up memory
		delete cpReceivedData;
		cpReceivedData = 0;

		return false;
	}

	//convert back to a Server Data Packet
	*_pReceivedData = *(reinterpret_cast<ServerDataPacket*>(cpReceivedData));
	_pReceivedData->socReceivedFrom = m_ClientSocketAddress;

	//Clean up memory
	delete cpReceivedData;
	cpReceivedData = 0;
		
	return true;

}

bool CServer::Broadcast(ClientDataPacket* _pDataToSend)
{
	//initialize the sock address to broadcast on
	struct sockaddr_in socBroadcastAddress;
	socBroadcastAddress.sin_family = AF_INET;
	socBroadcastAddress.sin_addr.s_addr = INADDR_BROADCAST;

	//Create and get size of the packet to be sent
	ClientDataPacket packetToSend = *_pDataToSend;
	int iBytesToSend = sizeof(packetToSend) + 1;

	//Convert Client data packet to a char pointer so that it is able to be sent
	char* cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

	//Loop through possible client ports
	for (int iClientPort = NetworkValues::DEFAULT_CLIENT_PORT;
		iClientPort <= NetworkValues::MAX_CLIENT_PORT;
		iClientPort++)
	{
		//Set port number
		socBroadcastAddress.sin_port = htons(iClientPort);

		//Send data
		int iNumBytes = sendto( m_hServerSocket,										// socket to send through.
								cpPacketToSend,											// data to send
								iBytesToSend,											// number of bytes to send
								0,														// flags
								reinterpret_cast<sockaddr*>(&socBroadcastAddress),		// address to be filled with packet target
								sizeof(socBroadcastAddress)								// size of the above address struct.
								);

		if (iBytesToSend != iNumBytes)
		{
			//There was an error sending data from client to server
			int iError = WSAGetLastError();
			//Something went wrong with sending a packet
			return false;
		}

	}

	//Successfully broadcast
	return true;

}


bool CServer::AddUser(std::string _UserName, sockaddr_in _ClientAddress)
{
	std::pair<std::map<std::string, sockaddr_in>::iterator, bool> MapIterater;

	//Add passed in params to map of clients
	MapIterater = m_pMapClients->insert(std::pair<std::string, sockaddr_in>(_UserName, _ClientAddress));

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapIterater.second;
}