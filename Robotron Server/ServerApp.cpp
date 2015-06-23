//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CServerApp.cpp
// Description : The core Server app and its functionality for the server
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//TO DO COmment headers

//This include
#include "ServerApp.h"

//Globals statics
CMySemaphore CServerApp::s_Mutex(1);
CServerApp* CServerApp::s_pServerApp = 0;

CServerApp::CServerApp(void)
{
	//Frame limiter
	m_iFrameTimeStart = 0;
	m_iFrameTimeEnd = 0;
	m_iFrameTimeDifference = 0;

	m_iSecondCounter = 0;
	m_iFrameCounter = 0;
	m_iFPS = 0;
}

CServerApp::~CServerApp(void)
{
	//Server
	delete m_pClock;
	m_pClock = 0;
	
	//Networking
	
	//Add server info to client packet
	SetServerInfo();
	//Send to all so so has left
	m_pClientPacket->packetType = PT_QUIT;

	//Send quit message to all clients
	SendToAll(m_pClientPacket);
	
	//Remove all clients from map
	m_pMapClientInfo->clear();

	//Make sure the the client is set to inactive
	m_pServer->SetActive(false);
	//Join the threads
	m_RecieveThread.join();

	delete m_pServer;
	m_pServer = 0;
	delete m_pServerPacket;
	m_pServerPacket = 0;
	delete m_pClientPacket;
	m_pClientPacket = 0;
	delete m_pServerDataQueue;
	m_pServerDataQueue = 0;
	delete m_pMapClientInfo;
	m_pMapClientInfo = 0;

	m_pGame->DestroyInstance();
	m_pGame = 0;
}

CServerApp& CServerApp::GetInstance()
{
	//Create the game instance if it doesnt exist 
	if (s_pServerApp == 0)
	{
		s_pServerApp = new CServerApp();
	}
	return (*s_pServerApp);
}

void CServerApp::DestroyInstance()
{
	delete s_pServerApp;
	s_pServerApp = 0;
}

bool CServerApp::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight, LPWSTR* _pCmdArgs)
{
	//Initialise Server app Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Store the server arguments
	m_strServerName = WideStringToString(_pCmdArgs[1]);
	m_strHostClient = WideStringToString(_pCmdArgs[2]);
	m_bGameStart = false;
	m_bSinglePlayer = false;
 
	//Initialise member variables
	m_pServer = new CServer();
	VALIDATE(m_pServer->Initialise());
	m_bHaveHost = false;
	m_pServerDataQueue = new std::queue < ServerDataPacket >;
	m_pServerPacket = new ServerDataPacket;
	m_pClientPacket = new ClientDataPacket;
	

	//Create and initialize the game
	//Create and initialize the server side game
	m_pGame = &(CGame::GetInstance());
	m_pGame->Initialise();

	//Initialise Map of player states
	m_pMapClientInfo = new std::map < std::string, ClientInfo > ;
		
	//Set Server info on creation
	SetServerInfo();

	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CServerApp::ReceiveDataThread, (this));

	m_pClientPacket->packetType = PT_CREATE;
	m_pClientPacket->serverInfo.serverSocAddr = m_pServer->GetServerSocketAddress();
	//Send back the the server name and Host client names to be checked against
	std::string strTextToSend = m_strServerName + ":" + m_strHostClient;
	StringToStruct(strTextToSend.c_str(), NetworkValues::MAX_CHAR_LENGTH, m_pClientPacket->cText);
		

	//Broadcast to all potential clients
	m_pServer->Broadcast(m_pClientPacket);

		
	return true;
}


//Frame calls
void CServerApp::Process()
{	
	ProcessReceiveData();

	if (m_bGameStart)
	{
		CheckDeletion();
		CheckCreation();
	}
	
	//Add server info to client packet
	SetServerInfo();
	//Send to all so so has left
	m_pClientPacket->packetType = PT_UPDATE;

	m_pGame->Process(m_pClientPacket);

	SendToAll(m_pClientPacket);
}

void CServerApp::Draw()
{

}

bool CServerApp::ProcessSingleFrame()
{
	//Start time
	m_iFrameTimeStart = (int)timeGetTime();
	//Use some time to ensure Delta tick not 0.0f
	Sleep(1);

	//Is the Server still active? if not return false
	if (m_pServer->getActive() == false)
	{
		return false;
	}
	
	Process();

	//Limit frames to roughly 60 frames per second
	LimitFrame();

	return true;
}

