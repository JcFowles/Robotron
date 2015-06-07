//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Network_Defines.h 
// Description : Header file containing Network Defines, Structures, Enum, ect
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once
#if !defined(__NETWORK_DEFINES_H__)
#define __NETWORK_DEFINES_H__

#pragma comment( lib, "winmm.lib" )
#pragma comment(lib, "Ws2_32.lib")

//library includes
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <queue>
#include <map>

//Local Includes
#include "../Shared/MySemaphore.h"

namespace NetworkValues
{
	unsigned const DEFAULT_SERVER_PORT = 61000;
	unsigned const DEFAULT_CLIENT_PORT = 60000;
	unsigned const MAX_CLIENT_PORT = 60999;
	unsigned const MAX_USERS = 16;
	//ip address of udp server
	const char ipAddUPD[INET_ADDRSTRLEN] = "127.0.0.1";
	unsigned const MAX_CHAR_LENGTH = 255;
}

enum ePacketType
{
	PT_CREATE,
	PT_COMMAND,
	PT_RESPONSE
};

//Data structure to send to Server received 
struct ServerDataPacket
{
	ePacketType packetType;
	sockaddr_in socReceivedFrom;
	char cText[NetworkValues::MAX_CHAR_LENGTH];
	bool bSuccess;
	
};

//Data structure to send to client
struct ClientDataPacket
{
	ePacketType packetType;
	sockaddr_in socReceivedFrom;
	char cText[NetworkValues::MAX_CHAR_LENGTH];
	bool bSuccess;
};



#define VALIDATE(a) if (!a) return (false)

#endif //__NETWORK_DEFINES_H__


