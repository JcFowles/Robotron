#include "Game.h"

CGame* CGame::s_pGame = 0;

CGame::CGame()
{
	m_uiNextObjID = 1;
	m_uiNextProjectileID = 1;
}

CGame::~CGame()
{
	//Delete player list
	delete m_plistPlayers;
	m_plistPlayers = 0;

	delete m_pMapPlayersIDs;
	m_pMapPlayersIDs = 0;

	//Delete enemy list
	delete m_plistEnemies;
	m_plistEnemies = 0;
	
	delete m_pListPowerUps;
	m_pListPowerUps = 0;

	delete m_pListProjectiles;
	m_pListProjectiles = 0;


	delete m_CreatedEnemies; 
	m_CreatedEnemies = 0;
	delete m_DestroyEnemies;
	m_DestroyEnemies = 0;

	delete m_CreatedPowerUp;
	m_CreatedPowerUp = 0; 
	
	delete m_DestroyPowerUp;
	m_DestroyPowerUp = 0;

	delete m_CreatedProjectile;
	m_CreatedProjectile = 0;

	delete m_DestroyProjectile;
	m_DestroyProjectile = 0;

	delete m_pClock;
	m_pClock = 0;

}

CGame& CGame::GetInstance()
{
	//Create the game instance if it doesnt exist 
	if (s_pGame == 0)
	{
		s_pGame = new CGame();
	}
	return (*s_pGame);
}

void CGame::DestroyInstance()
{
	delete s_pGame;
	s_pGame = 0;
}

bool CGame::Initialise()
{
	m_pClock = new CClock();
	m_pClock->Initialise();

	//Initialise player list
	m_plistPlayers = new std::map < std::string, PlayerStates >;
	m_pMapPlayersIDs = new std::map < UINT, std::string >;
	m_plistEnemies = new std::map < UINT, EnemyStates > ;
	m_pListPowerUps = new std::map < UINT, PowerUpStates > ;
	m_pListProjectiles = new std::map < UINT, ProjectileStates > ;


	m_CreatedEnemies = new std::queue<EnemyStates>;
	m_DestroyEnemies = new std::queue<EnemyStates>;

	m_CreatedPowerUp = new std::queue<PowerUpStates>;
	m_DestroyPowerUp = new std::queue<PowerUpStates>;

	m_CreatedProjectile = new std::queue<ProjectileStates>;
	m_DestroyProjectile = new std::queue<ProjectileStates>;


	//TO DO: move and update
	SpawnPowerUp();


	return true;
}

void CGame::Process(ServerDataPacket* _pServerPacket, ClientDataPacket* _pClientPacket)
{
	//Process clock and get the delta tick
	m_pClock->Process();
	float fDT = m_pClock->GetDeltaTick();

	ProcessInput(fDT, _pServerPacket);
	
	
	UpdatePlayers(_pClientPacket);
	UpdateEnemies(_pClientPacket, fDT);
	UpdateProjectile(_pClientPacket, fDT);

	//If no Enemies spawn next wave
	if (m_plistEnemies->size() < 1)
	{
		int NumEnemies = 0;
		if (bMoreEnemies)
		{
			NumEnemies = m_iExtraEnemies;
			bMoreEnemies = false;
		}
		else
		{
			NumEnemies = kiInitialNumEnemies + (m_plistPlayers->size()* kiNumEnemiesPP * m_uiStage);
			if (NumEnemies >= NetworkValues::MAX_ENEMYS )
			{
				
				bMoreEnemies = true;
				m_iExtraEnemies = NumEnemies - NetworkValues::MAX_ENEMYS ;
				NumEnemies = NetworkValues::MAX_ENEMYS ;
			}
			else
			{
				bMoreEnemies = false;
			}
			m_uiStage++;
		}
		
		
		SpawnWave(NumEnemies);
				

	}

}



