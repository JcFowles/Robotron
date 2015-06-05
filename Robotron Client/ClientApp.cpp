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
	//Initialise Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Initialise Game Variables
	m_pClock = new CClock();
	VALIDATE(m_pClock->Initialise());
	m_strGameTitle = "Robotron";
	m_GameState = MAIN_MENU;

	//Initialise Networking variables
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());
	ServerDataPacket* m_ServerPacket = new ServerDataPacket;
	ClientDataPacket* m_ClientPacket = new ClientDataPacket;
	m_pClientDataQueue = new std::queue < ClientDataPacket >;
	//Create and run separate thread to constantly recieve data
	m_ClientThread = std::thread(&CClientApp::ReceiveDataThread, (this));
	
	//Initialise Graphic variables
	m_pRenderManager = new CD3D9Renderer();
	//TO DO: Full screen???
	bool bFullScreen = false;
	VALIDATE(m_pRenderManager->Initialise(_iScreenWidth, _iScreenHeight, _hWnd, bFullScreen));

	
	//TO DO
	//Initialize the key down boolean array
	//m_bIsKeyDown = new bool[255];
	//Set all key downs to false
	//memset(m_bIsKeyDown, false, 255);

	//TO DO: Run Server function
//	std::string filename;
//#ifdef _DEBUG
//	filename = "..\\Debug\\Robotron Server";
//#endif
//#ifndef _DEBUG
//	//TO DO: change file path for final release
//	filename = "..\\Release\\Robotron Server";
//#endif
//
//	int errer = (int)ShellExecuteA(_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);



	

	return true;

}

void CClientApp::Process()
{
	
	//ProcessReceiveData();
	

	//TO DO: Remove, This is a send recieve test
	/*ServerDataPacket* packet = new ServerDataPacket;
	packet->iNumber = 78;
	std::string steTest = "1234567890";
	strcpy_s(packet->cText, steTest.c_str());

	m_pClient->SendData(packet);

	ClientDataPacket* ClientPacket = new ClientDataPacket;
	m_pClient->ReceiveData(ClientPacket);*/

}

void CClientApp::Draw()
{
	m_pRenderManager->StartRender(true, true, false);

	switch (m_GameState)
	{
		case MAIN_MENU:
		{
			MainMenuDraw();
		}break;

	}

	m_pRenderManager->EndRender();
}

//TO DO FIRST: 
void CClientApp::RenderText(std::string _strText, int _iYPos, eTextType _TextType)
{
	//Create the text space as a RECT
	RECT Rect;
	Rect.left = 0;
	Rect.right = m_iScreenWidth;
	Rect.top = _iYPos;
	int uiFontHeight = m_pRenderManager->GetFontHeight(_TextType);
	Rect.bottom = (Rect.top + uiFontHeight);
	
	//Change the color if the mouse hovers over a main menu rect
	DWORD TextColor = D3DCOLOR_XRGB(0, 0, 0);
	switch (_TextType)
	{
		case TEXT_MAIN_MENU:
		{
			TextColor = D3DCOLOR_XRGB(0, 0, 255);
			if (m_MousePosition.y >= Rect.top &&
				m_MousePosition.y <= Rect.bottom)
			{
				TextColor = D3DCOLOR_XRGB(255, 0, 0);
			}

		}break;

		case TEXT_TITLE:
		{
			TextColor = D3DCOLOR_XRGB(0, 255, 0);

		}break;
	}
	

	//Render the title 
	m_pRenderManager->RenderText(_strText, Rect, TextColor, _TextType);

}

void CClientApp::MainMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);
	
	////***MAIN MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
	RenderText("START", iYPos += 200, TEXT_MAIN_MENU);
	RenderText("OPTIONS", iYPos += uiFontHeight, TEXT_MAIN_MENU);
	RenderText("INSTRUCTIONS", iYPos += uiFontHeight, TEXT_MAIN_MENU);
	RenderText("EXIT", iYPos += uiFontHeight, TEXT_MAIN_MENU);

}

void CClientApp::RenderSingleFrame()
{
	//TO DO 
	//if game is in main menu run main menu
			
	Process();
	Draw();
		
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
