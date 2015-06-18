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

	//Send message
	m_pServer->SendData(m_pClientPacket);
	m_pServer->RemoveAll();

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
	delete m_pMapPlayerStates;
	m_pMapPlayerStates = 0;
	/*delete m_pMapClients;
	m_pMapClients = 0;*/
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
	
	//Initialise Map of player states
	m_pMapPlayerStates = new std::map < std::string, PlayerStates > ;

	//Set Server info on creation
	SetServerInfo();
	
	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CServerApp::ReceiveDataThread, (this));

	m_pClientPacket->packetType = PT_CREATE;
	m_pClientPacket->serverInfo.serverSocAddr = m_pServer->GetServerSocketAddress();
	//Send back the the server name and Host client names to be checked against
	std::string strTextToSend = m_strServerName + ":" + m_strHostClient;
	AddTextToClientDataPacket(strTextToSend);
	
	//Broadcast to all potential clients
	m_pServer->Broadcast(m_pClientPacket);
		
	return true;
}

void CServerApp::Process()
{	
	ProcessReceiveData();


	
}

void CServerApp::Draw()
{

}

bool CServerApp::RenderSingleFrame()
{
	if (m_pServer->getActive() == false)
	{
		return false;
	}
	
	ProcessReceiveData();

	Process();

	return true;
}

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
			ProcessJoinRequest();
		}
			break;

		case PT_ACTIVE:
		{
			ProcessActive();
		}
			break;

		case PT_LEAVE:
		{
			ProcessLeave();	
		}
		break;
		case PT_QUIT:
		{
			ProcessQuit();
		}
		case PT_INPUT:
		{
			//Add server info to client packet
			SetServerInfo();
			//Send to all so so has left
			m_pClientPacket->packetType = PT_UPDATE;
			
			m_pGame->Process(m_pServerPacket, m_pClientPacket);

			m_pServer->SendData(m_pClientPacket);
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
		m_pServer->AddUser(m_strHostClient, m_pServerPacket->clientInfo.clientSocAddr);

		//Add to the map of players states
		std::string strUserName(m_pServerPacket->clientInfo.cUserName);
		PlayerStates tempState;
		strcpy_s(tempState.cPLayerName, m_pServerPacket->clientInfo.cUserName);
		tempState.f3Positions = { 0, 20, 0 };
		AddUser(strUserName, tempState);
		
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

void CServerApp::ProcessJoinRequest()
{
	//Initialize and create join request message
	m_pClientPacket->packetType = PT_JOIN_REQUEST;
	m_pClientPacket->bSuccess = false;
	
	//Try add user and its socket address to the map of clients
	if (m_pServer->AddUser(m_pServerPacket->clientInfo.cUserName, m_pServerPacket->clientInfo.clientSocAddr))
	{
		//Add to the map of players states
		std::string strUserName(m_pServerPacket->clientInfo.cUserName);
		PlayerStates tempState;
		strcpy_s(tempState.cPLayerName, m_pServerPacket->clientInfo.cUserName);
		tempState.f3Positions = { 0, 20, 0 };
		AddUser(strUserName, tempState);
		
		m_pClientPacket->bSuccess = true;
	}
	else
	{
		//User name already in use
		std::string strError = "<KW>INVALID_USERNAME";
		AddTextToClientDataPacket(strError);
	}
			
	//Add server info to client packet
	SetServerInfo();


	//Send message
	m_pServer->SendData(m_pClientPacket, m_pServerPacket->clientInfo.clientSocAddr);
}

void CServerApp::ProcessActive()
{
	std::string strUserName(m_pServerPacket->clientInfo.cUserName);
	//Set the active-ness based on the bSuccess value
	m_pServer->SetActiveClient(strUserName, m_pServerPacket->bSuccess);

	//Check if all clients are active
	if (m_pServer->AllActive())
	{
		//Add server info to client packet
		SetServerInfo();
		//Send to all so so has left
		m_pClientPacket->packetType = PT_GAME_START;
		
		//Single player
		if (m_bSinglePlayer == true)
		{
			std::vector<PlayerStates> vecPlayerStates;

			std::map< std::string, PlayerStates>::iterator iterClient = m_pMapPlayerStates->begin();

			vecPlayerStates.push_back(iterClient->second);
			//Create and initialize the server side game
			m_pGame = &(CGame::GetInstance());
			m_pGame->Initialise(vecPlayerStates, m_pClientPacket);

			//Send message
			m_pServer->SendData(m_pClientPacket);
			//Set Game to start
			m_bGameStart = true;
		}
		else //Multiplayer
		{
			//If more than 2 players are in the lobby
			if (m_pServer->GetNumClients() > 1)
			{

				std::vector<PlayerStates> vecPlayerStates;
				//Set player positions
				std::map< std::string, PlayerStates>::iterator iterClient = m_pMapPlayerStates->begin();
				std::map< std::string, PlayerStates>::iterator iterClientEnd = m_pMapPlayerStates->end();
				int iClient = 0;
				while (iterClient != iterClientEnd)
				{
					//Add the player state to the temp vector
					vecPlayerStates.push_back(iterClient->second);

					iClient++;
					iterClient++;
				}

				//Create and initialize the server side game
				m_pGame = &(CGame::GetInstance());
				m_pGame->Initialise(vecPlayerStates, m_pClientPacket);
				

				//Send message
				m_pServer->SendData(m_pClientPacket);
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
		AddTextToClientDataPacket(strUserName);

		//Send message
		m_pServer->SendData(m_pClientPacket);
		m_pServer->RemoveUser(strUserName);
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
		m_pServer->SendData(m_pClientPacket);
		m_pServer->RemoveAll();
		//Sleep for a small time to make sure every one got the message
		Sleep(50);
		m_pServer->SetActive(false);
	}
}

//String manipulation
std::string CServerApp::WideStringToString(wchar_t* _wstr)
{
	//Convert the Wide String to a standard string
	size_t lengthWstr = (wcslen(_wstr) + 1);
	size_t convertedCount = 0;
	char* cConversion = new char[lengthWstr * 2];
	wcstombs_s(&convertedCount, cConversion, lengthWstr, _wstr, _TRUNCATE);

	std::string strConverted = (std::string)(cConversion);

	//Deallocate memory
	delete cConversion;
	cConversion = 0;

	//Return the converted standard string
	return strConverted;
}

//Packet manipulations
void CServerApp::SetServerInfo()
{
	/*<SERVER_INFO>*/
	//Server Name
	AddServerToServerInfo(m_strServerName);
	//Host Name
	AddHostNameToServerInfo(m_strHostClient);
	//Socket address
	m_pClientPacket->serverInfo.serverSocAddr = m_pServer->GetServerSocketAddress();
	//Number of clients
	m_pClientPacket->serverInfo.iNumClients = m_pServer->GetNumClients();
	//Client List
	m_pServer->SetActiveClientList(m_pClientPacket);
	//Set game states
	SetGameState(m_pClientPacket);
}

void CServerApp::AddServerToServerInfo(std::string _srtServerName)
{
	//Add Server name to the server info of the Client packet
	if (_srtServerName.length() < NetworkValues::MAX_NAME_LENGTH + 1)
	{
		strcpy_s(m_pClientPacket->serverInfo.cServerName, _srtServerName.c_str());
	}
}

void CServerApp::AddHostNameToServerInfo(std::string _srtHostName)
{
	//Add host name to the server info of the Client packet
	if (_srtHostName.length() < NetworkValues::MAX_NAME_LENGTH + 1)
	{
		strcpy_s(m_pClientPacket->serverInfo.cHostName, _srtHostName.c_str());
	}
}

void CServerApp::AddTextToClientDataPacket(std::string _srtText)
{
	//convert and add the string to the ServerDataPacket
	if (_srtText.length() < NetworkValues::MAX_CHAR_LENGTH)
	{
		strcpy_s(m_pClientPacket->cText, _srtText.c_str());
	}
}

bool CServerApp::AddUser(std::string _UserName, PlayerStates _playerStates)
{

	std::pair<std::map<std::string, PlayerStates>::iterator, bool> MapClientIter;

	//Add the players with there states into the map
	MapClientIter = m_pMapPlayerStates->insert(std::pair<std::string, PlayerStates>(_UserName, _playerStates));

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapClientIter.second;
}

void CServerApp::SetGameState(ClientDataPacket* _pDataToSend)
{
	//Reset the list of active clients
	for (int iUser = 0; iUser < NetworkValues::MAX_USERS; iUser++)
	{
		//Set the user name to nothing
		std::string strText = "";
		strcpy_s(_pDataToSend->PlayerInfo[iUser].cPLayerName, strText.c_str());
		//Reset positions
		_pDataToSend->PlayerInfo[iUser].f3Positions = { 0, 0, 0 };
		
	}

	//Run through the map of active clients
	std::map< std::string, PlayerStates>::iterator iterClient = m_pMapPlayerStates->begin();
	std::map< std::string, PlayerStates>::iterator iterClientEnd = m_pMapPlayerStates->end();
	int iUser = 0;
	while (iterClient != iterClientEnd)
	{
		std::string strText = iterClient->first;
		if (strText.length() < NetworkValues::MAX_NAME_LENGTH)
		{
			//Set the user name in the active client list in the server info to the current user in the map of active clients
			strcpy_s(_pDataToSend->PlayerInfo[iUser].cPLayerName, strText.c_str());
		}

		_pDataToSend->PlayerInfo[iUser].f3Positions = iterClient->second.f3Positions;
		//Get next clients user name
		iterClient++;
		//Increment the iUser
		iUser++;
	}
}