void CGame::ProcessInput(float _fDt, ServerDataPacket* _pServerPacket)
{
	//Get the users who sent the packets name
	std::string strUserName(_pServerPacket->clientInfo.cUserName);
	std::map<std::string, PlayerStates>::iterator playerIter = m_plistPlayers->find(strUserName);
	
	if (playerIter->second.fFireRate > 0.0f)
	{
		playerIter->second.fFireRate -= _fDt;
	}

	if (playerIter != m_plistPlayers->end())
	{

		if (_pServerPacket->PlayerInputs.bDownPress)
		{
			playerIter->second.f3Acceleration.z = playerIter->second.f3Acceleration.z - 1.0f;
		}

		if (_pServerPacket->PlayerInputs.bUpPress)
		{
			playerIter->second.f3Acceleration.z = playerIter->second.f3Acceleration.z + 1.0f;
		}

		if (_pServerPacket->PlayerInputs.bRightPress)
		{
			playerIter->second.f3Acceleration.x = playerIter->second.f3Acceleration.x + 1.0f;
		}

		if (_pServerPacket->PlayerInputs.bLeftPress)
		{
			playerIter->second.f3Acceleration.x = playerIter->second.f3Acceleration.x - 1.0f;
		}			
		
		if (!(_pServerPacket->PlayerInputs.bLeftPress) &&
			!(_pServerPacket->PlayerInputs.bRightPress) &&
			!(_pServerPacket->PlayerInputs.bDownPress) &&
			!(_pServerPacket->PlayerInputs.bUpPress))
		{
			playerIter->second.f3Velocity = {0.0f, 0.0f, 0.0f };

			//Deceleration to be implemented in future
			/*playerIter->second.f3Velocity -= {0.016667f, 0.016667f, 0.016667f };
			if (playerIter->second.f3Velocity.x < 0)
			{
			playerIter->second.f3Velocity.x = 0.0f;
			}
			if (playerIter->second.f3Velocity.y < 0)
			{
			playerIter->second.f3Velocity.y = 0.0f;
			}
			if (playerIter->second.f3Velocity.z < 0)
			{
			playerIter->second.f3Velocity.z = 0.0f;
			}*/
		}
		
		playerIter->second.f3Acceleration = playerIter->second.f3Acceleration.Normalise();
		playerIter->second.f3Acceleration = playerIter->second.f3Acceleration * _fDt;
				
		POINT CursorPos = _pServerPacket->PlayerInputs.CursorPos;
		CursorPos.x -= 500;
		CursorPos.y -= 500;

		float3 f3Direction(float(CursorPos.x), 0.0f, -1*float(CursorPos.y));
		f3Direction.Normalise();
		playerIter->second.f3Direction = f3Direction; 

		if (_pServerPacket->PlayerInputs.bActivate)
		{
			//Limit the fire rate
			if (playerIter->second.fFireRate <= 0.0f)
			{
				SpawnProjectile(playerIter->second.f3Direction, playerIter->second.f3Positions, playerIter->second.uiPlayerID);
				playerIter->second.fFireRate = kfFireRate;
			}
		}
		

	}
}

