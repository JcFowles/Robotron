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
#include "../Shared/EnemyObj.h"
#include "../Shared/PowerUpObj.h"

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

	bool Initialise(IRenderer* _RenderManager, std::string _ControllingPlayer);

	void Draw();

	void Process(ClientDataPacket* _pClientPacket);
	//void ProcessInput(float _fDt);

	//Render
	void RenderSingleFrame();

	void UpdatePlayerList(std::vector<std::string> _Players, ClientDataPacket* _pClientPacket);

	bool AddPlayer(ClientDataPacket* _pClientPacket, std::string _strPlayerToAdd);
	void AddAllPlayers(ClientDataPacket* _pClientPacket);
	void RemovePlayer(std::string _strLeftPlayer);
	void CreateEnemyLust();

	int CreatePlayerAssest();
	int CreateEnemyAssest(eEnemyTypes _EnemyType);
	int CreatePowerUpAssest(ePowerType _Type);
	void SpawnWave(ClientDataPacket* _pClientPacket);

	/***********************
	* CreateCubeMesh: Creates a cube Mesh with origin in its very center
	* @author: Jc Fowles
	* @parameter: float _fCubeSize: Size of cube
	* @return: CMesh*: Pointer to a created Cube mesh
	********************/
	CMesh* CreateCubeMesh(float _fCubeSize, int iTextureID);

	bool m_bLightning;

private:
	//Disallowing copies and extra constructions
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);

	//member variables
private:
	
	IRenderer* m_pRenderManager;
		
	static CGame* s_pGame;
	
	std::map<std::string, CPlayerObj*>* m_plistPlayers;
	CMesh* m_pPlayerMesh;
	int m_iPlayerMaterialID;


	int m_iDirectionID;
	int m_iLightningID;


	std::map<UINT, CEnemyObj*>* m_pListEnemies;
	std::map<UINT, CPowerUpObj*>* m_pListPowerUps;

	std::map<eEnemyTypes, UINT>* m_iEnemyIDs;
	std::map<ePowerType, UINT>* m_iPowerUpIDs;


	std::string m_strPlayerName;
	CPlayerObj* m_pPlayerAvatar;
	
	
	float m_fPlayerSize;
	
	std::map<eEnemyTypes, CMesh* >* m_pEnemyMesh;
	std::map<ePowerType, CMesh* >* m_pPowerUpMesh;
	
	CMesh* m_pLustMesh;
	CMesh* m_pWrathMesh;
	CMesh* m_pSlothMesh;
	
	CTerrain* m_pTerrain;
	CCameraStatic* m_pCamera;

	
	//bool m_bLightning;

	int m_iNumberPlayers;
	int m_iIndexOfClientPlayer;
};

#endif //__CLIENT_GAME_H__
