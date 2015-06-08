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

bool CServerApp::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight, LPWSTR* _pCmdArgs)
{
	//Initialise Server app Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Store the server arguments
	m_strServerName = WideStringToString(_pCmdArgs[1]);
	m_strHostClient = WideStringToString(_pCmdArgs[2]);

	//Initialise member variables
	m_pServer = new CServer();
	VALIDATE(m_pServer->Initialise());
	m_pServerDataQueue = new std::queue < ServerDataPacket >;
	m_pServerPacket = new ServerDataPacket;
	m_pClientPacket = new ClientDataPacket;
	//Initialise the map of client addresses
	m_pMapClients = new std::map < std::string, sockaddr_in > ;

	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CServerApp::ReceiveDataThread, (this));

	m_pClientPacket->packetType = PT_CREATE;
	m_pClientPacket->socReceivedFrom = m_pServer->GetServerSocketAddress();
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

void CServerApp::AddTextToClientDataPacket(std::string _srtText)
{
	//convert and add the string to the ServerDataPacket
	if (_srtText.length() < NetworkValues::MAX_CHAR_LENGTH)
	{
		strcpy_s(m_pClientPacket->cText, _srtText.c_str());
	}
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
		case PT_FIND:
		{
			ProcessFind();
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
		//Client has sent back that it is this servers host

		//Give the server the host clients information
		//So that the server knows who its hosting client is
		m_pServer->SetHostClientInfo(m_strHostClient, m_pServerPacket->socReceivedFrom);

		//Add the client to the map of clients
		m_pServer->AddUser(m_strHostClient, m_pServerPacket->socReceivedFrom);
		
	}
	
}

void CServerApp::ProcessFind()
{
	m_pClientPacket->packetType = PT_FIND;
	AddTextToClientDataPacket(m_strServerName);
	m_pClientPacket->iNum = m_pMapClients->size();

	m_pServer->SendData(m_pClientPacket, m_pServerPacket->socReceivedFrom);
}

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