void CGame::UpdatePlayers(ClientDataPacket* _pClientPacket)
{

	bool bCollision = false;

	//Process list of players
	std::map<std::string, PlayerStates>::iterator playerIter = m_plistPlayers->begin();
	std::map<std::string, PlayerStates>::iterator playerIterEnd = m_plistPlayers->end();
	int iPlayer = 0;
	while (playerIter != playerIterEnd)
	{
		//playerIter->second.f3Positions = playerIter->second.f3Positions + playerIter->second.f3Velocity;
		
		playerIter->second.f3Velocity += playerIter->second.f3Acceleration;
		playerIter->second.f3Velocity.Limit(playerIter->second.fMaxSpeed);
		playerIter->second.f3Positions += playerIter->second.f3Velocity;
		playerIter->second.f3Acceleration = playerIter->second.f3Acceleration * 0.0f;
			
		//Calculate bounding box
		playerIter->second.BBox.f3CollisionMin = playerIter->second.f3Positions - kfPlayerSize;
		playerIter->second.BBox.f3CollisionMax = playerIter->second.f3Positions + kfPlayerSize;
					
		//Check collisions
		std::map<UINT, PowerUpStates>::iterator PowUpColIter = m_pListPowerUps->begin();
		while (PowUpColIter != m_pListPowerUps->end())
		{
			
			if (CheckCollision(playerIter->second.BBox, PowUpColIter->second.BBox))
			{
				//Get Power up points
				playerIter->second.uiScore += PowUpColIter->second.uiPoints;

				//Get Power Up ability
				//TO DO:
						
				//Delete the power up
				m_DestroyPowerUp->push(PowUpColIter->second);

				break;
				
			}
			

			PowUpColIter++;
		}


		//If the enemy has collided with a player
		std::map<std::string, PlayerStates>::iterator PlayerColIter = m_plistPlayers->begin();
		while (PlayerColIter != m_plistPlayers->end())
		{
			if (PlayerColIter != playerIter)
			{
				if (CheckCollision(playerIter->second.BBox, PlayerColIter->second.BBox))
				{
					bCollision = true;
					break;
				}
			}

			PlayerColIter++;
		}

		if (bCollision == false)
		{
			std::map<UINT, EnemyStates>::iterator EnemyColIter = m_plistEnemies->begin();
			while (EnemyColIter != m_plistEnemies->end())
			{
				
				if (CheckCollision(playerIter->second.BBox, EnemyColIter->second.BBox))
				{
					bCollision = true;
					break;
				}
				
				EnemyColIter++;
			}
		}

		if (bCollision == true)
		{
			//Set the position back to where it was
			playerIter->second.f3Positions -= playerIter->second.f3Velocity;

			//Then Reset its bounding box
			playerIter->second.BBox.f3CollisionMin = playerIter->second.f3Positions - kfPlayerSize;
			playerIter->second.BBox.f3CollisionMax = playerIter->second.f3Positions + kfPlayerSize;

			playerIter->second.f3Velocity = playerIter->second.f3Velocity*0.0f;
			playerIter->second.f3Acceleration = playerIter->second.f3Acceleration * 0.0f;
		}
				
		//Set the player positions in the packet to send
		_pClientPacket->PlayerInfo[iPlayer].f3Positions = playerIter->second.f3Positions;

		iPlayer++;
		playerIter++;
	}
}

void CGame::UpdateEnemies(ClientDataPacket* _pClientPacket, float _fDt)
{
	float fEnemySize = 0.0f;
	bool bCollision = false;

	//Process list of Enemies
	std::map<UINT, EnemyStates>::iterator enemyIter = m_plistEnemies->begin();
	std::map<UINT, EnemyStates>::iterator enemyIterEnd = m_plistEnemies->end();
	int iEnemy = 0;
	while (enemyIter != enemyIterEnd)
	{
		bCollision = false;

		if (enemyIter->second.fHealth <= 0.0f)
		{
			//Remove the enemy that is dead
			m_DestroyEnemies->push(enemyIter->second);
		}
		else
		//If the enemy is still alive process it
		{
			switch (enemyIter->second.Etype)
			{
			case ET_LUST:
			{
				UpdateLust(&enemyIter->second, _fDt);
				fEnemySize = kfLustSize;
			}
			break;
			default:
				break;
			}


			//Calculate Collision Box
			enemyIter->second.BBox.f3CollisionMin = enemyIter->second.f3Positions - fEnemySize;
			enemyIter->second.BBox.f3CollisionMax = enemyIter->second.f3Positions + fEnemySize;

			//Check collisions

			//If the enemy has collided with a player
			std::map<std::string, PlayerStates>::iterator PlayerColIter = m_plistPlayers->begin();
			while (PlayerColIter != m_plistPlayers->end())
			{

				if (CheckCollision(enemyIter->second.BBox, PlayerColIter->second.BBox))
				{
					bCollision = true;
					break;
				}

				PlayerColIter++;
			}

			if (bCollision == false)
			{
				std::map<UINT, EnemyStates>::iterator EnemyColIter = m_plistEnemies->begin();
				while (EnemyColIter != m_plistEnemies->end())
				{
					if (EnemyColIter != enemyIter)
					{
						if (CheckCollision(enemyIter->second.BBox, EnemyColIter->second.BBox))
						{
							bCollision = true;
							break;
						}
					}
					EnemyColIter++;
				}
			}

			if (bCollision == true)
			{
				//Set the position back to where it was
				enemyIter->second.f3Positions -= enemyIter->second.f3Velocity;

				//Then Reset its bounding box
				enemyIter->second.BBox.f3CollisionMin = enemyIter->second.f3Positions - fEnemySize;
				enemyIter->second.BBox.f3CollisionMax = enemyIter->second.f3Positions + fEnemySize;

				enemyIter->second.f3Velocity = enemyIter->second.f3Velocity*0.0f;
				enemyIter->second.f3Acceleration = enemyIter->second.f3Acceleration * 0.0f;
			}

		}
		enemyIter++;
	}

}