void CServerApp::LimitFrame()
{
	//End  the time
	m_iFrameTimeEnd = (int)timeGetTime();

	//Calculate the total time taken to render one frame
	m_iFrameTimeDifference = m_iFrameTimeEnd - m_iFrameTimeStart;

	//Increment the second counter by the frame difference
	m_iSecondCounter += m_iFrameTimeDifference;
	m_iFrameCounter++;

	//Calculate remaining time to render frames at 60fps
	int iTimeToWait;
	if (m_iFrameCounter % 3 == 0)
	{
		//Every third frame wait 1 millisecond less to offset the 2 frames over the 16.667 (1000/60) mark
		iTimeToWait = (16) - (m_iFrameTimeDifference);
	}
	else
	{
		iTimeToWait = (17) - (m_iFrameTimeDifference);
	}

	//Sleep only if the Rendering of the frame took less than 1/60th of a seconds
	if (iTimeToWait > 0)
	{
		Sleep(iTimeToWait);
		m_iSecondCounter += iTimeToWait;
	}

	if (m_iSecondCounter >= 1000)
	{
		//Remove one second from the counter. 
		//Prevents integer wrap around
		m_iSecondCounter -= 1000;
		//FPS for this second is set to the number of frames rendered
		m_iFPS = m_iFrameCounter;
		//Reset the Frame counter
		m_iFrameCounter = 0;
	}
}


//Process receive data
void CServerApp::ReceiveDataThread()
{
	ServerDataPacket* pServerPacket = new ServerDataPacket;
	while (m_pServer->getActive())
	{
		if (m_pServer->ReceiveData(pServerPacket))
		{
			//***CRITICAL SECTION***
			//Stop other threads looking at the queue
			//While one thread is in the process of acquiring an item from it
			s_Mutex.Wait();
			//Push onto the work queue
			m_pServerDataQueue->push(*pServerPacket);
			//Signal that you are done with the queue
			s_Mutex.Signal();
		}

	}

	//Memory clean up
	delete pServerPacket;
	pServerPacket = 0;
}

void CServerApp::ProcessReceiveData()
{
	while (m_pServerDataQueue->empty() == false)
	{
		//***CRITICAL SECTION***
		//Stop other threads looking at the queue
		//While one thread is in the process of acquiring an item from it
		s_Mutex.Wait();
		
		//Check to see if the queue is empty
		if (m_pServerDataQueue->empty())
		{
			//If queue is empty send release signal and break out of the while loop
			s_Mutex.Signal();
			break;
		}

		//Get item from the work queue
		*m_pServerPacket = m_pServerDataQueue->front();
		m_pServerDataQueue->pop();
		//Signal that you are done with the queue
		s_Mutex.Signal();
		
		switch (m_pServerPacket->packetType)
		{
		case PT_DEFAULT:
		{
			ProcessDefault();
		}
			break;
		case PT_CREATE:
		{
			//only process creation if you have no host
			if (m_bHaveHost == false)
			{
				ProcessCreation();
			}
		}
			break;
		case PT_FIND:
		{
			ProcessFind();
		}
			break;
					
		case PT_JOIN_REQUEST:
		{
			if (ProcessJoinRequest())
			{
				//Add server info to client packet
				SetServerInfo();
				//Send to all so so has left
				m_pClientPacket->packetType = PT_CLIENT_JOINED;

				//Add the user that just joined to the text part and send that to all users that this one joined
				StringToStruct(m_pServerPacket->clientInfo.cUserName, NetworkValues::MAX_CHAR_LENGTH, m_pClientPacket->cText);
			
				SendToAll(m_pClientPacket);
			}

		}
			break;

		case PT_ACTIVE:
		{
			ProcessActive();
		}
			break;

		case PT_LEAVE:
		{
			if (m_pServerPacket->clientInfo.cUserName == m_strHostClient)
			{
				ProcessQuit();
			}
			else
			{
				ProcessLeave();
			}
		}
		break;
		case PT_QUIT:
		{
			ProcessQuit();
		}
		case PT_INPUT:
		{
			//Process the inputs
			m_pGame->ProcessInput(m_pServerPacket);		
		}
		break;
		default:
			break;
		}
	}


}

