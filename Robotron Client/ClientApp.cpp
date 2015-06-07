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
	//Game

	delete m_pClock;
	m_pClock = 0;
	delete[] m_bIsClicked;
	m_bIsClicked = 0;

	//Networking
	
	//Make sure the the client is set to inactive
	m_pClient->SetActive(false);
	//Join the threads
	m_RecieveThread.join();
	
	delete m_pClient;
	m_pClient = 0;
	delete m_pServerPacket;
	m_pServerPacket = 0;
	delete m_pClientPacket;
	m_pClientPacket = 0;
	delete m_pClientDataQueue;
	m_pClientDataQueue = 0;

	//Graphics
	
	m_pRenderManager->Shutdown();
	delete m_pRenderManager;
	m_pRenderManager = 0;
	

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
	m_bIsHost = false;
	m_pClock = new CClock();
	VALIDATE(m_pClock->Initialise());
	//If adding or changing stings here go set states Dependant
	m_strGameTitle = "Robotron";
	m_strMainMenuOptions.push_back("SINGLE PLAYER");
	m_strMainMenuOptions.push_back("MULTIPLAYER");
	m_strMainMenuOptions.push_back("OPTIONS");
	m_strMainMenuOptions.push_back("INSTRUCTIONS");
	m_strMainMenuOptions.push_back("EXIT");

	m_strStartOptions.push_back("JOIN GAME");
	m_strStartOptions.push_back("HOST GAME");
	m_strStartOptions.push_back("BACK");

	m_strExitOptions.push_back("YES - Close The Game");
	m_strExitOptions.push_back("NO - Take Me Back");
	
	m_eGameState = GS_MENU;
	m_eHostState = HS_DEFAULT;
	m_eMenuState = MS_MAIN;
	//Initialize the mouse boolean array
	m_bIsClicked = new bool[10];
	//Set all values to false
	memset(m_bIsClicked, false, 10);
	//Initialize the key down boolean array
	m_bIsKeyDown = new bool[255];
	//Set all key downs to false
	memset(m_bIsKeyDown, false, 255);
	
	//Initialise Networking variables
	m_strServerName = "";
	m_strUserName = "";
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());
	m_pServerPacket = new ServerDataPacket;
	m_pClientPacket = new ClientDataPacket;
	m_pClientDataQueue = new std::queue < ClientDataPacket >;

	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CClientApp::ReceiveDataThread, (this));
	
	//Initialise Graphic variables
	m_pRenderManager = new CD3D9Renderer();
	//TO DO: Full screen???
	bool bFullScreen = false;
	VALIDATE(m_pRenderManager->Initialise(_iScreenWidth, _iScreenHeight, _hWnd, bFullScreen));

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

void CClientApp::ProcessInputs(int _iInput)
{
	switch (m_eGameState)
	{
		case GS_MENU:
		{
			switch (m_eMenuState)
			{
				case MS_MAIN:
					break;
				case MS_SINGLE_PLAYER:
					break;
				case MS_MULTI_PLAYER:
					break;
				case MS_OPTIONS:
					break;
				case MS_INSTRUCTIONS:
					break;
				case MS_EXIT:
					break;
				case MS_JOIN_GAME:
					break;
				case MS_HOST_GAME:
				{
					ProcessHostGame(_iInput);
				}
				break;
				default:
					break;
			}
		}
			break;
		case GS_PLAY:
			break;
		default:
			break;
	}
	

}

void CClientApp::ProcessHostGame(int _iInput)
{
	switch (m_eHostState)
	{
		case HS_DEFAULT:
		{
			
		}
			break;
		case HS_SERVER_NAME:
		{
			if (_iInput == 13)
			{
				m_eHostState = HS_USER_NAME;
				int c = 9;
			}
			else
			{
				ProcessTextInput(&m_strServerName, _iInput);
			}
		}
			break;
		case HS_USER_NAME:
		{
			if (_iInput == 13)
			{
				m_eHostState = HS_DONE;
			}
			else
			{
				ProcessTextInput(&m_strUserName, _iInput);
			}
		}
			break;
		case HS_DONE:
			{
				

			
			}break;
		default:
			break;
	}
}

void CClientApp::ProcessTextInput(std::string* _strText, int _iInput)
{
	std::string strTemp;
	strTemp = *_strText;

	//If shift is not down
	if (m_bIsKeyDown[VK_SHIFT] != true)
	{
		if ((_iInput >= 65 && _iInput <= 90) ||
			(_iInput >= 97 && _iInput <= 122))
		{
			//and a char has been pressed, force lower case
			_iInput += 32;
		}
	}

	char cUserInput = _iInput;
	
	if (cUserInput == '\b') 
	{
		//Backspace has been pressed remove a char
		strTemp = _strText->substr(0, _strText->size() - 1);
	}
	else if ((_iInput >= 65 && _iInput <= 90) ||
			(_iInput >= 97 && _iInput <= 122))
	{
		//char has been pressed add it to the string
		strTemp += cUserInput;
	}
	
	*_strText = strTemp;
}

