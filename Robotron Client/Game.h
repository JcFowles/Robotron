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
#include "../Shared/ProjectileObj.h"
#include "../Shared/Graphics_Defines.h"
#include "Terrain.h"
#include "CameraStatic.h"
#include "Camera.h"

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

	void DrawPlayers();
	void DrawEnemies();
	void DrawPowerUps();
	void RenderTeamScores(ClientDataPacket* _pClientPacket);
	void Process(ClientDataPacket* _pClientPacket);
	
	void ProcessPlayers(ClientDataPacket* _pClientPacket);
	void ProcessEnemies(ClientDataPacket* _pClientPacket);
	void ProcessProjectiles(ClientDataPacket* _pClientPacket);
	void ProcessPowerUps(ClientDataPacket* _pClientPacket);
	void ProcessCamera();
	//Render
	void RenderSingleFrame();

	void UpdatePlayerList(std::vector<std::string> _Players, ClientDataPacket* _pClientPacket);

	bool AddPlayer(ClientDataPacket* _pClientPacket, std::string _strPlayerToAdd);
	void AddAllPlayers(ClientDataPacket* _pClientPacket);
	void RemovePlayer(std::string _strLeftPlayer);
	

	/***********************
	* CreatePlayerAssest: Create the rendering assets for the player
	* @author: Jc Fowles
	* @return: int: Returns the material iD for saving, to reference when drawing
	********************/
	int CreatePlayerAssest();

	/***********************
	* CreateEnemyAssest: Create the rendering assets for the projectile
	* @author: Jc Fowles
	* @parameter: eEnemyTypes _EnemyType: The Type of enemy to create assets for
	* @return: int: Returns the material iD for saving, to reference when drawing
	********************/
	int CreateEnemyAssest(eEnemyTypes _EnemyType);

	/***********************
	* CreatePowerUpAssest: Create the rendering assets for the Power up
	* @author: Jc Fowles
	* @parameter:ePowerType _Type: The Type of power up to create assets for
	* @return: int: Returns the material iD for saving, to reference when drawing
	********************/
	int CreatePowerUpAssest(ePowerType _Type);
	
	/***********************
	* CreatePRojectileAssest: Create the rendering assets for the projectile
	* @author: Jc Fowles
	* @return: int: Returns the material iD for saving, to reference when drawing
	********************/
	int CreateProjectileAssest();
	
	/***********************
	* CreateCubeMesh: Creates a cube Mesh with origin in its very center
	* @author: Jc Fowles
	* @parameter: float _fCubeSize: Size of cube
	* @return: CMesh*: Pointer to a created Cube mesh
	********************/
	CMesh* CreateCubeMesh(float _fCubeSize, int iTextureID);
		
	/***********************
	* SetLightning: Set weather to strike lightning or not
	* @author: Jc Fowles
	* @parameter: bool _bLightning: Weather or not to strike lightning
	* @return: void: 
	********************/
	void SetLightning(bool _bLightning){ m_bLightning = _bLightning; };

	/***********************
	* CreateEnemy: Create an enemy
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to create a enemy
	* @return: void: 
	********************/
	void CreateEnemy(ClientDataPacket* _pClientPacket);

	/***********************
	* CreateEnemy: Create an Power UP
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to create a Power UP
	* @return: void:
	********************/
	void CreatePowerUp(ClientDataPacket* _pClientPacket);

	/***********************
	* CreateEnemy: Create an Projectile
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to create a Projectile
	* @return: void:
	********************/
	void CreateProjectile(ClientDataPacket* _pClientPacket);

	/***********************
	* DeleteEnemy: delete an enemy
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to delete a enemy
	* @return: void:
	********************/
	void DeleteEnemy(ClientDataPacket* _pClientPacket);

	/***********************
	* DeleteEnemy: delete an Power UP
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to delete a Power UP
	* @return: void:
	********************/
	void DeletePowerUp(ClientDataPacket* _pClientPacket);

	/***********************
	* DeleteProjectile: Delete an Projectile
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet containing info needed to delete a Projectile
	* @return: void:
	********************/
	void DeleteProjectile(ClientDataPacket* _pClientPacket);
		
	/***********************
	* ToggleCamera: Toggle camera between first and third person camera
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void ToggleCamera();

	
	
private:
	//Disallowing copies and extra constructions

	/***********************
	* CGame: Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CGame();

	/***********************
	* CGame: Copy Constructor of the game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CGame(const CGame& _kr);

	/***********************
	* operator=: Overloaded assignment operator
	* @author: Jc Fowles
	* @return:
	********************/
	CGame& operator= (const CGame& _kr);

	//member variables
private:
	
	IRenderer* m_pRenderManager;
		
	static CGame* s_pGame;
	
	std::map<std::string, CPlayerObj*>* m_plistPlayers;
	
	int m_iDirectionID;
	int m_iLightningID;
	
	std::map<UINT, CEnemyObj*>* m_pListEnemies;
	std::map<UINT, CPowerUpObj*>* m_pListPowerUps;
	std::map<UINT, CProjectileObj*>* m_pListBullets;

	int m_iPlayerMaterialID;
	std::map<eEnemyTypes, UINT>* m_iEnemyIDs;
	std::map<ePowerType, UINT>* m_iPowerUpIDs;
	int m_iBulletMaterialID;
	
	std::string m_strPlayerName;
	CPlayerObj* m_pPlayerAvatar;
	
	CMesh* m_pPlayerMesh;
	CMesh* m_pProjectileMesh;
	std::map<eEnemyTypes, CMesh* >* m_pEnemyMesh;
	std::map<ePowerType, CMesh* >* m_pPowerUpMesh;
			
	CTerrain* m_pTerrain;
	CCameraStatic* m_pCamera;
	CCamera* m_pDebugCamera;
		
	bool m_bLightning;

	int m_iNumberPlayers;
	int m_iIndexOfClientPlayer;

	//Testing
	ClientDataPacket* m_pClientPacket;

	bool bToggle;
	bool bTab; // TO DO

	bool m_bPauseItemSelected;
	std::string m_strPauseItem;

};

#endif //__CLIENT_GAME_H__
