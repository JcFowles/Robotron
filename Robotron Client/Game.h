//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CGame.h
// Description : Header file containing declarations for CGame Class (Client side)
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CLIENT_GAME_H__
#define __CLIENT_GAME_H__

//Library include

//Local includes
#include "../Shared/Network_Defines.h"
#include "../Shared/Clock.h"
#include "../Shared/IRenderer.h"
#include "../Shared/Mesh.h"
#include "../Shared/PlayerObj.h"
#include "../Shared/Graphics_Defines.h"
#include "Terrain.h"
#include "CameraStatic.h"

//TO DO: comment headers

class CGame
{
public:

	//Singleton Methods
	static CGame& GetInstance();
	static void DestroyInstance();

	~CGame(void);

	bool Initialise(IRenderer* _RenderManager, std::vector<PlayerStates> _Players, int _iIndexOfClientPlayer);

	void Draw();

	void Process(ClientDataPacket* _pClientPacket);
	//void ProcessInput(float _fDt);

	//Render
	void RenderSingleFrame();

	/***********************
	* CreateCubeMesh: Creates a cube Mesh with origin in its very center
	* @author: Jc Fowles
	* @parameter: float _fCubeSize: Size of cube
	* @return: CMesh*: Pointer to a created Cube mesh
	********************/
	CMesh* CreatePlayerMesh(float _fCubeSize, int iTextureID);

private:
	//Disallowing copies and extra constructions
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);

	//member variables
private:
	
	IRenderer* m_pRenderManager;
		
	static CGame* s_pGame;
	
	std::map<std::string, CPlayerObj>* m_plistPlayers;
	CPlayerObj* m_pPlayerAvatar;

	CMesh* m_pPlayerMesh;

	CTerrain* m_pTerrain;
	CCameraStatic* m_pCamera;

	float m_fPlayerSize;
	int m_iNumberPlayers;
	int m_iIndexOfClientPlayer;
};

#endif //__CLIENT_GAME_H__
