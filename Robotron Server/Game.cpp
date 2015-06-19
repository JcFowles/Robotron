#include "Game.h"

CGame* CGame::s_pGame = 0;

CGame::CGame()
{
	m_uiNextObjID = 0;
}


CGame::~CGame()
{
	//Delete player list
	delete m_plistPlayers;
	m_plistPlayers = 0;

	//Delete enemy list
	delete m_plistEnemies;
	m_plistEnemies = 0;
	
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
	m_plistEnemies = new std::map < UINT, EnemyStates > ;

	//TO DO: move and update
	SpawnWave();

	return true;
}

void CGame::Process(ServerDataPacket* _pServerPacket, ClientDataPacket* _pClientPacket)
{
	//Process clock and get the delta tick
	m_pClock->Process();
	float fDT = m_pClock->GetDeltaTick();

	ProcessInput(fDT, _pServerPacket);
	
	//Process list of players
	std::map<std::string, PlayerStates>::iterator playerIter = m_plistPlayers->begin();
	std::map<std::string, PlayerStates>::iterator playerIterEnd = m_plistPlayers->end();
	int iPlayer = 0;
	while (playerIter != playerIterEnd)
	{
		playerIter->second.f3Positions = playerIter->second.f3Positions + playerIter->second.f3Velocity;

	
		//Set the player positions in the packet to send
		_pClientPacket->PlayerInfo[iPlayer].f3Positions = playerIter->second.f3Positions;

		playerIter->second.f3Velocity = { 0.0f, 0.0f, 0.0f };

		iPlayer++;
		playerIter++;
	}




}

void CGame::ProcessInput(float _fDt, ServerDataPacket* _pServerPacket)
{
	//Get the users who sent the packets name
	std::string strUserName(_pServerPacket->clientInfo.cUserName);
	std::map<std::string, PlayerStates>::iterator playerIter = m_plistPlayers->find(strUserName);
	
	if (playerIter != m_plistPlayers->end())
	{

		if (_pServerPacket->PlayerInputs.bDownPress)
		{
			//playerIter->second.f3Velocity.z = -1*(playerIter->second.f3Velocity.z * _fDt);
			playerIter->second.f3Velocity.z = playerIter->second.f3Velocity.z - 0.05f;
		}

		if (_pServerPacket->PlayerInputs.bUpPress)
		{
			//playerIter->second.f3Velocity.z = (playerIter->second.f3Velocity.z * _fDt);
			playerIter->second.f3Velocity.z = playerIter->second.f3Velocity.z + 0.05f;
		}

		if (_pServerPacket->PlayerInputs.bRightPress)
		{
			//playerIter->second.f3Velocity.x =  (playerIter->second.f3Velocity.x * _fDt);
			playerIter->second.f3Velocity.x = playerIter->second.f3Velocity.x + 0.05f;
		}

		if (_pServerPacket->PlayerInputs.bLeftPress)
		{
			//playerIter->second.f3Velocity.x = -1*(playerIter->second.f3Velocity.x * _fDt);
			playerIter->second.f3Velocity.x = playerIter->second.f3Velocity.x - 0.05f;
		}			
		
		POINT CursorPos = _pServerPacket->PlayerInputs.CursorPos;
		CursorPos.x -= 500;
		CursorPos.y -= 500;

		float3 f3Direction(float(CursorPos.x), 0.0f, -1*float(CursorPos.y));
		f3Direction.Normalise();
		playerIter->second.f3Direction = f3Direction; 

		if (_pServerPacket->PlayerInputs.bActivate)
		{
			//Spawn a bullet
		}

	}
}

void CGame::UpdatePlayers(std::vector<std::string> _ListPlayers, ClientDataPacket* _pClientPacket)
{
	
}

void CGame::AddPlayer(std::string _strUser)
{
	//Create the player states
	PlayerStates tempPlayerState;
	StringToStruct(_strUser.c_str(), NetworkValues::MAX_NAME_LENGTH, tempPlayerState.cPlayerName);
	int iNumCurrent = m_plistPlayers->size();
	tempPlayerState.f3Positions = { float(iNumCurrent * 5), 20.0f, 0.0f };
	tempPlayerState.f3Direction = { 0.0f, 0.0f, 1.0f };
	tempPlayerState.f3Velocity = { 0.0f, 0.0f, 0.0f };
	tempPlayerState.uiPlayerID = m_uiNextObjID++;

	//Add the player and its states to the list
	m_plistPlayers->insert(std::pair<std::string, PlayerStates>(_strUser, tempPlayerState));
		
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

void CGame::SpawnWave()
{
	//create Enemy State
	EnemyStates tempEnemyState;
	tempEnemyState.Etype = ET_LUST;
	tempEnemyState.f3Direction = { 0.0f, 0.0f, 1.0f };
	tempEnemyState.f3Positions = { float(m_uiNextObjID * 5), 20.0f, 5.0f };
	tempEnemyState.uiEnemyID = m_uiNextObjID++;

	//Add to the list of enemies
	m_plistEnemies->insert(std::pair<UINT, EnemyStates>(tempEnemyState.uiEnemyID, tempEnemyState));

}

