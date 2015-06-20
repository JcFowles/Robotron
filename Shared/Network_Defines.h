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

#ifndef __NETWORK_DEFINES_H__
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
#include "../Shared/App_Defines.h"

namespace NetworkValues
{
	unsigned const DEFAULT_SERVER_PORT = 60995;
	unsigned const MAX_SERVER_PORT = 61000;
	unsigned const DEFAULT_CLIENT_PORT = 60000;
	unsigned const MAX_CLIENT_PORT = 60020; // 4 more than max users, why?? potential broken port maybe?
	unsigned const MAX_USERS = 6;

	unsigned const MAX_ENEMYS = 30; //TO DO: Calculate fancy formula, using increase per player and waves and min shit
	
	//IP address of UDP server
	const char ipAddUPD[INET_ADDRSTRLEN] = "127.0.0.1";
	unsigned const MAX_CHAR_LENGTH = 255;
	unsigned const MAX_NAME_LENGTH = 10; 
}

enum ePacketType
{
	PT_DEFAULT,
	PT_CREATE,
	PT_FIND,
	PT_JOIN_REQUEST,
	PT_CLIENT_JOINED,
	PT_ACTIVE,
	PT_GAME_START,
	PT_LEAVE,
	PT_QUIT,

	PT_INPUT, 
	PT_UPDATE
};

struct PlayerStates
{
	char cPlayerName[NetworkValues::MAX_NAME_LENGTH + 1];
	UINT uiPlayerID;

	float3 f3Positions;
	float3 f3Velocity;
	float3 f3Direction;
	float3 f3Acceleration;
	
	float fMaxSpeed;
	float fMaxForce;
	//TO DO: Collision box
	//TO DO: SCORE
	//TO DO: Health
	//Other states ie shooting, health, points etc
};

struct EnemyStates
{
	UINT uiEnemyID;
	eEnemyTypes Etype;

	float3 f3Positions;
	float3 f3Velocity;
	float3 f3Direction;
	float3 f3Acceleration;

	//PlayerStates f3TagetPlayer;
	float3 f3Target;

	float fMaxSpeed;
	float fMaxForce;
	float fMaxAccel;


	//TO DO: everything else
};

struct ClientInfo
{
	sockaddr_in clientSocAddr;
	sockaddr_in ServerSocAddr;
	char cUserName[NetworkValues::MAX_NAME_LENGTH + 1];
	bool bActive;
	
};

//Data structure to send to Server received 
struct ServerDataPacket
{
	char cText[NetworkValues::MAX_CHAR_LENGTH];
	bool bSuccess;
	ePacketType packetType;
	ClientInfo clientInfo;
	InputStates PlayerInputs;
};

struct ServerInfo
{
	//If you add anything to this search for
	/*<SERVER_INFO>*/
	char cServerName[NetworkValues::MAX_NAME_LENGTH + 1];
	char cHostName[NetworkValues::MAX_NAME_LENGTH + 1];
	ClientInfo activeClientList[NetworkValues::MAX_USERS];
	UINT iNumClients;
	sockaddr_in serverSocAddr;
};

//Data structure to send to client
struct ClientDataPacket
{
	char cText[NetworkValues::MAX_CHAR_LENGTH];
	bool bSuccess;
	ePacketType packetType;
	ServerInfo serverInfo;
	
	PlayerStates PlayerInfo[NetworkValues::MAX_USERS];
	EnemyStates EnemyInfo[NetworkValues::MAX_ENEMYS];
	UINT iNumEnemies;
}; 



#define VALIDATE(a) if (!a) return (false)

#endif //__NETWORK_DEFINES_H__