void CServerApp::ProcessCreation()
{

	//Convert the text in received packet to a string to be manipulated
	std::string strCreation(m_pServerPacket->cText);

	if (strCreation == "<KW>HOST")
	{

		//Set whether server will be set to a multiplayer or single player server
		m_bSinglePlayer = m_pServerPacket->bSuccess;

		//Client has sent back that it is this servers host

		//Give the server the host clients information
		//So that the server knows who its hosting client is
		m_pServer->SetHostClientInfo(m_strHostClient, m_pServerPacket->clientInfo.clientSocAddr);

		//Add the client to the map of clients
		AddUser(m_strHostClient, m_pServerPacket->clientInfo);
		
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_CLIENT_JOINED;
		//Add the user name of the joining player, in this case the host name
		StringToStruct(m_strHostClient.c_str(), NetworkValues::MAX_NAME_LENGTH, m_pClientPacket->cText);
		//Send the data back to the client
		m_pServer->SendData(m_pClientPacket, m_pServerPacket->clientInfo.clientSocAddr);

		//This server now has its host
		m_bHaveHost = true;

	}
	
}

void CServerApp::ProcessFind()
{
	if (m_bGameStart == false)
	{
		//Add server info to client packet
		SetServerInfo();
		//Create find message to send
		m_pClientPacket->packetType = PT_FIND;
		//Send message
		m_pServer->SendData(m_pClientPacket, m_pServerPacket->clientInfo.clientSocAddr);
	}
}

bool CServerApp::ProcessJoinRequest()
{
	//Initialize and create join request message
	m_pClientPacket->packetType = PT_JOIN_REQUEST;
	m_pClientPacket->bSuccess = false;
	
	//Try add user and its socket address to the map of client
	if (AddUser(m_pServerPacket->clientInfo.cUserName, m_pServerPacket->clientInfo))
	{
		//Send success		
		m_pClientPacket->bSuccess = true;
	}
	else
	{
		//User name already in use
		std::string strError = "<KW>INVALID_USERNAME";
		StringToStruct(strError.c_str(), NetworkValues::MAX_CHAR_LENGTH, m_pClientPacket->cText);

	}
			
	//Add server info to client packet
	SetServerInfo();
	
	//Send message
	m_pServer->SendData(m_pClientPacket, m_pServerPacket->clientInfo.clientSocAddr);

	return (m_pClientPacket->bSuccess);
}

void CServerApp::ProcessActive()
{
	
	//Set the activeness of the client who sent the message based on the success value
	std::string strUserName(m_pServerPacket->clientInfo.cUserName);
	std::map<std::string, ClientInfo>::iterator itterClient = m_pMapClientInfo->find(strUserName);
	if (itterClient != m_pMapClientInfo->end())
	{
		//Set the active-ness based on the bSuccess value
		itterClient->second.bActive = m_pServerPacket->bSuccess;
	}

	//Check if game should start
	if (AllActive())
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_GAME_START;
				
		std::vector<std::string> listPlayers;

		//Single player
		if (m_bSinglePlayer == true)
		{

			std::map< std::string, ClientInfo>::iterator iterClient = m_pMapClientInfo->begin();
			//add the player to list of players, in this case will only be one player
			listPlayers.push_back(iterClient->first);
						
			//Send message
			SendToAll(m_pClientPacket);
			//Set Game to start
			m_bGameStart = true;
		}
		else //Multiplayer
		{
			//If more than 1 players are in the lobby
			if (m_pMapClientInfo->size() > 1)
			{
				//Loop through the map, adding the players to list of players	
				std::map< std::string, ClientInfo>::iterator iterClient = m_pMapClientInfo->begin();
				std::map< std::string, ClientInfo>::iterator iterClientEnd = m_pMapClientInfo->end();
				while (iterClient != iterClientEnd)
				{
					//add the player to list of players	
					listPlayers.push_back(iterClient->first);
					//Next player				
					iterClient++;
				}
								
				//Send message
				SendToAll(m_pClientPacket);
				//Set Game to start
				m_bGameStart = true;

			}
		}
	}



}

void CServerApp::ProcessDefault()
{
	//Create default message
	m_pClientPacket->packetType = PT_DEFAULT;
	//Add server info to client packet
	SetServerInfo();
	//Send message
	m_pServer->SendData(m_pClientPacket, m_pServerPacket->clientInfo.clientSocAddr);
}

