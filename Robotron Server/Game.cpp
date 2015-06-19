#include "Game.h"

CGame* CGame::s_pGame = 0;

CGame::CGame()
{
}


CGame::~CGame()
{
	//Delete player list
	delete m_plistPlayers;
	m_plistPlayers = 0;

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

bool CGame::Initialise(std::vector<std::string> _ListPlayers, ClientDataPacket* _pClientPacket)
{
	m_pClock = new CClock();
	m_pClock->Initialise();

	m_plistPlayers = new std::map < std::string, PlayerStates >;

	//Get the number of players
	m_iNumberPlayers = _ListPlayers.size();

	//Initialise the list of players
	for (int iPlayer = 0; iPlayer < m_iNumberPlayers; iPlayer++)
	{
			
		//Create the player states
		PlayerStates tempState;
		StringToStruct(_ListPlayers[iPlayer].c_str(), NetworkValues::MAX_NAME_LENGTH, tempState.cPLayerName);
		tempState.f3Positions = { float(iPlayer * 5), 20.0f, 0.0f };
		tempState.f3Velocity = { 0.0f, 0.0f, 0.0f };
		
		//Add the player and its states to the list
		m_plistPlayers->insert(std::pair<std::string, PlayerStates>(_ListPlayers[iPlayer], tempState));

		//Set the player positions in the packet to send
		_pClientPacket->PlayerInfo[iPlayer].f3Positions = tempState.f3Positions;

	}

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

	//Create enemies if needed

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
	}
}
