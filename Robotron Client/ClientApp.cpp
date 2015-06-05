//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CClientApp.cpp
// Description : The game functionality for the client
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "ClientApp.h"

//Global statics
CMySemaphore CClientApp::s_Mutex(1);
CClientApp* CClientApp::s_pClientApp = 0;

CClientApp::CClientApp(void)
{

}

CClientApp::~CClientApp(void)
{
	delete m_pClient;
	m_pClient = 0;

	//delete m_pClientDataQueue
}

CClientApp& CClientApp::GetInstance()
{
	//Create the game intance if it doesnt exist 
	if (s_pClientApp == 0)
	{
		s_pClientApp = new CClientApp();
	}
	return (*s_pClientApp);
}

void CClientApp::DestroyInstance()
{
	//join threads

	delete s_pClientApp;
	s_pClientApp = 0;
}

bool CClientApp::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	//Initialise game Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Initialise member variables
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());
	m_pClientDataQueue = new std::queue < ClientDataPacket > ;
	

	//Initialize the key down boolean array
	m_bIsKeyDown = new bool[255];
	//Set all key downs to false
	memset(m_bIsKeyDown, false, 255);

	//TO DO
	std::string filename;
#ifdef _DEBUG
	filename = "..\\Debug\\Robotron Server";
#endif
#ifndef _DEBUG
	//TO DO: change file path for final release
	filename = "..\\Release\\Robotron Server";
#endif

	//int errer = (int)ShellExecuteA(_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);

	//Create and run separate thread to constantly recieve data
	m_ClientThread = std::thread(&CClientApp::ReceiveDataThread, (this));

	return true;

}

void CClientApp::Process()
{
	
	//TO DO: Remove, This is a send recieve test
	ServerDataPacket* packet = new ServerDataPacket;
	packet->iNumber = 78;
	std::string steTest = "1234567890";
	strcpy_s(packet->cText, steTest.c_str());

	m_pClient->SendData(packet);

	ClientDataPacket* ClientPacket = new ClientDataPacket;
	m_pClient->ReceiveData(ClientPacket);

}

void CClientApp::Draw()
{

}

void CClientApp::RenderSingleFrame()
{
	//TO DO 
	//if game is in main menu run main menu


	ProcessReceiveData();
	Process();
		
}

void CClientApp::ConvertToServerDataPacket(std::string _srtData)
{
	ServerDataPacket* packet = new ServerDataPacket;
	if (_srtData.length() < NetworkValues::MAX_CHAR_LENGTH)
	{
		strcpy_s(packet->cText, _srtData.c_str());
	}
}

void CClientApp::ReceiveDataThread()
{
	ClientDataPacket* pClientPacket = new ClientDataPacket;
	while (m_pClient->getActive())
	{
		if (m_pClient->ReceiveData(pClientPacket))
		{
			//***CRITICAL SECTION***
			//Stop other threads looking at the queue
			//While one thread is in the process of acquiring an item from it
			s_Mutex.Wait();
			//Push onto the work queue
			m_pClientDataQueue->push(*pClientPacket);
			//Signal that you are done with the queue
			s_Mutex.Signal();
		}

	}

	//Memory clean up
	delete pClientPacket;
	pClientPacket = 0;
}

void CClientApp::ProcessReceiveData()
{
	while (m_pClientDataQueue->empty() == false)
	{
		//***CRITICAL SECTION***
		//Stop other threads looking at the queue
		//While one thread is in the process of acquiring an item from it
		s_Mutex.Wait();

		//Check to see if the queue is empty
		if (m_pClientDataQueue->empty())
		{
			//If queue is empty send release signal and break out of the while loop
			s_Mutex.Signal();
			break;
		}

		//Get item from the work queue
		ClientDataPacket m_ServerPacket = m_pClientDataQueue->front();
		m_pClientDataQueue->pop();
		//Signal that you are done with the queue
		s_Mutex.Signal();

		int c = 9;
		//TO DO: DO some stuff with the ServerPacket

	}
}
