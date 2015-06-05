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
	//Create the game instance if it doesnt exist 
	if (s_pClientApp == 0)
	{
		s_pClientApp = new CClientApp();
	}
	return (*s_pClientApp);
}

void CClientApp::DestroyInstance()
{
	
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
	m_strMainMenuOptions.push_back("START");
	m_strMainMenuOptions.push_back("OPTIONS");
	m_strMainMenuOptions.push_back("INSTRUCTIONS");
	m_strMainMenuOptions.push_back("EXIT");
	m_strStartOptions.push_back("JOIN GAME");
	m_strStartOptions.push_back("HOST GAME");
	m_strStartOptions.push_back("BACK");
	m_strExitOptions.push_back("YES - Close The Game");
	m_strExitOptions.push_back("NO - Take Me Back");

	m_GameState = GS_MAIN_MENU;
	//Initialize the mouse boolean array
	m_bIsClicked = new bool[10];
	//Set all values to false
	memset(m_bIsClicked, false, 10);

	//Initialise Networking variables
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());
	ServerDataPacket* m_ServerPacket = new ServerDataPacket;
	ClientDataPacket* m_ClientPacket = new ClientDataPacket;
	m_pClientDataQueue = new std::queue < ClientDataPacket >;
	//Create and run separate thread to constantly receive data
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
//	int error = (int)ShellExecuteA(_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);



	

	return true;

}

void CClientApp::Process()
{
	
	//ProcessReceiveData();
	

	//TO DO: Remove, This is a send receive test
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
		//Menu states
		case GS_MAIN_MENU:
		{
			MainMenuDraw();
		}break;
		case GS_START_MENU:
		{
			StartMenuDraw();
		}break;
		case GS_OPTIONS_MENU:
		{
			//TO DO
		}break;
		case GS_INSTRUCTIONS:
		{
			//TO DO
		}break;
		case GS_EXIT_MENU:
		{
			ExitMenuDraw();
		}break;

		//Game states
		case GS_JOIN_GAME:
		{
			//TO DO
		}break;
		case GS_HOST_GAME:
		{
			HostGameDraw();
		}break;


	}

	m_pRenderManager->EndRender();
}

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
		case TEXT_TITLE:
		{
			TextColor = D3DCOLOR_XRGB(0, 255, 0);

		}break;

		case TEXT_MAIN_MENU:
		{
			TextColor = D3DCOLOR_XRGB(0, 0, 255);

		}break;

		case TEXT_MENU_SELECT:
		{
			TextColor = D3DCOLOR_XRGB(0, 0, 255);

			//If you hover over a click-able text
			if (m_MousePosition.y >= Rect.top &&
				m_MousePosition.y <= Rect.bottom)
			{
				//Change its color signifying that its click-able
				TextColor = D3DCOLOR_XRGB(255, 0, 0);
				//If the text was clicked
				if (m_bIsClicked[MK_LBUTTON])
				{
					//Run menu selection
					MenuSelection(_strText);
					//Set Left mouse button to false
					m_bIsClicked[MK_LBUTTON] = false;
				}
			}

		}break;

		
	}
	
	//Render the title 
	m_pRenderManager->RenderText(_strText, Rect, TextColor, _TextType);

}

void CClientApp::MenuSelection(std::string _strMenuItem)
{
	//Depending on the current game state run the designated Menu select 
	switch (m_GameState)
	{
		case GS_MAIN_MENU:
		{
			MainMenuSelect(_strMenuItem);
		}break;
		
		case GS_START_MENU:
		{
			StartMenuSelect(_strMenuItem);
		}break;

		case GS_OPTIONS_MENU:
		{
			//TO DO
			//OptionsMenuSelect(std::string _strMenuItem);
		}break;

		case GS_INSTRUCTIONS:
		{
			//TO DO
			//InstructMenuSelect(std::string _strMenuItem);
		}break;

		case GS_EXIT_MENU:
		{
			ExitMenuSelect(_strMenuItem);
		}break;

		default:
			break;
	}

}

void CClientApp::StartMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	////***START MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strStartOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strStartOptions[i], iYPos, TEXT_MENU_SELECT);
	}
}

void CClientApp::ExitMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
	iYPos += 200;
	RenderText("Are You Sure You Want To Exit?", iYPos, TEXT_MAIN_MENU);
	

	////***EXIT MENU***
	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 180 - (uiFontHeight + 100);
	for (unsigned int i = 0; i < m_strExitOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 100);
		RenderText(m_strExitOptions[i], iYPos, TEXT_MENU_SELECT);
	}

}

void CClientApp::HostGameDraw()
{
	//TO DO
	throw std::logic_error("The method or operation is not implemented.");
}

void CClientApp::MainMenuSelect(std::string _strMenuItem)
{
	//Run through main menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strMainMenuOptions.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strMainMenuOptions[iMenuItem])
		{
			break;
		}
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0: //START
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_START_MENU;
		
	}break;

	case 1: //OPTIONS
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_OPTIONS_MENU;
		
	}break;

	case 2: //INSTRUCTIONS
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_INSTRUCTIONS;
		
	}break;

	case 3:  //EXIT
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_EXIT_MENU;
		
	}break;

	default:
		break;
	}
}

void CClientApp::StartMenuSelect(std::string _strMenuItem)
{
	//Run through main menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strStartOptions.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strStartOptions[iMenuItem])
		{
			break;
		}
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0: //JOIN
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_JOIN_GAME;
	}break;

	case 1: //HOST
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_HOST_GAME;
	}break;

	case 3: //BACK
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_MAIN_MENU;
	}break;
	
	default:
		break;
	}
}

void CClientApp::ExitMenuSelect(std::string _strMenuItem)
{
	//TO DO
	//Run through main menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strExitOptions.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strExitOptions[iMenuItem])
		{
			break;
		}
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0: //YES
	{
		//End the program
		exit(0);
	}break;

	case 1: //NO
	{
		m_pRenderManager->Clear(true, true, false);
		m_GameState = GS_MAIN_MENU;
	}break;

	default:
		break;
	}
}

void CClientApp::MainMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);
	
	////***MAIN MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strMainMenuOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strMainMenuOptions[i], iYPos, TEXT_MENU_SELECT);
	}
	
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
	//TO DO
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
