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
	delete m_ServerPacket;
	m_ServerPacket = 0;
	delete m_ClientPacket;
	m_ClientPacket = 0;
}

CServerApp& CServerApp::GetInstance()
{
	//Create the game intance if it doesnt exist 
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
	ServerDataPacket* m_ServerPacket = new ServerDataPacket;
	ClientDataPacket* m_ClientPacket = new ClientDataPacket;

	//Create and run separate thread to constantly recieve data
	m_RecieveThread = std::thread(&CServerApp::ReceiveDataThread, (this));

	return true;
}

void CServerApp::Process()
{
	//TO DO : remove testing send and recieve 
	
	ClientDataPacket* ClientPacket = new ClientDataPacket;
	ClientPacket->iNumber = 100;

	std::string steTest = "Replied from server: i recieved yo shit: ";
	//steTest.append(ServerPacket->cText);
	int ser = steTest.length();

	strcpy_s(ClientPacket->cText, steTest.c_str());
	m_pServer->SendData(ClientPacket);

	int c = 8;
}

void CServerApp::Draw()
{

}

void CServerApp::RenderSingleFrame()
{
	
	
	ProcessReceiveData();

	Process();
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
		ServerDataPacket m_ServerPacket = m_pServerDataQueue->front();
		m_pServerDataQueue->pop();
		//Signal that you are done with the queue
		s_Mutex.Signal();

		int c = 9;
		//TO DO: DO some stuff with the ServerPacket

	}
}


