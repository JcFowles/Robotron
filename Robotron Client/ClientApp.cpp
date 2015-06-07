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
	delete[] m_bIsKeyDown;
	m_bIsKeyDown = 0;

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
	m_strClickedMenu = "";
	m_bMenuClicked = false;
	//If adding or changing stings here go set states Dependant
	m_strGameTitle = "Robotron";
	m_strMainMenuOptions.push_back("SINGLE PLAYER");
	m_strMainMenuOptions.push_back("MULTIPLAYER");
	m_strMainMenuOptions.push_back("OPTIONS");
	m_strMainMenuOptions.push_back("INSTRUCTIONS");
	m_strMainMenuOptions.push_back("EXIT");

	m_strMultiPlayerOptions.push_back("Join Game");
	m_strMultiPlayerOptions.push_back("Host Game");
	m_strMultiPlayerOptions.push_back("Back");

	m_strOptionsMenu.push_back("Game options");
	m_strOptionsMenu.push_back("Back");

	m_strInstructions.push_back("Game Instructions");
	m_strInstructions.push_back("Back");

	m_strExitOptions.push_back("Yes - Close The Game");
	m_strExitOptions.push_back("No - Take Me Back");
	
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
	//First thing to do is reprocess received data
	ProcessReceiveData();
	
	switch (m_eGameState)
	{
	case GS_MENU:
	{
		if (m_bMenuClicked)
		{
			ProcessMenuSelection(m_strClickedMenu);
		}
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
			break;
		case MS_LOBBY:
		{
			ProcessLobbyRequest();
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

void CClientApp::ProcessMenuSelection(std::string _strMenuItem)
{
	//Reset menu clicked
	m_bMenuClicked = false;

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
		m_pRenderManager->Clear(true, true, false);
		m_eGameState = GS_PLAY;
	}break;

	case MS_MULTI_PLAYER:
	{
		MultiPlayerMenuSelect(_strMenuItem);
	}break;

	case MS_HOST_GAME:
	case MS_JOIN_GAME:
	{
		HostMenuSelect(_strMenuItem);
	}break;

	case MS_OPTIONS:
	{
		OptionsMenuSelect(_strMenuItem);
	}break;

	case MS_INSTRUCTIONS:
	{
		InstructMenuSelect(_strMenuItem);
	}break;

	case MS_EXIT:
	{
		ExitMenuSelect(_strMenuItem);
	}break;

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

void CClientApp::ProcessLobbyRequest()
{
	//Broadcast to find all active servers
	
	//Save all available servers, and number of users


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

void CClientApp::MultiPlayerMenuSelect(std::string _strMenuItem)
{
	//Run through current menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strMultiPlayerOptions.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strMultiPlayerOptions[iMenuItem])
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

void CClientApp::HostMenuSelect(std::string _strMenuItem)
{
	switch (m_eHostState)
	{
	case HS_DEFAULT:
		break;
	case HS_SERVER_NAME:
	{
		if (_strMenuItem == "Back")
		{
			m_pRenderManager->Clear(true, true, false);
			m_eMenuState = MS_MULTI_PLAYER;
		}
	}
		break;
	case HS_USER_NAME:
	{
		if (_strMenuItem == "Back")
		{
			m_pRenderManager->Clear(true, true, false);
			m_eHostState = HS_SERVER_NAME;
		}
	}
		break;
	case HS_DONE:
		break;
	default:
		break;
	}
}

void CClientApp::OptionsMenuSelect(std::string _strMenuItem)
{
	//Run through current menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strOptionsMenu.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strOptionsMenu[iMenuItem])
		{
			break;
		}
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0:
		break;
	case 1: //Back
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
	}
		break;
	default:
		break;
	}
}

void CClientApp::InstructMenuSelect(std::string _strMenuItem)
{
	//Run through current menu options
	unsigned int iMenuItem;
	for (iMenuItem = 0; iMenuItem < m_strInstructions.size(); iMenuItem++)
	{
		if (_strMenuItem == m_strInstructions[iMenuItem])
		{
			break;
		}
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0:
		break;
	case 1: //Back
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
	}
		break;
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
			MultiPlayerMenuDraw();
		}break;
		case MS_OPTIONS:
		{
			OptionsMenuDraw();
		}break;
		case MS_INSTRUCTIONS:
		{
			InstructionMenuDraw();
		}break;
		case MS_EXIT:
		{
			ExitMenuDraw();
		}break;
		case MS_JOIN_GAME:
		{
			JoinMenuDraw();
		}break;
		case MS_HOST_GAME:
		{
			HostMenuDraw();
		}break;
		case MS_LOBBY:
		{
			LobbyMenuDraw();
		}break;
		default:
			break;
		}
	}
		break;
	case GS_PLAY:
		break;
	
	}



	m_pRenderManager->EndRender();
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

