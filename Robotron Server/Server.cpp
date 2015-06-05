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
//Delete m_pServerPacketQueue
}

bool CServer::Initialise()
{
	//Free up the memeory
	ZeroMemory(&m_ServerSocketAddress, sizeof(m_ServerSocketAddress));
	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		int error = WSAGetLastError();
		return false;
	}

	//Create a socket object
	m_hServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//check if its been correctly 
	if (INVALID_SOCKET == m_hServerSocket)
	{
		//Failure to create socket 
		//return failure
		return false;
	}

	//setup address structure
	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_port = htons(NetworkValues::DEFAULT_SERVER_PORT);
	m_ServerSocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

	//Try and bind the server socket
	if (bind(m_hServerSocket, reinterpret_cast<sockaddr*>(&m_ServerSocketAddress), sizeof(m_ServerSocketAddress)) == 0)
	{
		//Set server socket to be able to braodcast
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
			//and then return failure
			return false;
		}
	}
	else
	{
		//failed to bind close the created socket
		closesocket(m_hServerSocket);
		//and then return failure
		return false;
	}


	//Return succes
	m_bIsActive = true;
	return true;
}

bool CServer::SendData(ClientDataPacket* _pDataToSend)
{
	//TO DO : SEND TO ALL 
	//Create and get size of the packet to be sent
	ClientDataPacket packetToSend = *_pDataToSend;
	int iBytesToSend = sizeof(packetToSend) + 1;

	//Convert server data packet to a char pointer so that it is able to be sent
	char* cpPacketToSend = new char[iBytesToSend];
	cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

	//Send data
	int iNumSendBytes = sendto(m_hServerSocket,											// socket to send through.
								cpPacketToSend,											// data to send
								iBytesToSend,											// number of bytes to send
								0,														// flags
								reinterpret_cast<sockaddr*>(&m_ClientSocketAddress),	// address to be filled with packet target
								sizeof(m_ClientSocketAddress)							// size of the above address struct.
								);

	//check if data has been sent
	if (iBytesToSend != iNumSendBytes)
	{
		//There was an error sending data from client to server
		int iError = WSAGetLastError();
		return false;
	}

	return true;
}

bool CServer::ReceiveData(ServerDataPacket* _pReceivedData)
{
	int iBytesToReceive = sizeof(ServerDataPacket) + 1;

	//Convert server data packet to a char pointer so that it is able to be sent
	char* cpReceivedData = new char[iBytesToReceive];
	
	//calculate the size of client address
	int iSizeOfClientAddress = sizeof(m_ClientSocketAddress);

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
		//No data was recieved
		int iError = WSAGetLastError();
		return false;
	}

	//convert back to a Server Data Packet
	*_pReceivedData = *(reinterpret_cast<ServerDataPacket*>(cpReceivedData));
	
	//Clean up memory
	delete cpReceivedData;
	cpReceivedData = 0;
		
	return true;

}