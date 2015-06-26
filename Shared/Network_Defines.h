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
	unsigned const MAX_CLIENT_PORT = 60010; // 4 more than max users, why?? potential broken port maybe?
	unsigned const MAX_USERS = 6;

	unsigned const MAX_ENEMYS = 100; 
	unsigned const MAX_POWERUPS = 10;
	unsigned const MAX_PROJECTILE = 100;

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
	PT_UPDATE,

	PT_CREATE_ENEMY,
	PT_CREATE_POWERUP,
	PT_CREATE_PROJECTILE,

	PT_DELETE_ENEMY,
	PT_DELETE_POWERUP,
	PT_DELETE_PROJECTILE,

};

struct SteeringStates
{
	float3 f3TargetPosition;
	float fTargetSpeed;
	float3 f3TargetVelocity;

	//float3 f3Target;
	float3 f3Velocity;
	float3 f3Acceleration;

	float fSize;
	float fMaxSpeed;
	float fMaxForce;
	float fMaxAccel;
	float fWanderAngle;

};

struct PlayerStates
{
	char cPlayerName[NetworkValues::MAX_NAME_LENGTH + 1];
	UINT uiPlayerID;

	bool bAlive;

	float3 f3Positions;
	float3 f3Direction;
	float3 f3Velocity;
	float3 f3Acceleration;
	
	float fMaxSpeed;
	float fMaxForce;
	
	//Collision Box
	BoundingBox BBox;

	UINT uiScore;
	UINT uiScoreCheck;
	int iHealth;
	int iMaxHealth;
	int iDamage; 
	
	float fFireRate;
	
	UINT iNumEnemies;
	UINT iWaveNumber;

	int iLifeCount;
};

struct EnemyStates
{
	UINT uiEnemyID;
	eEnemyTypes Etype;
	float fRateOfDamage;

	char cTargetName[NetworkValues::MAX_NAME_LENGTH + 1];
	
	float3 f3Positions;
	float3 f3Direction;

	SteeringStates SteeringInfo;

	//Collision Box
	BoundingBox BBox; 

	float fHealth;
	UINT uiDamage;
	UINT uiPoints;
	
};

struct PowerUpStates
{
	UINT uiPowUpID;
	ePowerType Etype;

	float3 f3Positions;
	float3 f3Direction;
	
	SteeringStates SteeringInfo;

	//Collision Box
	BoundingBox BBox;
	
	UINT uiPoints;
};


struct ProjectileStates
{
	UINT uiProjectileID;
	//UINT uiOwnerID;
	char cOwnerName[NetworkValues::MAX_NAME_LENGTH + 1];

	float fMaxSpeed;
	float3 f3Velocity;
	float3 f3Acceleration;
	float3 f3Positions;
	float3 f3Direction;
		
	//Collision Box
	BoundingBox BBox;

	int iDamage;

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
	PowerUpStates PowUpInfo[NetworkValues::MAX_POWERUPS];
	ProjectileStates ProjectilesInfo[NetworkValues::MAX_PROJECTILE];

	EnemyStates singleEnemyInfo;
	PowerUpStates singlePowUpInfo;
	ProjectileStates singleProjectileInfo;

	UINT iNumPowerUps; 
	UINT iNumEnemies;
	UINT iNumProjectiles;
	UINT iNumPlayers;
	UINT iWaveNumber;
}; 



#define VALIDATE(a) if (!a) return (false)

#endif //__NETWORK_DEFINES_H__


