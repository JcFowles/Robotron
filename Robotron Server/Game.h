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

//TO DO: comment Headers

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
	static CGame& GetInstance();
	static void DestroyInstance();

	~CGame(void);

	bool Initialise();

	void Process(ClientDataPacket* _pClientPacket);
	void ProcessInput( ServerDataPacket* _pServerPacket);

	//TO DO:COMMENT
	
	void AddPlayer(std::string _strUser);
	void RemovePlayer(std::string _strLeftPlayer);
	void UpdatePlayers();
	void UpdateEnemies();
	void UpdateLust(EnemyStates* _Enemy);
	void UpdateSloth(EnemyStates* _pEnemy);
	void UpdateWrath(EnemyStates* _pEnemy);
	void UpdateProjectile();
	
	void UpdatePowerUp();
	void UpdateShield(PowerUpStates* _pPowerUp);

	void SetPlayerStates(ClientDataPacket* _pDataToSend);
	void SetEnemyStates(ClientDataPacket* _pDataToSend);
	void SetPowUpStates(ClientDataPacket* _pDataToSend);
	void SetProjectileStates(ClientDataPacket* _pDataToSend);
		
	/***********************
	* CheckCollision: Check if two bounding boxes are colliding
	* @author: Jc Fowles
	* @Parameter: BoundingBox _BBoxA: first bounding box
	* @Parameter: BoundingBox _BBoxB: Second bounding box
	* @return: bool: True if colliding
	********************/
	bool CheckCollision(BoundingBox _BBoxA, BoundingBox _BBoxB);
	
	void SpawnWave();
	void CreateLust(float3 _f3Pos);
	void CreateSloth(float3 _f3Pos);
	void CreateWrath(float3 _f3Pos);
	void SpawnPowerUp();
	bool SpawnProjectile(float3 _Direction, float3 _Position, UINT _uiOwnerID);

	
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
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);
	

	//member variables
private:
		
	static CGame* s_pGame;
	CClock* m_pClock;

	std::map<std::string, PlayerStates>* m_plistPlayers;
	std::map<UINT, std::string>* m_pMapPlayersIDs;

	
	std::map<UINT, EnemyStates>* m_plistEnemies;
	std::map<UINT, PowerUpStates>* m_pListPowerUps;
	std::map<UINT, ProjectileStates>* m_pListProjectiles;
	
	std::queue<EnemyStates>* m_CreatedEnemies;
	std::queue<EnemyStates>* m_DestroyEnemies;
	
	std::queue<PowerUpStates>* m_CreatedPowerUp;
	std::queue<PowerUpStates>* m_DestroyPowerUp;
	
	std::queue<ProjectileStates>* m_CreatedProjectile;
	std::queue<ProjectileStates>* m_DestroyProjectile;


	int m_iNumberPlayers;

	UINT m_uiStage;

	float m_fDt;
	

	UINT  m_uiNextObjID;
	UINT m_uiNextProjectileID;


	bool bMoreEnemies;
	int m_iExtraEnemies;

	int m_iLustCount;
	bool m_iWrath;
	int m_iSlothCount;


	float m_fTerrainDepth;
	float m_fTerrainWidth;

	bool bToggle;
};

#endif //__SERVER_GAME_H__