void CGame::UpdateLust(EnemyStates* _pEnemy, float _fDt)
{
	std::map<std::string, PlayerStates>::iterator PlayerIter = m_plistPlayers->begin();
	std::map<std::string, PlayerStates>::iterator ClosestPlayer = PlayerIter;
	float fDistance = abs((PlayerIter->second.f3Positions - _pEnemy->f3Positions).Magnitude());

	while (PlayerIter != m_plistPlayers->end())
	{
		float fNewDist = abs((PlayerIter->second.f3Positions - _pEnemy->f3Positions).Magnitude());

		if (fNewDist < fDistance)
		{
			//Update the Distance
			fDistance = fNewDist;
			//The player Iter is closer than the current player, so save the closest player to this one
			ClosestPlayer = PlayerIter;
		}

		PlayerIter++;
	}

	//Set the target to the the closest player
	_pEnemy->f3Target = ClosestPlayer->second.f3Positions;

	//Steerings
	seek(_pEnemy, _fDt);
		
	
}

void CGame::UpdateProjectile(ClientDataPacket* _pClientPacket, float fDT)
{
	std::map<UINT, ProjectileStates>::iterator BulletIter = m_pListProjectiles->begin();
	int iBullet = 0;
	while (BulletIter != m_pListProjectiles->end())
	{
		//Increase the bullet speed by delta tick
		
		//Calculate the new position
		BulletIter->second.f3Velocity += BulletIter->second.f3Direction.Normalise() * fDT * (BulletIter->second.fMaxSpeed);;
		BulletIter->second.f3Velocity.Limit(BulletIter->second.fMaxSpeed);
		BulletIter->second.f3Positions += BulletIter->second.f3Velocity;
		
		//Calculate bounding box
		BulletIter->second.BBox.f3CollisionMin = BulletIter->second.f3Positions - kfPlayerSize;
		BulletIter->second.BBox.f3CollisionMax = BulletIter->second.f3Positions + kfPlayerSize;


		//Calculate Collision
		std::map<UINT, EnemyStates>::iterator EnemyColIter = m_plistEnemies->begin();
		while (EnemyColIter != m_plistEnemies->end())
		{
			if (CheckCollision(BulletIter->second.BBox, EnemyColIter->second.BBox))
			{
				//Decrease enemy Health
				EnemyColIter->second.fHealth -= BulletIter->second.fDamage;

				//Add points to the player
				std::map<UINT, std::string>::iterator iterBulletOwnerName = m_pMapPlayersIDs->find(BulletIter->second.uiOwnerID);
				std::map<std::string, PlayerStates>::iterator iterBulletOwner = m_plistPlayers->find(iterBulletOwnerName->second);
				iterBulletOwner->second.uiScore += EnemyColIter->second.uiPoints;

				//Delete the bullet
				m_DestroyProjectile->push(BulletIter->second);
				
				break;
			}

			EnemyColIter++;
		}

		

		//Set the player positions in the packet to send
		_pClientPacket->ProjectilesInfo[iBullet].f3Positions = BulletIter->second.f3Positions;
		
		iBullet++;
		BulletIter++;
	}
}

void CGame::AddPlayer(std::string _strUser)
{
	//Create the player states
	PlayerStates tempPlayerState;
	StringToStruct(_strUser.c_str(), NetworkValues::MAX_NAME_LENGTH, tempPlayerState.cPlayerName);
	int iNumCurrent = m_plistPlayers->size();
	tempPlayerState.f3Positions = { float(iNumCurrent * 5), 1.0f, 0.0f };
	tempPlayerState.f3Direction = { 0.0f, 0.0f, 1.0f };
	tempPlayerState.f3Velocity = { 0.0f, 0.0f, 0.0f };
	tempPlayerState.f3Acceleration = { 0.0f, 0.0f, 0.0f };
	tempPlayerState.uiScore = 0;
	tempPlayerState.fFireRate = kfFireRate;

	//Collision Box
	tempPlayerState.BBox.f3CollisionMin = tempPlayerState.f3Positions - kfPlayerSize;
	tempPlayerState.BBox.f3CollisionMax = tempPlayerState.f3Positions + kfPlayerSize;

	tempPlayerState.uiHealth = 100;

	tempPlayerState.uiPlayerID = m_uiNextObjID++;
	tempPlayerState.fMaxSpeed = 10.0f;
	tempPlayerState.fMaxForce = 1.0f;
	//Add the player and its states to the list
	m_plistPlayers->insert(std::pair<std::string, PlayerStates>(_strUser, tempPlayerState));
	m_pMapPlayersIDs->insert(std::pair<UINT, std::string>(tempPlayerState.uiPlayerID, _strUser));
		
}

