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
	delete m_pMapClients;
	m_pMapClients = 0;
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

bool CServerApp::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	//Initialise Server app Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Initialise member variables
	m_pServer = new CServer();
	VALIDATE(m_pServer->Initialise());
	m_pServerDataQueue = new std::queue < ServerDataPacket >;
	m_pServerPacket = new ServerDataPacket;
	m_pClientPacket = new ClientDataPacket;
	strHostClient = "";
	//Initialise the map of client addresses
	m_pMapClients = new std::map < std::string, sockaddr_in > ;

	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CServerApp::ReceiveDataThread, (this));

	

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
		case PT_CREATE:
		{
			ProcessCreation();
		}
			break;
		default:
			break;
		}
		
		//TO DO: DO some stuff with the ServerPacket
		//TO DO : remove testing send and receive 

		//ClientDataPacket* ClientPacket = new ClientDataPacket;
		//ClientPacket->iNumber = 100;

		//std::string steTest = "Replied from server: i received yo shit: ";
		////steTest.append(ServerPacket->cText);
		//int ser = steTest.length();

		//strcpy_s(ClientPacket->cText, steTest.c_str());
		//m_pServer->SendData(ClientPacket);
	}
}

void CServerApp::ProcessCreation()
{
	//Convert the text in received packet to a string to be manipulated
	std::string strCreation(m_pServerPacket->cText);

	//Separate the user and server name
	std::size_t found = strCreation.find(":");
	std::string strUserName = strCreation.substr(found + 1);
	m_strServerName = strCreation.substr(0, found);

	//Add the user to the map of clients
	if (AddUser(strUserName, m_pServerPacket->socReceivedFrom))
	{
		//set this client to the host client of this server
		strHostClient = strUserName;

		m_pClientPacket->packetType = PT_CREATE;
		m_pClientPacket->bSuccess = true;
	}
	else
	{
		m_pClientPacket->packetType = PT_CREATE;
		m_pClientPacket->bSuccess = false;
	}

	//send back successful creation 
	m_pServer->SendData(m_pClientPacket);
	
}

bool CServerApp::AddUser(std::string _UserName, sockaddr_in _ClientAddress)
{
	std::pair<std::map<std::string, sockaddr_in>::iterator, bool> MapIterater;

	//Add passed in params to map of clients
	MapIterater = m_pMapClients->insert(std::pair<std::string, sockaddr_in>(_UserName, _ClientAddress));

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapIterater.second;
}