void CClientApp::MultiPlayerMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	//***Multiplayer MENU***
	//get font height 
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strMultiPlayerOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strMultiPlayerOptions[i], iYPos, TEXT_MENU_SELECT);
	}
}

void CClientApp::JoinMenuDraw()
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
		//m_eHostState = HS_USER_NAME;
		int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
		iYPos += 200;
		RenderText("Select Server to join: ", iYPos, TEXT_MAIN_MENU);
		iYPos += (uiFontHeight + 1);

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
		if (m_strActiveServers.size() < 1)	 //No servers found
		{
			//Print no servers found
			iYPos += (uiFontHeight + 1);
			RenderText("No Active Servers Found!", iYPos, TEXT_LIST);
		}
		else //Servers found
		{
			//Print the list of found servers
			uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
			for (unsigned int iActiveServer = m_strActiveServers.size(); iActiveServer > 0; iActiveServer--)
			{
				iYPos += (uiFontHeight + 1);
				RenderText(m_strActiveServers[iActiveServer], iYPos, TEXT_LIST);
			}
		}

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
		iYPos += (uiFontHeight + 1);
		RenderText("Back", iYPos, TEXT_MENU_SELECT);
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
			
	}
	default:
		break;
	}
	
}

void CClientApp::HostMenuDraw()
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

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
		iYPos += (uiFontHeight + 1);
		RenderText("Back", iYPos, TEXT_MENU_SELECT);
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

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
		iYPos += (uiFontHeight + 1);
		RenderText("Back", iYPos, TEXT_MENU_SELECT);
	}
		break;
	case HS_DONE:
	{
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
			//int error = (int)ShellExecuteA(m_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);
			int error = 42;
			if (error > 32) //Server exe opened successfully
			{
				//Create packet to send
				m_pServerPacket->packetType = PT_CREATE;
				std::string strTextToSend = m_strServerName + ":" + m_strUserName;
				AddTextToServerDataPacket(strTextToSend);

				//Send creation packet to server
				m_pClient->SendData(m_pServerPacket);
			}
		}
	}
	default:
		break;
	}

	

}

void CClientApp::LobbyMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	//***Options MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MAIN_MENU);
	iYPos += 200;
	
	RenderText("Multi Player Lobby", iYPos, TEXT_MAIN_MENU);

	//TO DO: get list of users and print them
	
}

void CClientApp::OptionsMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	//***Options MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strOptionsMenu.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strOptionsMenu[i], iYPos, TEXT_MENU_SELECT);
	}
}

void CClientApp::InstructionMenuDraw()
{
	int iYPos = (m_iScreenHeight / 8);
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE);

	//***Options MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU_SELECT);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strInstructions.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strInstructions[i], iYPos, TEXT_MENU_SELECT);
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
	case TEXT_LIST:
	{
		TextColor = D3DCOLOR_XRGB(0, 0, 255);

	}break;

	case TEXT_MENU_SELECT:
	case TEXT_LIST_SELECT:
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
				//Set menu clicked to true
				m_bMenuClicked = true;
				//Save which menu has been clicked
				m_strClickedMenu = _strText;
				//Set Left mouse button to false
				m_bIsClicked[MK_LBUTTON] = false;
			}
		}

	}break;

	}

	//Render the title 
	m_pRenderManager->RenderText(_strText, Rect, TextColor, _TextType);

}

void CClientApp::AddTextToServerDataPacket(std::string _srtText)
{
	//convert and add the string to the ServerDataPacket
	if (_srtText.length() < NetworkValues::MAX_CHAR_LENGTH)
	{
		strcpy_s(m_pServerPacket->cText, _srtText.c_str());
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

		switch (m_pClientPacket->packetType)
		{
		case PT_CREATE:
		{
			ProcessCreation();
		}
		break;
		default:
			break;
		}

	}
}

void CClientApp::ProcessCreation()
{
	//The only way this assert will trigger is if the client user name already exist in the server,
	//which is not possible, therefor should never trigger
	// TO DO: PP: Assert
	assert(m_pClientPacket->bSuccess);
	//Server created set menu state to lobby
	m_eMenuState = MS_LOBBY;
	//Return host state to default
	m_eHostState = HS_DEFAULT;
	
}