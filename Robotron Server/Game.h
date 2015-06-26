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
#include <fstream>

//Local Includes
#include "../Shared/Network_Defines.h"
#include "../Shared/Clock.h"
#include "SteerLib.h"


class CGame
{
public:
	//Singleton Methods
	
	/***********************
	* GetInstance: Get the game instance 
	* @author: Jc Fowles
	* @return: CGame&: 
	********************/
	static CGame& GetInstance();

	/***********************
	* DestroyInstance: Destroy the game instance
	* @author: Jc Fowles
	* @return: CGame&:
	********************/
	static void DestroyInstance();
		
	/***********************
	* ~CGame: Destructor
	* @author: Jc Fowles
	* @return: : 
	********************/
	~CGame(void);
		
	/***********************
	* Initialise: Initialise the game 
	* @author: Jc Fowles
	* @return: bool: Successful initialization
	********************/
	bool Initialise();
		
	/***********************
	* Process: Process the Game
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pClientPacket: Data packet to save data to send
	* @return: void: 
	********************/
	void Process(ClientDataPacket* _pClientPacket);

	/***********************
	* ProcessInput: Process the input
	* @author: Jc Fowles
	* @Parameter: ServerDataPacket * _pServerPacket: Data packet where inputs are found
	* @return: void:
	********************/
	void ProcessInput( ServerDataPacket* _pServerPacket);
		
	/***********************
	* AddPlayer: Add a player
	* @author: Jc Fowles
	* @Parameter: std::string _strUser: Name of player to add
	* @return: void: 
	********************/
	void AddPlayer(std::string _strUser);

	/***********************
	* RemovePlayer: Remove a player
	* @author: Jc Fowles
	* @Parameter: std::string _strLeftPlayer: name of player to remove
	* @return: void: 
	********************/
	void RemovePlayer(std::string _strLeftPlayer);
	
	/***********************
	* UpdatePlayers: Update players
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void UpdatePlayers();
	
	/***********************
	* UpdateEnemies: Update the enemies
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void UpdateEnemies();
	
	/***********************
	* UpdateLust: Update the lust enemy
	* @author: Jc Fowles
	* @Parameter: EnemyStates * _Enemy: Enemy to update
	* @return: void: 
	********************/
	void UpdateLust(EnemyStates* _Enemy);

	/***********************
	* UpdateSloth: Update the Sloth enemy
	* @author: Jc Fowles
	* @Parameter: EnemyStates * _Enemy: Enemy to update
	* @return: void:
	********************/
	void UpdateSloth(EnemyStates* _pEnemy);

	/***********************
	* UpdateSloth: Update the Wrath enemy
	* @author: Jc Fowles
	* @Parameter: EnemyStates * _Enemy: Enemy to update
	* @return: void:
	********************/
	void UpdateWrath(EnemyStates* _pEnemy);

	/***********************
	* UpdateProjectile: Update the projectile
	* @author: Jc Fowles
	* @return: void:
	********************/
	void UpdateProjectile();
	
	
	/***********************
	* UpdatePowerUp: Update the all the powers ups
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void UpdatePowerUp();

	/***********************
	* UpdatePowerUp: Update a single the powers up
	* @author: Jc Fowles
	* @Parameter: PowerUpStates* _pPowerUp: Power up to update
	* @return: void:
	********************/
	void UpdatePowerUp(PowerUpStates* _pPowerUp);
		
	/***********************
	* SetPlayerStates: Set the player states on the packet to send
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Packet to save date on
	* @return: void: 
	********************/
	void SetPlayerStates(ClientDataPacket* _pDataToSend);

	/***********************
	* SetEnemyStates: Set the enemy states on the packet to send
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Packet to save date on
	* @return: void:
	********************/
	void SetEnemyStates(ClientDataPacket* _pDataToSend);

	/***********************
	* SetPowUpStates: Set the power up states on the packet to send
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Packet to save date on
	* @return: void:
	********************/
	void SetPowUpStates(ClientDataPacket* _pDataToSend);

	/***********************
	* SetProjectileStates: Set the projectile states on the packet to send
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Packet to save date on
	* @return: void:
	********************/
	void SetProjectileStates(ClientDataPacket* _pDataToSend);
		
	/***********************
	* CheckCollision: Check if two bounding boxes are colliding
	* @author: Jc Fowles
	* @Parameter: BoundingBox _BBoxA: first bounding box
	* @Parameter: BoundingBox _BBoxB: Second bounding box
	* @return: bool: True if colliding
	********************/
	bool CheckCollision(BoundingBox _BBoxA, BoundingBox _BBoxB);
		
	/***********************
	* SpawnWave: Spawn a wave of enemies
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void SpawnWave();
	
	/***********************
	* CreateLust: Create enemy of type lust
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn enemy
	* @return: void:
	********************/
	void CreateLust(float3 _f3Pos);

	/***********************
	* CreateSloth: Create enemy of type Sloth
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn enemy
	* @return: void:
	********************/
	void CreateSloth(float3 _f3Pos);

	/***********************
	* CreateWrath: Create enemy of type Wrath
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn enemy
	* @return: void:
	********************/
	void CreateWrath(float3 _f3Pos);

	/***********************
	* SpawnPowerUp: Spawn power ups
	* @author: Jc Fowles
	* @return: void:
	********************/
	void SpawnPowerUp();

	/***********************
	* CreatePowerUpTen: Create power up of type
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn object
	* @return: void:
	********************/
	void CreatePowerUpTen(float3 _f3Pos);

	/***********************
	* CreatePowerUpTen: Create power up of type
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn object
	* @return: void:
	********************/
	void CreatePowerUpFifty(float3 _f3Pos);