void CServerApp::ProcessLeave()
{
	std::string strUserName(m_pServerPacket->clientInfo.cUserName);

	if (strUserName != m_strHostClient)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_LEAVE;
		StringToStruct(strUserName.c_str(), NetworkValues::MAX_NAME_LENGTH, m_pClientPacket->cText);
		
		//Remove the client from the map
		m_pMapClientInfo->erase(strUserName);

		//If the game exist remove the player from there too 
		if (m_pGame != 0)
		{
			m_pGame->RemovePlayer(strUserName);
		}
		
		//Send message
		SendToAll(m_pClientPacket);

	}
	else
	{
		ProcessQuit();
	}
}

void CServerApp::ProcessQuit()
{
	std::string strUserName(m_pServerPacket->clientInfo.cUserName);
	if ((strUserName == m_pServer->GetHostClientInfo()->first) &&
		(m_pServerPacket->clientInfo.clientSocAddr.sin_addr.S_un.S_addr == m_pServer->GetHostClientInfo()->second.sin_addr.S_un.S_addr)) //Compare two socket address are equal
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_QUIT;

		//Send message
		SendToAll(m_pClientPacket);
		
		//Remove all clients from map
		m_pMapClientInfo->clear();

		//Sleep for a small time to make sure every one got the message
		Sleep(50);
		m_pServer->SetActive(false);
	}
}


//Packet manipulations
void CServerApp::SetServerInfo()
{
	/*<SERVER_INFO>*/
	//Server Name
	StringToStruct(m_strServerName.c_str(), NetworkValues::MAX_NAME_LENGTH, m_pClientPacket->serverInfo.cServerName);
	//Host Name
	StringToStruct(m_strHostClient.c_str(), NetworkValues::MAX_NAME_LENGTH, m_pClientPacket->serverInfo.cHostName);
	//Socket address
	m_pClientPacket->serverInfo.serverSocAddr = m_pServer->GetServerSocketAddress();
	//Number of clients
	m_pClientPacket->serverInfo.iNumClients = m_pMapClientInfo->size();
	//Client List
	SetActiveClientList(m_pClientPacket);
	//Set game states
	SetGameState(m_pClientPacket);
}

void CServerApp::SetActiveClientList(ClientDataPacket* _pDataToSend)
{
	//Reset the list of active clients
	for (int iUser = 0; iUser < NetworkValues::MAX_USERS; iUser++)
	{
		//Set the user name to nothing
		std::string strText = "";
		//Set the user name in the active client list in the server info to an empty sting
		strcpy_s(_pDataToSend->serverInfo.activeClientList[iUser].cUserName, strText.c_str());

		//Set the active-ness of the user in the active client list in the server info to false
		_pDataToSend->serverInfo.activeClientList[iUser].bActive = false;
	}

	//Run through the map of active clients
	std::map< std::string, ClientInfo>::iterator iterClient = m_pMapClientInfo->begin();
	std::map< std::string, ClientInfo>::iterator iterClientEnd = m_pMapClientInfo->end();
	int iUser = 0;
	while (iterClient != iterClientEnd)
	{
		std::string strText = iterClient->first;
		if (strText.length() < NetworkValues::MAX_NAME_LENGTH)
		{
			//Set the user name in the active client list in the server info to the current user in the map of active clients
			strcpy_s(_pDataToSend->serverInfo.activeClientList[iUser].cUserName, strText.c_str());
		}

		//Set the active-ness of the current user in the active client list in the server info to the activeness of the client in the map
		_pDataToSend->serverInfo.activeClientList[iUser].bActive = iterClient->second.bActive;

		//Get next clients user name
		iterClient++;
		//Increment the iUser
		iUser++;
	}
}

void CServerApp::SetGameState(ClientDataPacket* _pDataToSend)
{
	//set the player states based on the game
	m_pGame->SetPlayerStates(_pDataToSend);

	//Set the enemyStates based on the game
	m_pGame->SetEnemyStates(_pDataToSend);

	//Set the PowerUpStates based on the game
	m_pGame->SetPowUpStates(_pDataToSend);

	//Set the Projectile states based on the game
	m_pGame->SetProjectileStates(_pDataToSend);
}