void CGame::RemovePlayer(std::string _strLeftPlayer)
{
		m_plistPlayers->erase(_strLeftPlayer);
}

void CGame::SetPlayerStates(ClientDataPacket* _pDataToSend)
{
	//Reset the list of active clients
	for (int iUser = 0; iUser < NetworkValues::MAX_USERS; iUser++)
	{
		//Set the user name to nothing
		std::string strText = "";
		strcpy_s(_pDataToSend->PlayerInfo[iUser].cPlayerName, strText.c_str());
	}

	//Run through the map of active clients
	std::map< std::string, PlayerStates>::iterator iterClient = m_plistPlayers->begin();
	std::map< std::string, PlayerStates>::iterator iterClientEnd = m_plistPlayers->end();
	int iUser = 0;
	while (iterClient != iterClientEnd)
	{
		std::string strText = iterClient->first;
		if (strText.length() < NetworkValues::MAX_NAME_LENGTH)
		{
			//Set the user name in the active client list in the server info to the current user in the map of active clients
			strcpy_s(_pDataToSend->PlayerInfo[iUser].cPlayerName, strText.c_str());
		}

		_pDataToSend->PlayerInfo[iUser] = iterClient->second;

		//Get next clients user name
		iterClient++;
		//Increment the iUser
		iUser++;
	}

	_pDataToSend->iNumPlayers = m_plistPlayers->size();
}

void CGame::SetEnemyStates(ClientDataPacket* _pDataToSend)
{
	//Run through the map of active clients
	std::map< UINT, EnemyStates>::iterator iterEnemy = m_plistEnemies->begin();
	std::map< UINT, EnemyStates>::iterator iterEnemyEnd = m_plistEnemies->end();
	int iEnemy = 0;
	while (iterEnemy != iterEnemyEnd)
	{
		_pDataToSend->EnemyInfo[iEnemy] = iterEnemy->second;

		//Get next clients user name
		iterEnemy++;
		//Increment the iUser
		iEnemy++;
	}

	_pDataToSend->iNumEnemies = m_plistEnemies->size();

}

void CGame::SetPowUpStates(ClientDataPacket* _pDataToSend)
{
	//Run through the map of active clients
	std::map< UINT, PowerUpStates>::iterator iterPowUp = m_pListPowerUps->begin();
	int iPow = 0;
	while (iterPowUp != m_pListPowerUps->end())
	{
		_pDataToSend->PowUpInfo[iPow] = iterPowUp->second;

		//Get next power up
		iterPowUp++;
		//Increment the power up
		iPow++;
	}

	_pDataToSend->iNumPowerUps = m_pListPowerUps->size();
}

void CGame::SetProjectileStates(ClientDataPacket* _pDataToSend)
{
	//Run through the map of active clients
	std::map< UINT, ProjectileStates>::iterator iterBullet = m_pListProjectiles->begin();
	int iBullet = 0;
	while (iterBullet != m_pListProjectiles->end())
	{
		_pDataToSend->ProjectilesInfo[iBullet] = iterBullet->second;

		//Get next bullet
		iterBullet++;
		//Increment the bullet
		iBullet++;
	}

	_pDataToSend->iNumProjectiles = m_pListProjectiles->size();
}

void CGame::SpawnWave(int _NumToSpawn)
{
	
	for (int i = 0; i < _NumToSpawn; i++)
	{
		float RandPosX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		RandPosX = RandPosX * 256; //Surface Size //TO DO
		RandPosX = RandPosX - 256 / 2; //set back to middle
		float RandPosZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		RandPosZ = RandPosZ * 256; //Surface Size //TO DO
		RandPosZ = RandPosZ - 256 / 2; //set back to middle
		float3 f3Pos = { RandPosX, 1.0f, RandPosZ };
		CreateLust(f3Pos);
	}

}