	/***********************
	* CreatePowerUpTen: Create power up of type
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn object
	* @return: void:
	********************/
	void CreatePowerUpHundred(float3 _f3Pos);

	/***********************
	* DestroyAllPowerUps: Destroy all power ups
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: position to spawn object
	* @return: void:
	********************/
	void DestroyAllPowerUps();
		
	/***********************
	* SpawnProjectile: Spawn a projectile
	* @author: Jc Fowles
	* @Parameter: float3 _Direction: Direction it will be fired
	* @Parameter: float3 _Position: position It will spawn
	* @Parameter: std::string strOwner: Who shot the projectile
	* @Parameter: int _Damage: How much damage it will do
	* @return: bool: Successful
	********************/
	bool SpawnProjectile(float3 _Direction, float3 _Position, std::string strOwner, int _Damage);
		
	/***********************
	* RespawnPlayers: Respawn players
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void RespawnPlayers();
	
	/***********************
	* GetSinglePlayer: Get weather or not game is player in single player mode
	* @author: Jc Fowles
	* @return: bool: Weather or not the game is in single player mode
	********************/
	bool GetSinglePlayer() const { return m_bSinglePlayer; }
		
	/***********************
	* SetSinglePlayer: Set weather game is played in single player mode
	* @author: Jc Fowles
	* @Parameter: bool _SinglePlayer: value set single player mode too
	* @return: void: 
	********************/
	void SetSinglePlayer(bool _SinglePlayer) { m_bSinglePlayer = _SinglePlayer; }

	
	/***********************
	* GetNextCreatedEnemy: Returns the next enemy to create
	* @author: Jc Fowles
	* @Parameter: EnemyStates * _pEnemyState: EnemyState pointer to save the info used to create the new one
	* @return: bool: true if enemy needs to be created or false if no enemies need to be created
	********************/
	bool GetNextCreatedEnemy(EnemyStates* _pEnemyState);

	/***********************
	* GetNextCreatedPowerUp: Returns the next power up to create
	* @author: Jc Fowles
	* @Parameter: PowerUpStates * _pPowUpState: PowerUpStates pointer to save the info used to create the new one
	* @return: bool: true if power up needs to be created or false if no power up need to be created
	********************/
	bool GetNextCreatedPowerUp(PowerUpStates* _pPowUpState);

	/***********************
	* GetNextCreatedProjectile: Returns the next Projectile to create
	* @author: Jc Fowles
	* @Parameter: ProjectileStates* _pProjectileState: ProjectileStates pointer to save the info used to create the new one
	* @return: bool: true if Projectile needs to be created or false if no Projectile need to be created
	********************/
	bool GetNextCreatedProjectile(ProjectileStates* _pProjectileState);
		
	/***********************
	* GetNextDeletedPowerUp: Returns the next enemy to destroy
	* @author: Jc Fowles
	* @Parameter: EnemyStates* _pEnemyState: EnemyStates pointer to save the info used to delete it
	* @return: bool: true if enemy needs to be delete or false if no enemy need to be deleted
	********************/
	bool GetNextDeletedEnemy(EnemyStates* _pEnemyState);

	/***********************
	* GetNextDeletedPowerUp: Returns the next Power Up to destroy
	* @author: Jc Fowles
	* @Parameter: PowerUpStates* _pPowUpState: PowerUpStates pointer to save the info used to delete it
	* @return: bool: true if power up needs to be delete or false if no power up need to be deleted
	********************/
	bool GetNextDeletedPowerUp(PowerUpStates* _pPowUpState);

	/***********************
	* GetNextDeletedProjectile: Returns the next Projectile to destroy
	* @author: Jc Fowles
	* @Parameter: ProjectileStates * _pProjectileState: ProjectileStates pointer to save the info used to delete it
	* @return: bool: true if Projectile needs to be delete or false if no Projectile need to be deleted
	********************/
	bool GetNextDeletedProjectile(ProjectileStates* _pProjectileState);

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
		
	static CGame* s_pGame;
	CClock* m_pClock;

	std::map<std::string, PlayerStates>* m_plistPlayers;
		
	std::map<UINT, EnemyStates>* m_plistEnemies;
	std::map<UINT, PowerUpStates>* m_pListPowerUps;
	std::map<UINT, ProjectileStates>* m_pListProjectiles;
	
	std::queue<EnemyStates>* m_CreatedEnemies;
	std::queue<EnemyStates>* m_DestroyEnemies;
	
	std::queue<PowerUpStates>* m_CreatedPowerUp;
	std::queue<PowerUpStates>* m_DestroyPowerUp;
	
	std::queue<ProjectileStates>* m_CreatedProjectile;
	std::queue<ProjectileStates>* m_DestroyProjectile;
	

	UINT m_uiStage;
	int m_iNumberPlayers;
	float m_fDt;
	
	UINT  m_uiNextObjID;
	UINT m_uiNextProjectileID;
	
	bool bMoreEnemies;
	int m_iExtraEnemies;

	int m_iLustCount;
	bool m_iWrath;
	int m_iSlothCount;

	int m_iPowTenCount;
	int m_iPowFiftyCount;
	int m_iPowHundredCount;

	float m_fTerrainDepth;
	float m_fTerrainWidth;

	bool bToggle;
	bool m_bSinglePlayer;
	bool m_bStartTimer;
	bool m_bGameOver;
	float m_fSpawnTimer;
	
	float m_fSpeedIncreament;
	float m_fHealthIncreament;
	int m_iDamageIncreament;
};

#endif //__SERVER_GAME_H__