void CClientApp::Draw()
{
	m_pRenderManager->StartRender(true, true, false);

	switch (m_eGameState)
	{
	case GS_MENU:
	{
		switch (m_eMenuState)
		{
			//Menu states
		case MS_MAIN:
		{
			MainMenuDraw();
		}break;
		case MS_SINGLE_PLAYER:
		{
			//TO DO: Start a single player instance
			// StartSinglePlayerDraw();
		}break;
		case MS_MULTI_PLAYER:
		{
			StartMultiPlayerDraw();
		}break;
		case MS_OPTIONS:
		{
			//TO DO
		}break;
		case MS_INSTRUCTIONS:
		{
			//TO DO
		}break;
		case MS_EXIT:
		{
			ExitMenuDraw();
		}break;

		//Game states
		case MS_JOIN_GAME:
		{
			//TO DO
		}break;
		case MS_HOST_GAME:
		{
			HostGameDraw();
		}break;
		}
	}
		break;
	case GS_PLAY:
		break;
	default:
		break;
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
	switch (m_eMenuState)
	{
		case MS_MAIN:
		{
			MainMenuSelect(_strMenuItem);
		}break;
		
		case MS_SINGLE_PLAYER:
		{
			//TO DO
			// //StartMenuSelect(_strMenuItem);
		}break;

		case MS_MULTI_PLAYER:
		{
			StartMultiplayerMenuSelect(_strMenuItem);
		}break;
		
		case MS_OPTIONS:
		{
			//TO DO
			//OptionsMenuSelect(std::string _strMenuItem);
		}break;

		case MS_INSTRUCTIONS:
		{
			//TO DO
			//InstructMenuSelect(std::string _strMenuItem);
		}break;

		case MS_EXIT:
		{
			ExitMenuSelect(_strMenuItem);
		}break;

		default:
			break;
	}

}

void CClientApp::StartMultiPlayerDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	////***Multiplayer MENU***
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
		
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	switch (m_eHostState)
	{
		case HS_DEFAULT:
		{
			m_eHostState = HS_SERVER_NAME;
		}
			break;
		case HS_SERVER_NAME:
		{
			//***Host MENU***
			int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
			iYPos += 200; //- (uiFontHeight + 1);
			RenderText("Enter Server Name: ", iYPos, TEXT_MAIN_MENU);

			iYPos += (uiFontHeight + 100);
			RenderText(m_strServerName, iYPos, TEXT_MAIN_MENU);
		}
			break;
		case HS_USER_NAME:
		{
			//***Host MENU***
			int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
			iYPos += 200; //- (uiFontHeight + 1);
			RenderText("Enter User Name: ", iYPos, TEXT_MAIN_MENU);

			iYPos += (uiFontHeight + 100);
			RenderText(m_strUserName, iYPos, TEXT_MAIN_MENU);
		}
			break;
		case HS_DONE:
		{
			//TO DO:
			//Run Game
			
			//only run this if you are not yet a host, ie this only gets run once 
			if (m_bIsHost == false)
			{
				m_bIsHost = true;

				//Run Server exe
				std::string filename;

				#ifdef _DEBUG
								filename = "..\\Debug\\Robotron Server";
				#endif
				#ifndef _DEBUG
								//TO DO: change file path for final release
								filename = "..\\Release\\Robotron Server";
				#endif

				//TO DO: hide server
				int error = (int)ShellExecuteA(m_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);

				if (error > 32) //Server exe opened successfully
				{
					//RETURN HERE
					//Send Stuff to Server
					//OWNAGE
					// SERVER NAME
					// USER NAME OF CLIENT
					// ECT
					
				}

			}
		}
		default:
			break;
	}
	
	//RETURN HERE

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
	case 0: //Single player
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_SINGLE_PLAYER;

	}break;

	case 1: //Multi player
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MULTI_PLAYER;
		
	}break;

	case 2: //OPTIONS
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_OPTIONS;
		
	}break;

	case 3: //INSTRUCTIONS
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_INSTRUCTIONS;
		
	}break;

	case 4:  //EXIT
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_EXIT;
		
	}break;

	default:
		break;
	}
}

void CClientApp::StartMultiplayerMenuSelect(std::string _strMenuItem)
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
		m_eMenuState = MS_JOIN_GAME;
		m_bIsHost = false;
	}break;

	case 1: //HOST
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_HOST_GAME;
	}break;

	case 2: //BACK
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
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
		//TO DO, memory leak stuff
		m_pClient->SetActive(false);
	}break;

	case 1: //NO
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
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

bool CClientApp::RenderSingleFrame()
{
	//TO DO 
	//if game is in main menu run main menu
	if (m_pClient->GetActive() == false)
	{
		return false;
	}


			
	Process();
	Draw();

	return true;
		
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
	while (m_pClient->GetActive())
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
		*m_pClientPacket = (m_pClientDataQueue->front());
		m_pClientDataQueue->pop();
		//Signal that you are done with the queue
		s_Mutex.Signal();

		int c = 9;
		//TO DO: DO some stuff with the m_pClientPacket

	}
}
