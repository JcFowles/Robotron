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


class CGame
{
public:
	//Singleton Methods
	static CGame& GetInstance();
	static void DestroyInstance();

	~CGame(void);

	bool Initialise(std::vector<PlayerStates> _Players, ClientDataPacket* _pClientPacket);

	void Process(ServerDataPacket* _pServerPacket, ClientDataPacket* _pClientPacket);
	void ProcessInput(float _fDt, ServerDataPacket* _pServerPacket);

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
			
	int m_iNumberPlayers;

	UINT m_uiStage;


};

#endif //__SERVER_GAME_H__