void CGame::CreateLust(float3 _f3Pos)
{
	//create Enemy State
	EnemyStates tempEnemyState;
	tempEnemyState.Etype = ET_LUST;

	tempEnemyState.f3Direction = { 0.0f, 0.0f, 1.0f };
	tempEnemyState.f3Positions = _f3Pos;
	tempEnemyState.f3Velocity = { 0.0f, 0.0f, 0.0f };
	tempEnemyState.f3Acceleration = { 0.0f, 0.0f, 0.0f };
	tempEnemyState.uiPoints = kuiLustPoints;

	//Collision Box
	tempEnemyState.BBox.f3CollisionMin = tempEnemyState.f3Positions - kfLustSize;
	tempEnemyState.BBox.f3CollisionMax = tempEnemyState.f3Positions + kfLustSize;

	tempEnemyState.uiEnemyID = m_uiNextObjID++;
	tempEnemyState.fMaxSpeed = 5.00f;
	tempEnemyState.fMaxForce = 30.0f;
	tempEnemyState.fMaxAccel = 0.8f;

	tempEnemyState.fHealth = kfLustHealth;

	std::pair<std::map<UINT, EnemyStates>::iterator, bool> mapIter;
	//Add to the list of enemies
	mapIter = m_plistEnemies->insert(std::pair<UINT, EnemyStates>(tempEnemyState.uiEnemyID, tempEnemyState));

	if (mapIter.second != false)
	{
		//Add to the list of created
		m_CreatedEnemies->push(tempEnemyState);
	}
}

void CGame::SpawnPowerUp()
{
	//create Enemy State
	PowerUpStates tempPowUPState;
	tempPowUPState.Etype = PU_SHIELD;

	tempPowUPState.f3Direction = { 0.0f, 0.0f, 1.0f };
	tempPowUPState.f3Positions = { float(m_uiNextObjID * 5), 1.0f, -50.0f };
	tempPowUPState.f3Velocity = { 0.0f, 0.0f, 0.0f };
	tempPowUPState.f3Acceleration = { 0.0f, 0.0f, 0.0f };
	tempPowUPState.uiPoints = kuiShieldPoints;

	//Collision Box
	tempPowUPState.BBox.f3CollisionMin = tempPowUPState.f3Positions - kfShieldSize;
	tempPowUPState.BBox.f3CollisionMax = tempPowUPState.f3Positions + kfShieldSize;

	tempPowUPState.uiPowUpID = m_uiNextObjID++;
	tempPowUPState.fMaxSpeed = 0.10f;
	tempPowUPState.fMaxForce = 0.1f;
	
	std::pair<std::map<UINT, PowerUpStates>::iterator, bool> mapIter;
	//Add to the list of enemies
	mapIter = m_pListPowerUps->insert(std::pair<UINT, PowerUpStates>(tempPowUPState.uiPowUpID, tempPowUPState));
	
	if (mapIter.second != false)
	{
		//Add to the list of created
		m_CreatedPowerUp->push(tempPowUPState);
	}
	
}

bool CGame::SpawnProjectile(float3 _Direction, float3 _Position, UINT _uiOnwerID)
{
	//Limit to the creation of the bullets 
	if (m_pListProjectiles->size() <= NetworkValues::MAX_PROJECTILE)
	{
		ProjectileStates tempProjectile;

		tempProjectile.fMaxSpeed = 15.0f;

		tempProjectile.f3Direction = _Direction;
		tempProjectile.f3Positions = _Position;
		tempProjectile.f3Velocity = { 0.0f, 0.0f, 0.0f };
		tempProjectile.f3Acceleration = _Direction.Normalise();
		tempProjectile.uiOwnerID = _uiOnwerID;

		//Collision Box
		tempProjectile.BBox.f3CollisionMin = tempProjectile.f3Positions - kfBulletSize;
		tempProjectile.BBox.f3CollisionMax = tempProjectile.f3Positions + kfBulletSize;

		tempProjectile.fDamage = kfBulletDamage;

		tempProjectile.uiProjectileID = m_uiNextProjectileID++;
		
		std::pair<std::map<UINT, ProjectileStates>::iterator, bool> mapIter;
		//Add to the list of enemies
		mapIter = m_pListProjectiles->insert(std::pair<UINT, ProjectileStates>(tempProjectile.uiProjectileID, tempProjectile));

		if (mapIter.second == false)
		{
			return false;
		}
		//Add to the list of created
		m_CreatedProjectile->push(tempProjectile);

		return true;
	}
	else
	{
		//Too many bullets need to delete some
		std::map < UINT, ProjectileStates > ::iterator projToDelete = m_pListProjectiles->begin();
		m_DestroyProjectile->push(projToDelete->second);
		m_pListProjectiles->erase(projToDelete->first);
		return false;
	}
	
}

