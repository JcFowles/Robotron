//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CGame.h
// Description : Header file containing declarations for CGame Class (Server side)
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __SERVER_GAME_H__
#define __SERVER_GAME_H__

//Library Includes

//Local Includes
#include "../Shared/Network_Defines.h"
#include "../Shared/Clock.h"
#include "../Shared/IRenderer.h"
#include "../Shared/PlayerObj.h"
#include "SteerLib.h"


class CGame
{
public:
	//Singleton Methods
	static CGame& GetInstance();
	static void DestroyInstance();

	~CGame(void);

	bool Initialise();

	void Process(ServerDataPacket* _pServerPacket, ClientDataPacket* _pClientPacket);
	void ProcessInput(float _fDt, ServerDataPacket* _pServerPacket);


	
	void AddPlayer(std::string _strUser);
	void RemovePlayer(std::string _strLeftPlayer);
	void UpdatePlayers(ClientDataPacket* _pClientPacket);
	void UpdateEnemies(ClientDataPacket* _pClientPacket);
	void UpdateLust(EnemyStates* _Enemy);
	
	
	void SetPlayerStates(ClientDataPacket* _pDataToSend);
	void SetEnemyStates(ClientDataPacket* _pDataToSend);
	void SetPowUpStates(ClientDataPacket* _pDataToSend);
	
	void SpawnWave();
private:
	//Disallowing copies and extra constructions
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);
	
	//member variables
private:

	//IRenderer* m_pRenderManager;

	static CGame* s_pGame;
	CClock* m_pClock;

	std::map<std::string, PlayerStates>* m_plistPlayers;
	std::map<UINT, EnemyStates>* m_plistEnemies;
	std::map<UINT, PowerUpStates>* m_pListPowerUps;
			
	int m_iNumberPlayers;

	UINT m_uiStage;

	UINT  m_uiNextObjID;

};

#endif //__SERVER_GAME_H__