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
#include "IRenderer.h"
#include "Mesh.h"
#include "PlayerObj.h"
#include "EnemyObj.h"
#include "PowerUpObj.h"
#include "ProjectileObj.h"
#include "../Shared/Graphics_Defines.h"
#include "Terrain.h"
#include "CameraStatic.h"
#include "DebugCamera.h"

class CGame
{
public:

	//Singleton Methods
	
	/***********************
	* GetInstance: Returns a reference to the singleton CGame object, if one does not exist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a reference to the singleton game object
	********************/
	static CGame& GetInstance();

	/***********************
	* DestroyInstance: Destroys the singleton CGame object
	* @author: Jc Fowles
	* @return: void
	********************/
	static void DestroyInstance();

	/***********************
	* ~CGame: Destructor of the CGame instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CGame(void);
		
	/***********************
	* Initialise: Initialise the Game Instance
	* @author: Jc Fowles
	* @Parameter: IRenderer * _RenderManager: Render manager used to render all assest
	* @Parameter: std::string _ControllingPlayer: Name of the player who owns and controls this game
	* @return: bool: 
	********************/
	bool Initialise(IRenderer* _RenderManager, std::string _ControllingPlayer, bool _bSinglePLayer);
		
	/***********************
	* Draw: Draw all the assets of the game
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void Draw();

	/***********************
	* DrawPlayers: Draw the all the player assets of the game
	* @author: Jc Fowles
	* @return: void:
	********************/
	void DrawPlayers();

	/***********************
	* DrawEnemies: Draw the all the enemy assets of the game
	* @author: Jc Fowles
	* @return: void:
	********************/
	void DrawEnemies();

	/***********************
	* DrawPowerUps: Draw the all the power up assets of the game
	* @author: Jc Fowles
	* @return: void:
	********************/
	void DrawPowerUps();

	/***********************
	* RenderTeamScores: renders the score board to the screen
	* @author: Jc Fowles
	* @return: void:
	********************/
	void RenderTeamScores(ClientDataPacket* _pClientPacket);

	/***********************
	* Process: Process All the assets of the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket* _pClientPacket: Client packet used to update the assets
	* @return: void:
	********************/
	void Process(ClientDataPacket* _pClientPacket);
		
	/***********************
	* ProcessPlayers: Process All the player assets of the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Client packet used to update the assets
	* @return: void: 
	********************/
	void ProcessPlayers(ClientDataPacket* _pClientPacket);

	/***********************
	* ProcessEnemies: Process All the enemies assets of the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Client packet used to update the assets
	* @return: void:
	********************/
	void ProcessEnemies(ClientDataPacket* _pClientPacket);

	/***********************
	* ProcessProjectiles: Process All the projectiles assets of the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Client packet used to update the assets
	* @return: void:
	********************/
	void ProcessProjectiles(ClientDataPacket* _pClientPacket);

	/***********************
	* ProcessPowerUps: Process All the power ups assets of the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Client packet used to update the assets
	* @return: void:
	********************/
	void ProcessPowerUps(ClientDataPacket* _pClientPacket);

	/***********************
	* ProcessCamera: Process the camera view of the game
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessCamera();

	//Render
		
	/***********************
	* RenderSingleFrame: Process and draw a single frame
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void RenderSingleFrame();
		
	/***********************
	* AddPlayer: Add a player to the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Packet with the players infomation
	* @Parameter: std::string _strPlayerToAdd: Name of the player to add
	* @return: bool: 
	********************/
	bool AddPlayer(ClientDataPacket* _pClientPacket, std::string _strPlayerToAdd);
	
	/***********************
	* AddAllPlayers: Add all the players to the game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Packet with all the players infomation
	* @return: void: 
	********************/
	void AddAllPlayers(ClientDataPacket* _pClientPacket);
	
	/***********************
	* RemovePlayer: Remove a player from the game
	* @author: Jc Fowles
	* @Parameter: std::string _strLeftPlayer: the name of the player you are removing
	* @return: void: 
	********************/
	void RemovePlayer(std::string _strLeftPlayer);
	

	/***********************
	* CreatePlayerAssest: Create the rendering assets for the player
	* @author: Jc Fowles
	* @return: int: Returns the material iD for saving, to reference when drawing
	********************/
	int CreatePlayerAssest();
	//TO DO

	int CreateOverLayAssest();
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
	
	//TO DO
	bool GameOverCheck();

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
	//TO DO
	 void CreateOverLay();
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

	void CameraMove(float _fUnit);
	void CameraPitch(float _fUnit);
	void CameraYaw(float _fUnit);
	//TO DO:
	CDebugCamera* GetDebugCam() const { return m_pDebugCam; }
	void SetDebugCam(CDebugCamera* _DebugCam) { m_pDebugCam = _DebugCam; }

	bool GetDebug() const { return m_bDebug; }
	void SetDebug(bool _Debug) { m_bDebug = _Debug; }
	
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
	int m_iSpotLightID;
	
	
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
	CTerrain* m_pUnderLay;
	int m_pOverLayMatID;
	CMesh* m_pOverlayMesh;

	CCameraStatic* m_pCamera;
	CDebugCamera* m_pDebugCam;
	
	bool m_bDebug;
	
	bool m_bLightning;

	int m_iNumberPlayers;
	int m_iIndexOfClientPlayer;

	//Testing
	ClientDataPacket* m_pClientPacket;

	bool bToggle;

	bool m_bPauseItemSelected;
	std::string m_strPauseItem;

	bool m_bSinglePlayer;
};

#endif //__CLIENT_GAME_H__