bool CServerApp::AddUser(std::string _UserName, ClientInfo _ClientInfo)
{
	std::pair<std::map<std::string, ClientInfo>::iterator, bool> MapClientIter;
	
	//Initialise this client to in active
	_ClientInfo.bActive = false;
	_ClientInfo.ServerSocAddr = m_pServer->GetServerSocketAddress();
		
	MapClientIter = m_pMapClientInfo->insert(std::pair<std::string, ClientInfo>(_UserName, _ClientInfo));

	//if the user is added to this map add it to the game as well
	if (MapClientIter.second == true)
	{
		m_pGame->AddPlayer(_UserName);
	}

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapClientIter.second;
}

bool CServerApp::AllActive()
{
	//Set all active to true
	bool bAllActive = true;
	if (m_pMapClientInfo->size() < 1)
	{
		bAllActive = false;
	}

	//Search through all clients 
	std::map< std::string, ClientInfo>::iterator iterClient = m_pMapClientInfo->begin();
	std::map< std::string, ClientInfo>::iterator iterClientEnd = m_pMapClientInfo->end();
	while (iterClient != iterClientEnd)
	{
		//If any client is not active 
		if (iterClient->second.bActive == false)
		{
			//set all active to false
			bAllActive = false;
		}
		//Check next in map
		iterClient++;
	}

	//Return all active
	return bAllActive;
}

bool CServerApp::SendToAll(ClientDataPacket* _pDataToSend)
{
	//Send to all clients 
	std::map< std::string, ClientInfo>::iterator iterClient = m_pMapClientInfo->begin();
	std::map< std::string, ClientInfo>::iterator iterClientEnd = m_pMapClientInfo->end();
	while (iterClient != iterClientEnd)
	{
		//Failed to send
		bool bSuccessSendingAll  = m_pServer->SendData(_pDataToSend, iterClient->second.clientSocAddr);
		assert(("Failed to send to a client", bSuccessSendingAll));
		
		//Get next client
		iterClient++;
	}

	return true;
}

void CServerApp::CheckCreation()
{
	//Check enemies
	EnemyStates* pTempEnemy = new EnemyStates;
	while (m_pGame->GetNextCreatedEnemy(pTempEnemy) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_CREATE_ENEMY;
		//Save the enemy 
		m_pClientPacket->singleEnemyInfo = *pTempEnemy;

		SendToAll(m_pClientPacket);
	}
	delete pTempEnemy;
	pTempEnemy = 0;


	//Check Power Ups
	PowerUpStates* pTempPowUp = new PowerUpStates;
	while (m_pGame->GetNextCreatedPowerUp(pTempPowUp) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_CREATE_POWERUP;
		//Save the power up
		m_pClientPacket->singlePowUpInfo = *pTempPowUp;

		SendToAll(m_pClientPacket);
	}
	delete pTempPowUp;
	pTempPowUp = 0;

	//Check Power Ups
	ProjectileStates* pTempBullet = new ProjectileStates;
	while (m_pGame->GetNextCreatedProjectile(pTempBullet) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_CREATE_PROJECTILE;
		//Save the bullet
		m_pClientPacket->singleProjectileInfo = *pTempBullet;

		SendToAll(m_pClientPacket);
	}
	delete pTempBullet;
	pTempBullet = 0;

}

void CServerApp::CheckDeletion()
{
	//Check enemies
	EnemyStates* pTempEnemy = new EnemyStates;
	while (m_pGame->GetNextDeletedEnemy(pTempEnemy) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_DELETE_ENEMY;
		//Save the enemy 
		m_pClientPacket->singleEnemyInfo = *pTempEnemy;

		SendToAll(m_pClientPacket);
	}
	delete pTempEnemy;
	pTempEnemy = 0;


	//Check Power Ups
	PowerUpStates* pTempPowUp = new PowerUpStates;
	while (m_pGame->GetNextDeletedPowerUp(pTempPowUp) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_DELETE_POWERUP;
		//Save the power up
		m_pClientPacket->singlePowUpInfo = *pTempPowUp;

		SendToAll(m_pClientPacket);
	}
	delete pTempPowUp;
	pTempPowUp = 0;

	//Check Power Ups
	ProjectileStates* pTempBullet = new ProjectileStates;
	while (m_pGame->GetNextDeletedProjectile(pTempBullet) == true)
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_DELETE_PROJECTILE;
		//Save the bullet
		m_pClientPacket->singleProjectileInfo = *pTempBullet;

		SendToAll(m_pClientPacket);
	}
	delete pTempBullet;
	pTempBullet = 0;

}