bool CGame::CheckCollision(BoundingBox _BBoxA, BoundingBox _BBoxB)
{

	if (((_BBoxA.f3CollisionMax.x >= _BBoxB.f3CollisionMin.x) && (_BBoxA.f3CollisionMin.x <= _BBoxB.f3CollisionMax.x)) &&
		((_BBoxA.f3CollisionMax.y >= _BBoxB.f3CollisionMin.y) && (_BBoxA.f3CollisionMin.y <= _BBoxB.f3CollisionMax.y)) &&
		((_BBoxA.f3CollisionMax.z >= _BBoxB.f3CollisionMin.z) && (_BBoxA.f3CollisionMin.z <= _BBoxB.f3CollisionMax.z)))
	{
		//The Bounding boxes are colliding
		return true;
	}
	else
	{
		//The Bounding boxes are not colliding
		return false;
	}
}


//Creating and Deleting Objects
bool CGame::GetNextCreatedEnemy(EnemyStates* _pEnemyState)
{
	if (m_CreatedEnemies->size() > 0)
	{
		//Get the next enemy state to create the enemy
		*_pEnemyState = m_CreatedEnemies->front();
		m_CreatedEnemies->pop();

		return true;
	}
	else
	{
		//There are no enemies to create
		return false;
	}
}

bool CGame::GetNextCreatedPowerUp(PowerUpStates* _pPowUpState)
{
	if (m_CreatedPowerUp->size() > 0)
	{
		//Get the next power up state to create the power up
		*_pPowUpState = m_CreatedPowerUp->front();
		m_CreatedPowerUp->pop();

		return true;
	}
	else
	{
		//There are no power ups to create
		return false;
	}
}

bool CGame::GetNextCreatedProjectile(ProjectileStates* _pProjectileState)
{
	if (m_CreatedProjectile->size() > 0)
	{
		//Get the next power up state to create the power up
		*_pProjectileState = m_CreatedProjectile->front();
		m_CreatedProjectile->pop();

		return true;
	}
	else
	{
		//There are no power ups to create
		return false;
	}
}

bool CGame::GetNextDeletedEnemy(EnemyStates* _pEnemyState)
{
	if (m_DestroyEnemies->size() > 0)
	{
		//Get the next enemy state to create the enemy
		*_pEnemyState = m_DestroyEnemies->front();
		m_plistEnemies->erase(_pEnemyState->uiEnemyID);
		m_DestroyEnemies->pop();

		return true;
	}
	else
	{
		//There are no enemies to create
		return false;
	}
}

bool CGame::GetNextDeletedPowerUp(PowerUpStates* _pPowUpState)
{
	if (m_DestroyPowerUp->size() > 0)
	{
		//Get the next power up state to create the power up
		*_pPowUpState = m_DestroyPowerUp->front();
		m_pListPowerUps->erase(_pPowUpState->uiPowUpID);
		m_DestroyPowerUp->pop();

		return true;
	}
	else
	{
		//There are no power ups to create
		return false;
	}
}

bool CGame::GetNextDeletedProjectile(ProjectileStates* _pProjectileState)
{
	if (m_DestroyProjectile->size() > 0)
	{
		//Get the next power up state to create the power up
		*_pProjectileState = m_DestroyProjectile->front();
		m_pListProjectiles->erase(_pProjectileState->uiProjectileID);
		m_DestroyProjectile->pop();

		return true;
	}
	else
	{
		//There are no power ups to create
		return false;
	}
}