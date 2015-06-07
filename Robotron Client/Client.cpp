//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CClient.cpp
// Description : Client functionality
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "Client.h"

CClient::CClient()
{
}

CClient::~CClient()
{
	closesocket(m_hClientSocket);
}

bool CClient::Initialise()
{
	//Initialize member variables
	m_bWaitOnRecieve = false;
	
	//Free up the memory
	bool bSuccess = false;
	ZeroMemory(&m_ClientSocketAddress, sizeof(m_ClientSocketAddress));
	ZeroMemory(&m_ServerSocketAddress, sizeof(m_ServerSocketAddress));

	//Initiates use of the Winsock DLL by a process
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		int error = WSAGetLastError();
		return false;
	}

	//Loop Through set amount of ports until socket successfully bound
	for (unsigned int uiPort = NetworkValues::DEFAULT_CLIENT_PORT;
		 uiPort <= NetworkValues::MAX_CLIENT_PORT;
		 uiPort++)
	{
		//Create a socket object
		m_hClientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		//check if its been correctly 
		if (INVALID_SOCKET == m_hClientSocket)
		{
			int error = WSAGetLastError();
			continue;
		}

		//Initialise port values
		m_ClientSocketAddress.sin_family = AF_INET;
		m_ClientSocketAddress.sin_port = htons(uiPort);
		m_ClientSocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

		//Try and bind the socket to the current port number
		if (bind(m_hClientSocket, reinterpret_cast<sockaddr*>(&m_ClientSocketAddress), sizeof(m_ClientSocketAddress)) == 0)
		{
			//Set Client socket to be able to broadcast
			bool bBroadcastVal = true;
			if (	setsockopt(m_hClientSocket,
					SOL_SOCKET,
					SO_BROADCAST,
					(char*)&bBroadcastVal,
					sizeof(bBroadcastVal)) == -1)
			{
				//Error Setting client socket to be able to broadcast
				closesocket(m_hClientSocket);
				//Try another port number
				continue;
			}
			
			//Set success to true
			bSuccess = true;
			break;
		}
		else
		{
			//Unable to bind, close the created socket
			closesocket(m_hClientSocket);
		}

		//if we got to this point it meant we have failed to bind
		//Therefore try another port number
	}

	//setup address structure
	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_port = htons(NetworkValues::DEFAULT_SERVER_PORT);
	//m_ServerSocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	inet_pton(AF_INET, NetworkValues::ipAddUPD, &m_ServerSocketAddress.sin_addr);
	
	//Return success
	return bSuccess;
}

bool CClient::SendData(ServerDataPacket* _pDataToSend)
{
	ServerDataPacket packetToSend = *_pDataToSend;
	int iBytesToSend = sizeof(packetToSend) + 1;
		
	//Convert server data packet to a char pointer so that it is able to be sent
	char* cpPacketToSend = new char[iBytesToSend];
	cpPacketToSend = reinterpret_cast<char*>(&packetToSend);

	//Send data
	int iNumSendBytes = sendto( m_hClientSocket,									// socket to send through.
							cpPacketToSend,											// data to send
							iBytesToSend,											// number of bytes to send
							0,														// flags
							reinterpret_cast<sockaddr*>(&m_ServerSocketAddress),	// address to be filled with packet target
							sizeof(m_ServerSocketAddress)							// size of the above address struct.
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

bool CClient::ReceiveData(ClientDataPacket* _pReceivedData)
{
	int iBytesToReceive = sizeof(ClientDataPacket) + 1;

	//Convert server data packet to a char pointer so that it is able to be sent
	char* cpReceivedData = new char[iBytesToReceive];

	//calculate the size of client address
	int iSizeOfClientAddress = sizeof(m_ServerSocketAddress);

	struct timeval stucTimeValue;
	if (m_bWaitOnRecieve) //Wait till you receive something
	{
		// No timeout
		stucTimeValue.tv_sec = 0;
		stucTimeValue.tv_usec = 0;
		setsockopt(m_hClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&stucTimeValue, sizeof(stucTimeValue));
	}
	else  //Time out after 1 seconds
	{	
		// 1 sec timeout
		stucTimeValue.tv_sec = 1;
		stucTimeValue.tv_usec = 0000;
		setsockopt(m_hClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&stucTimeValue, sizeof(stucTimeValue));
	}
	
	int iNumReceivedBytes = recvfrom(																// pulls a packet from a single source...
										m_hClientSocket,											// client-end socket being used to read from
										cpReceivedData,												// incoming packet to be filled
										iBytesToReceive,											// length of incoming packet to be filled
										0,															// flags
										reinterpret_cast<sockaddr*>(&(m_ServerSocketAddress)),		// address to be filled with packet source
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
	*_pReceivedData = *(reinterpret_cast<ClientDataPacket*>(cpReceivedData));

	//Clean up memory
	delete cpReceivedData;
	cpReceivedData = 0;
	
	return true;
}
