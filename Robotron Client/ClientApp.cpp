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
	//Frame limiter
	m_iFrameTimeStart = 0;
	m_iFrameTimeEnd = 0;
	m_iFrameTimeDifference = 0;

	m_iSecondCounter = 0;
	m_iFrameCounter = 0;
	m_iFPS = 0;
}

CClientApp::~CClientApp(void)
{
	//Game
	
	
	m_pGame->DestroyInstance();

	//delete m_pGame;
	//m_pGame = 0;

	delete m_pInputManager;
	m_pInputManager = 0;

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
	delete m_pMapActiveServers;
	m_pMapActiveServers = 0;
	delete m_pMapActiveClients;
	m_pMapActiveClients = 0;
	delete m_pSelectedServer;
	m_pSelectedServer = 0;


	//Graphics
	
	m_pRenderManager->Shutdown();
	delete m_pRenderManager;
	m_pRenderManager = 0;
	

}

CClientApp& CClientApp::GetInstance()
{
	//Create the Client instance if it doesnt exist 
	if (s_pClientApp == 0)
	{
		s_pClientApp = new CClientApp();
	}
	return (*s_pClientApp);
}

void CClientApp::PreQuit()
{
	if (s_pClientApp != 0)
	{
		//Send Quit message
		SetClientInfo();

		if (m_bIsHost)
		{
			m_pServerPacket->packetType = PT_QUIT;
		}
		else
		{
			m_pServerPacket->packetType = PT_LEAVE;
		}

		m_pClient->SendData(m_pServerPacket);
		
	}
}

void CClientApp::DestroyInstance()
{	
	delete s_pClientApp;
	s_pClientApp = 0;
}

bool CClientApp::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	//Initialise Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	//Initialise Game Variables
	m_bIsHost = false;
	m_bServerCreated = false;
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
	
	m_strOptionsMenu.push_back("Game options");
	
	m_strInstructions.push_back("Game Instructions");
	
	m_strExitOptions.push_back("Yes - Close The Game");
	m_strExitOptions.push_back("No - Take Me Back");
	
	m_eGameState = GS_MENU;
	m_eHostState = HS_DEFAULT;
	m_eMenuState = MS_MAIN;

	//Initialize the key down boolean array
	m_bIsKeyDown = new bool[255];
	//Set all key downs to false
	memset(m_bIsKeyDown, false, 255);
	
	//Initialise Networking variables
	m_strServerName = "";
	m_strUserName = "";
	m_strErrorReason = "";
	m_bJoinError = false;
	m_bClientActive = false;
	
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());
	m_pServerPacket = new ServerDataPacket;
	m_pClientPacket = new ClientDataPacket;
	m_pClientDataQueue = new std::queue < ClientDataPacket >;
	m_pMapActiveServers = new std::multimap< std::string, ServerInfo>;
	m_pMapActiveClients = new std::map< std::string, ClientInfo>;
	m_pSelectedServer = new std::pair < std::string, ServerInfo > ;
	//Create and run separate thread to constantly receive data
	m_RecieveThread = std::thread(&CClientApp::ReceiveDataThread, (this));
	
	//Initialise Graphic variables
	m_pRenderManager = new CD3D9Renderer();
	//TO DO: Full screen???
	bool bFullScreen = false;
	VALIDATE(m_pRenderManager->Initialise(_iScreenWidth, _iScreenHeight, _hWnd, bFullScreen));

	

	//Initialise the the input manager
	m_pInputManager = new CInputManager();
	VALIDATE(m_pInputManager->Initialise(_hInstance, m_hWnd, m_iScreenWidth, m_iScreenHeight));


	m_iLightningTimer = 0;

	return true;

}

//Process based on states
void CClientApp::Process()
{
	
	//Reset the player inputs
	m_pInputManager->ResetInputStates();
	//Process the Player the inputs
	m_pInputManager->ProcessInput();
	
	switch (m_eGameState)
	{
	case GS_MENU:
	{
		//Menu selected on mouse click down
		//Process the selected menu item on mouse click up
		if (m_strClickedMenu != "")
		{
			int c = 9;
		}
		if (m_pInputManager->GetInputStates().bActivate == false) 
		{
			ProcessMenuSelection(m_strClickedMenu);
			m_strClickedMenu = "";
		}


		switch (m_eMenuState)
		{
		case MS_MAIN:
		{
			//Reset Data
			ResetData();
		}
		break;
		case MS_MULTI_PLAYER:
		{
			//Reset Data
			ResetData();
		}
		case MS_JOIN_GAME:
		{
			switch (m_eHostState)
			{
			case HS_SERVER_NAME:
			{
				//If m_strActiveServers didnt need to be processed remove them
				if (m_bMenuClicked == false)
				{
					//Clear server list
					m_pMapActiveServers->clear();
				}

				//Broadcast to find servers
				FindServers();
			}
				break;
			default:
				break;
			}
		}
			break;
		case MS_HOST_GAME:
		{
			switch (m_eHostState)
			{
			case HS_DONE:
			{
				//open server app
				OpenServerApp();
			}
				break;
			default:
				break;
			}
		}
			break;
		case MS_LOBBY:
		{
			RequestUserList();
		}
			break;
		default:
			break;
		}

	}
		break;
	case GS_PLAY:
	{
		ProcessGameInput();
		ProcessLightning();	
	}
		break;
	default:
		break;
	}

	//process received data
	ProcessReceiveData();
	
}

void CClientApp::ProcessLightning()
{
	//Check to see if its time to flash lightning, approx every 4 seconds
	if (m_iLightningTimer > 4)
	{
		//Flash lighting 2 in a seconds for 20 frames each
		if ((m_iFrameCounter < 10) && (m_iFrameCounter > 1))
		{
			m_pGame->SetLightning(true);
		}
		else if ((m_iFrameCounter > 10) && (m_iFrameCounter < 20))
		{
			m_pGame->SetLightning(false);
		}
		else if ((m_iFrameCounter > 20) && (m_iFrameCounter < 40))
		{
			m_pGame->SetLightning(true);
		}
		else if ((m_iFrameCounter > 40) && (m_iFrameCounter < 60))
		{
			m_pGame->SetLightning(false);

			//Reset the Lightning timer
			m_iLightningTimer = 0;
		}

	}
}

void CClientApp::ProcessGameInput()
{
	//Set server info
	SetClientInfo();
	//Set up message
	m_pServerPacket->packetType = PT_INPUT;

	//get the player input states
	m_pServerPacket->PlayerInputs = m_pInputManager->GetInputStates();

	//Send the inputs
	m_pClient->SendData(m_pServerPacket);

}

void CClientApp::ProcessTextInputs(int _iInput)
{
	switch (m_eGameState)
	{
		case GS_MENU:
		{
			switch (m_eMenuState)
			{
				case MS_SINGLE_PLAYER:
				{
					ProcessSinglePlayer(_iInput);
				}
					break;
				case MS_JOIN_GAME:
				//fall though
				case MS_HOST_GAME:
				{
					ProcessHostGame(_iInput);
				}
				break;
				default:
					break;
			}
		}
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
		SinglePlayerSelect(_strMenuItem);
		
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

	case MS_LOBBY:
	{
		LobbyMenuSelect(_strMenuItem);
	}

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

void CClientApp::ProcessSinglePlayer(int _iInput)
{
	if (_iInput == 13)	//Enter Key
	{
		//Send Creation
		m_strServerName = "Single";
		
		//Open the Server
		OpenServerApp();
	}
	else
	{
		//Continue processing text input
		ConvertTextInput(&m_strUserName, _iInput);
	}

}

void CClientApp::ProcessHostGame(int _iInput)
{
	switch (m_eHostState)
	{
		case HS_DEFAULT:
			break;
		case HS_SERVER_NAME:
		{
			//Only reprocess inputs if you are hosting a game	
			if (m_eMenuState == MS_HOST_GAME )
			{
				if (_iInput == 13)	//Enter Key
				{
					//Go to input user name
					m_eHostState = HS_USER_NAME;
				}
				else
				{
					//Continue processing text input
					ConvertTextInput(&m_strServerName, _iInput);
				}
			}
			
		}
			break;
		case HS_USER_NAME:
		{
			if (_iInput == 13) //Enter Key
			{
				if (m_eMenuState == MS_HOST_GAME)
				{
					//Ready to host server
					m_eHostState = HS_DONE;
					m_bJoinError = false;
				}
				else if (m_eMenuState == MS_JOIN_GAME)
				{
					//Request to join selected server
					//Initialize request
					m_pServerPacket->packetType = PT_JOIN_REQUEST;
					//Add Client info to server packet
					SetClientInfo();
					//Send message 
					m_pClient->SendData(m_pServerPacket);
					
				}
			}
			else
			{
				//Continue processing text input
				ConvertTextInput(&m_strUserName, _iInput);
			}
		}
			break;
		case HS_DONE:
			break;
		default:
			break;
	}
}

//Process menu selection inputs
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
		m_bsinglePlayer = true;

	}break;

	case 1: //Multi player
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MULTI_PLAYER;
		m_bsinglePlayer = false;

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

void CClientApp::SinglePlayerSelect(std::string _strMenuItem)
{
	
	//Back button was clicked
	if (_strMenuItem == "\t Back")
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
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

	//Back button was clicked
	if (_strMenuItem == "\t Back")
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
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
		if (m_eMenuState == MS_JOIN_GAME)
		{
			//Check if it was a server name that was selected
			std::map< std::string, ServerInfo>::iterator iterServer = m_pMapActiveServers->begin();
			std::map< std::string, ServerInfo>::iterator iterServertEnd = m_pMapActiveServers->end();
			while (iterServer != iterServertEnd)
			{
				if (_strMenuItem == "\t"+iterServer->first) //Yes a server name was selected
				{			
					//Check if server is full
					if (m_pClientPacket->serverInfo.iNumClients < NetworkValues::MAX_USERS)
					{
						//Go to User name input
						*m_pSelectedServer = *iterServer;
						//Set the server socket address to the selected socket address
						m_pClient->SetServerSocketAddress(m_pSelectedServer->second.serverSocAddr);
						m_eHostState = HS_USER_NAME;
					}
				}
				iterServer++;
			}
		}
		if (_strMenuItem == "\t Back")
		{
			m_pRenderManager->Clear(true, true, false);
			m_eMenuState = MS_MULTI_PLAYER;
			//Reset Data
			ResetData();
		}
	}
		break;
	case HS_USER_NAME:
	{
		
		if (_strMenuItem == "\t Back")
		{
			m_pRenderManager->Clear(true, true, false);
			m_eHostState = HS_SERVER_NAME;
		}
	}
		break;
	case HS_DONE:
	{

	}
		break;
	default:
		break;
	}
}

void CClientApp::LobbyMenuSelect(std::string _strMenuItem)
{
	if (_strMenuItem == "\t Back")
	{
		//Send message to server that I have left
		//Add Client info to server packet
		SetClientInfo();
		//other Aspects of message to send
		m_pServerPacket->packetType = PT_LEAVE;
		//Send message
		m_pClient->SendData(m_pServerPacket);
		
		m_pRenderManager->Clear(true, true, false);
		//Reset Data
		ResetData();
		m_eMenuState = MS_HOST_GAME;
		m_eHostState = HS_USER_NAME;
	}

	if (_strMenuItem == "\t Ready")
	{
		//Send Request to server to update status to active
		
		//Add Client info to server packet
		SetClientInfo();
		//other Aspects of message to send
		m_pServerPacket->packetType = PT_ACTIVE;
		m_pServerPacket->bSuccess = true;
		//Send message
		m_pClient->SendData(m_pServerPacket);
	}

	if (_strMenuItem == "\t Cancel")
	{
		//Send Request to server to update status to active

		//Add Client info to server packet
		SetClientInfo();
		//other Aspects of message to send
		m_pServerPacket->packetType = PT_ACTIVE;
		m_pServerPacket->bSuccess = false;
		//Send message
		m_pClient->SendData(m_pServerPacket);
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

	//Back button was clicked
	if (_strMenuItem == "\t Back")
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0:
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

	//Back button was clicked
	if (_strMenuItem == "\t Back")
	{
		m_pRenderManager->Clear(true, true, false);
		m_eMenuState = MS_MAIN;
	}

	//Switch on which text rect was clicked on, 
	//Then clear the screen and set the game state
	switch (iMenuItem)
	{
	case 0:
		break;
	default:
		break;
	}
}

void CClientApp::ExitMenuSelect(std::string _strMenuItem)
{
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


//Text based inputs
void CClientApp::ConvertTextInput(std::string* _strText, int _iInput)
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

		//If user name was changed - set join error to false
		m_bJoinError = false;
	}
	else if ((_iInput >= 65 && _iInput <= 90) ||
		(_iInput >= 97 && _iInput <= 122))
	{
		//Only add to the string if its within length limits
		if (strTemp.length() < NetworkValues::MAX_NAME_LENGTH)
		{
			//char has been pressed add it to the string
			strTemp += cUserInput;

			//If user name was changed - set join error to false
			m_bJoinError = false;
		}
	}

	*_strText = strTemp;
}

void CClientApp::EnterUserName(int _iYPos)
{
	//***Host MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	_iYPos += 250;
	DWORD dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText("Enter User Name: ", _iYPos, TEXT_MENU, false, dwTextFormat);

	//render user name
	_iYPos += (uiFontHeight + 150);
	RenderText(m_strUserName, _iYPos, TEXT_MENU, false, dwTextFormat);

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	_iYPos = m_iScreenHeight - (2 * uiFontHeight);
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t Back", _iYPos, TEXT_MENU, true, dwTextFormat);
}

void CClientApp::RenderText(std::string _strText, int _iYPos, eTextType _TextType, bool _bSelectable, DWORD _format)
{
	//Create the text space as a RECT
	RECT Rect;
	Rect.left = 10;
	Rect.right = m_iScreenWidth - 10;
	Rect.top = _iYPos;
	int uiFontHeight = m_pRenderManager->GetFontHeight(_TextType);
	Rect.bottom = (Rect.top + uiFontHeight);

	//Change the color if the mouse hovers over a main menu rect
	DWORD TextColor = D3DCOLOR_XRGB(0, 0, 0);
	switch (_TextType)
	{
	case TEXT_TITLE:
	{
		TextColor = D3DCOLOR_XRGB(155, 0, 0);

	}break;
	case TEXT_MENU:
	{
		TextColor = D3DCOLOR_XRGB(100, 0, 0);
	}break;
	case TEXT_LIST:
	{
		TextColor = D3DCOLOR_XRGB(255, 0, 0);

	}break;
	}

	if (_bSelectable)
	{
		TextColor = D3DCOLOR_XRGB(255, 50, 0);

		//If you hover over a click-able text
		if (m_pInputManager->GetInputStates().CursorPos.y >= Rect.top	&&
			m_pInputManager->GetInputStates().CursorPos.y <= Rect.bottom &&
			m_pInputManager->GetInputStates().CursorPos.x <= Rect.right	&&
			m_pInputManager->GetInputStates().CursorPos.x >= Rect.left
			)

		{
			//Change its color signifying that its click-able
			TextColor = D3DCOLOR_XRGB(255, 185, 0);
			//If the text was clicked
			if (m_pInputManager->GetInputStates().bActivate)
			{
				//Set menu clicked to true
				m_bMenuClicked = true;
				//Save which menu has been clicked
				m_strClickedMenu = _strText;
			}
		}
	}

	//Render the title 
	m_pRenderManager->RenderText(_strText, Rect, TextColor, _TextType, _format);

}


//Draw Dependant on state
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
			SinglePlayerMenuDraw();
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
	{
		m_pGame->Draw();
	}
		break;
	
	}



	m_pRenderManager->EndRender();
}

void CClientApp::MainMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_TOP | DT_SINGLELINE ;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	////***MAIN MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 220 - (uiFontHeight + 10);
	for (unsigned int i = 0; i < m_strMainMenuOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 40);
		RenderText(m_strMainMenuOptions[i], iYPos, TEXT_MENU, true, dwTextFormat);
	}

}

void CClientApp::SinglePlayerMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	//User name
	EnterUserName(iYPos);

}

void CClientApp::MultiPlayerMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	//***Multiplayer MENU***
	//get font height 
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 300 - (uiFontHeight + 50);
	for (unsigned int i = 0; i < m_strMultiPlayerOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 50);
		RenderText(m_strMultiPlayerOptions[i], iYPos, TEXT_MENU, true, dwTextFormat);
	}

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos = m_iScreenHeight - (2*uiFontHeight);
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
}

void CClientApp::JoinMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	switch (m_eHostState)
	{
	case HS_DEFAULT:
	{
		m_eHostState = HS_SERVER_NAME;
	}
		break;
	case HS_SERVER_NAME:
	{
		int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos += 200;
		RenderText("Select Server to join: ", iYPos, TEXT_MENU, false, dwTextFormat);
		iYPos += (uiFontHeight + 1);

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
		if (m_pMapActiveServers->size() < 1)	 //No servers found
		{
			//Print no servers found
			iYPos += (uiFontHeight + 1);
			RenderText("No Active Servers Found!", iYPos, TEXT_LIST, false, dwTextFormat);
		}
		else //Servers found
		{

			//Server Headings
			iYPos += (uiFontHeight + 1);
			//Render server name
			dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
			RenderText("\tServer Name", iYPos, TEXT_LIST, false, dwTextFormat);
			//Render host of server
			dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
			RenderText("\tServer Host", iYPos, TEXT_LIST, false, dwTextFormat);
			//Render Number of clients in server
			dwTextFormat = DT_RIGHT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
			RenderText("Player Count", iYPos, TEXT_LIST, false, dwTextFormat);



			//Print the list of found servers
			uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
			std::map< std::string, ServerInfo >::iterator iterServer = m_pMapActiveServers->begin();
			std::map< std::string, ServerInfo >::iterator iterServertEnd = m_pMapActiveServers->end();
			while (iterServer != iterServertEnd)
			{
				iYPos += (uiFontHeight + 1);
				//Calculate server Fullness
				std::string strNumClients = std::to_string(iterServer->second.iNumClients);
				std::string strMaxClients = std::to_string(NetworkValues::MAX_USERS);
				std::string strFullness;
				bool bSelectable = false;
				if (iterServer->second.iNumClients >= NetworkValues::MAX_USERS) //Server is full
				{
					strFullness = "FULL";
					//Server full therefor not selectable
					bSelectable = false;
				}
				else
				{
					strFullness = strNumClients + "/" + strMaxClients;
					//Server not full therefor selectable
					bSelectable = true;
				}
				
				
				//Render host of server
				dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
				std::string strHostName(iterServer->second.cHostName);
				RenderText(strHostName, iYPos, TEXT_LIST, bSelectable, dwTextFormat);
				//Render Number of clients in server
				dwTextFormat = DT_RIGHT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
				//Render Server fullness
				RenderText(strFullness, iYPos, TEXT_LIST, bSelectable, dwTextFormat);
				
				//Render server name
				dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
				RenderText("\t" + iterServer->first, iYPos, TEXT_LIST, bSelectable, dwTextFormat);

				//Go to next one in the list
				iterServer++;
			}

		}
				
		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos = m_iScreenHeight - (2*uiFontHeight);
		dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
		RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
				
	}
		break;
	case HS_USER_NAME:
	{
		//***Host MENU***
		int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos += 200; 
		dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
		RenderText("Enter User Name: ", iYPos, TEXT_MENU, false, dwTextFormat);
		//Render the user name
		iYPos += (uiFontHeight + 100);
		RenderText(m_strUserName, iYPos, TEXT_MENU, false, dwTextFormat);
		
		//Render User name is in Use
		if (m_bJoinError)
		{
			uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
			iYPos += (uiFontHeight + 1);
			dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
			RenderText(m_strErrorReason, iYPos, TEXT_LIST, false, dwTextFormat);
		}

		//Render back button
		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos = m_iScreenHeight - (2*uiFontHeight);
		dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
		RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
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

	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

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
		int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos += 250; 
		dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
		RenderText("Enter Server Name: ", iYPos, TEXT_MENU, false, dwTextFormat);

		//Render the server name
		iYPos += (uiFontHeight + 150);
		RenderText(m_strServerName, iYPos, TEXT_MENU, false, dwTextFormat);

		uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos = m_iScreenHeight - (2 * uiFontHeight);
		dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
		RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
	}
		break;
	case HS_USER_NAME:
	{
		EnterUserName(iYPos);
	}
		break;
	case HS_DONE:
	{
		
	}
	default:
		break;
	}

	

}

void CClientApp::LobbyMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	//***Lobby MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 200;
	
	RenderText("Multi-player Lobby", iYPos, TEXT_MENU, false, dwTextFormat);
	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += uiFontHeight;
	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
	iYPos += uiFontHeight;
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\tPlayers", iYPos, TEXT_LIST, false, dwTextFormat);
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("Ready", iYPos, TEXT_LIST, false, dwTextFormat);

	std::string ReadyButton;

	//Get list of users
	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_LIST);
	iYPos += uiFontHeight;
	for (int i = 0; i < NetworkValues::MAX_USERS; i++)
	{
		std::string strUserName(m_pClientPacket->serverInfo.activeClientList[i].cUserName);

		if (strUserName != "")
		{
			iYPos += uiFontHeight + 1;
			//Render client names
			dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
			RenderText("\t"+strUserName, iYPos, TEXT_LIST, false, dwTextFormat);
			//Render if they are ready or not
			std::string strReady;
			if (m_pClientPacket->serverInfo.activeClientList[i].bActive)
			{
				strReady = "[x]";
			}
			else
			{
				strReady = "[  ]";
			}
			dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
			RenderText(strReady, iYPos, TEXT_LIST, false, dwTextFormat);
		}

		//Checking the active clients state to set the ready button
		if (strUserName == m_strUserName)
		{
			if (m_pClientPacket->serverInfo.activeClientList[i].bActive)
			{
				ReadyButton = "Cancel";
			}
			else
			{
				ReadyButton = "Ready";
			}
		}
	}

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos = m_iScreenHeight - (3*(uiFontHeight + 1));
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t "+ReadyButton, iYPos, TEXT_MENU, true, dwTextFormat);

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos = m_iScreenHeight - (2*uiFontHeight);
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
	
}

void CClientApp::OptionsMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);


	//***Options MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strOptionsMenu.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strOptionsMenu[i], iYPos, TEXT_MENU, true, dwTextFormat);
	}

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos = m_iScreenHeight - (2 * uiFontHeight);
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
}

void CClientApp::InstructionMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	//***Options MENU***
	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 200 - (uiFontHeight + 1);
	for (unsigned int i = 0; i < m_strInstructions.size(); i++)
	{
		iYPos += (uiFontHeight + 1);
		RenderText(m_strInstructions[i], iYPos, TEXT_MENU, true, dwTextFormat);
	}

	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos = m_iScreenHeight - (2 * uiFontHeight);
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE | DT_EXPANDTABS;
	RenderText("\t Back", iYPos, TEXT_MENU, true, dwTextFormat);
}

void CClientApp::ExitMenuDraw()
{
	int iYPos = (m_iScreenHeight / 16);
	DWORD dwTextFormat;
	m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

	//***TITLE***
	dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
	RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

	int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 200;
	RenderText("Are You Sure You Want To Exit?", iYPos, TEXT_MENU, false, dwTextFormat);
	

	////***EXIT MENU***
	uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
	iYPos += 180 - (uiFontHeight + 100);
	for (unsigned int i = 0; i < m_strExitOptions.size(); i++)
	{
		iYPos += (uiFontHeight + 100);
		RenderText(m_strExitOptions[i], iYPos, TEXT_MENU, true, dwTextFormat);
	}

}


//Render Frame
bool CClientApp::RenderSingleFrame()
{
	//Start time
	m_iFrameTimeStart = (int)timeGetTime();
	//Use some time to ensure Delta tick not 0.0f
	Sleep(1);

	if (m_iFrameCounter == 1)
	{
		m_iLightningTimer++;
	}
	
	//Is the client still active? if not return false
	if (m_pClient->GetActive() == false)
	{
		return false;
	}
	

	Process();
	Draw();

	//Limit frames to roughly 60 frames per second
	LimitFrame();
		
	return true;
		
}

void CClientApp::LimitFrame()
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

//Process received data
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
		case PT_DEFAULT:
		{
			
		}
			break;
		case PT_CREATE:
		{
			if (m_bServerCreated == false)
			{
				//Initial message received from the server (The broadcast message to find its host)
				ProcessCreation();
			}
		}
		break;
		case PT_FIND:
		{
			ProcessServerFind();
		}
		break;
		case PT_JOIN_REQUEST:
		{
			ProcessJoinRequest();
		}
		break;
		case PT_CLIENT_JOINED:
		{
			std::string JoinedUser(m_pClientPacket->cText);
			//Check that the joined user is not yourself
			for (unsigned int iJoinedPLayer = 0; iJoinedPLayer < NetworkValues::MAX_USERS; iJoinedPLayer++)
			{
				//Find the joined users Client info
				std::string strUserCheck(m_pClientPacket->serverInfo.activeClientList[iJoinedPLayer].cUserName);
				if (strUserCheck == JoinedUser)
				{
					//Then add this user the the list of clients
					bool addedUser = AddUser(JoinedUser, m_pClientPacket->serverInfo.activeClientList[iJoinedPLayer]);
					assert(("failed to add user to client app map", addedUser));

				}
			}
	
		}
		break;
		case PT_GAME_START:
		{
			m_eGameState = GS_PLAY;
		}
		break;
		case PT_LEAVE:
		{
			std::string strUserName(m_pClientPacket->cText);
			//So and So has left
			//Remove them from the game
			m_pGame->RemovePlayer(strUserName);
			m_pMapActiveClients->erase(strUserName);

		
		}
			break;
		case PT_QUIT:
		{
			//Server has quit 
			//Reset states
			ResetData();
			m_eGameState = GS_MENU;
			m_eMenuState = MS_MAIN;
		}
		break;
		case PT_UPDATE:
		{
			m_pGame->Process(m_pClientPacket);
		}
		break;
		case PT_CREATE_ENEMY:
		{
			m_pGame->CreateEnemy(m_pClientPacket);
		}
		break;
		case PT_CREATE_POWERUP:
		{
			m_pGame->CreatePowerUp(m_pClientPacket);
		}
		break;
		case PT_CREATE_PROJECTILE:
		{
			m_pGame->CreateProjectile(m_pClientPacket);
		}
		break;
		case PT_DELETE_ENEMY:
		{
			m_pGame->DeleteEnemy(m_pClientPacket);
		}
		break;
		case PT_DELETE_POWERUP:
		{
			m_pGame->DeletePowerUp(m_pClientPacket);
		}
		break;
		case PT_DELETE_PROJECTILE:
		{
			m_pGame->DeleteProjectile(m_pClientPacket);
		}
		break;
		default:
			break;
		}

	}
}

void CClientApp::ProcessCreation()
{
	//Convert the text in received packet to a string to be manipulated
	std::string strCreation(m_pClientPacket->cText);

	//Separate the user and server name
	std::size_t found = strCreation.find(":");
	std::string strUserName = strCreation.substr(found + 1);
	std::string strServerName = strCreation.substr(0, found);

	//Check the received message
	if ((strUserName == m_strUserName) && 
		(strServerName == m_strServerName))
	{
		//If the received packet has the same values that was sent 
		//when the server was run, then send back to that server that 
		//This client is its host
				
		//Set the server socket address to the server that replied correctly
		m_pClient->SetServerSocketAddress(m_pClientPacket->serverInfo.serverSocAddr);
		
		//Add Client info to server packet
		SetClientInfo();
		//Set up the message
		m_pServerPacket->packetType = PT_CREATE;
		//send whether in single player mode or Multiplayer mode
		m_pServerPacket->bSuccess = m_bsinglePlayer;
		std::string strTextToSend = "<KW>HOST";
		StringToStruct(strTextToSend.c_str(), NetworkValues::MAX_CHAR_LENGTH, m_pServerPacket->cText);
		//Send message 
		m_pClient->SendData(m_pServerPacket);

		//Set that the server has been created
		m_bServerCreated = true;
				
		//Load the Game
		LoadGame();
	

		//Set the menu state to lobby
		m_eMenuState = MS_LOBBY;
		
		//If single player active
		if (m_bsinglePlayer)
		{
			//Add Client info to server packet
			SetClientInfo();
			//other Aspects of message to send
			m_pServerPacket->packetType = PT_ACTIVE;
			m_pServerPacket->bSuccess = true;
			//Send message
			m_pClient->SendData(m_pServerPacket);
		}

	}

}

void CClientApp::ProcessServerFind()
{
	//Add server to multimap of servers 
	AddServer(m_pClientPacket->serverInfo.cServerName, m_pClientPacket->serverInfo);
}

void CClientApp::ProcessJoinRequest()
{
	//Check if Successful
	if (m_pClientPacket->bSuccess)
	{
		//We are done with connecting
		//Set the menu state to lobby
		m_eMenuState = MS_LOBBY;
		m_bJoinError = false;

		//Load the Game
		LoadGame();
				
	}
	else //Not Successful, find out why it is not
	{
		//Set Join Error to true
		m_bJoinError = true;

		//Convert the text in received packet to a string to be manipulated
		std::string strCreation(m_pClientPacket->cText);
		
		if (strCreation == "<KW>INVALID_USERNAME")
		{
			//User name already in use, try again
			//Send back to user name
			m_strErrorReason = "Username already in Use";
		}
	}
}

void CClientApp::FindServers()
{
	//BroadCast to find if any server replies
	m_pServerPacket->packetType = PT_FIND;
	//Add Client info to server packet
	SetClientInfo();
	//Broad Cast message
	m_pClient->Broadcast(m_pServerPacket);

	//TO DO: REMOVE AFTER ADDING FRAME LIMITER
	//Sleep(100);
	
}

bool CClientApp::AddServer(std::string _ServerName, ServerInfo _serverInfo)
{
	//Add passed in params to map of servers
	m_pMapActiveServers->insert(std::pair<std::string, ServerInfo >(_ServerName, _serverInfo));
	return true;
}

void CClientApp::RequestUserList()
{
	//Create a default message to send to server
	m_pServerPacket->packetType = PT_DEFAULT;
	
	//Add Client info to server packet
	SetClientInfo();
	//Send message 
	m_pClient->SendData(m_pServerPacket);
}


//Miscellaneous functions
void CClientApp::SetClientInfo()
{
	/*<CLIENT_INFO>*/
	//TO DO ZeroMemory
	//User Name
	StringToStruct(m_strUserName.c_str(), NetworkValues::MAX_NAME_LENGTH, m_pServerPacket->clientInfo.cUserName);
	//Client Socket address
	m_pServerPacket->clientInfo.clientSocAddr = m_pClient->GetClientSocketAddress();
	//Server Socket Address
	m_pServerPacket->clientInfo.ServerSocAddr = m_pClient->GetServerSocketAddress();
	//Activeness 
	m_pServerPacket->clientInfo.bActive = m_bClientActive;
}

void CClientApp::OpenServerApp()
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
		std::string strTextToSend = m_strServerName + " " + m_strUserName;
		int iError = (int)ShellExecuteA(m_hWnd, "open", filename.c_str(), strTextToSend.c_str(), NULL, SW_MINIMIZE);

		//Robotron Server.exe was unable to open for some reason.
		//Reason as to why it didnt open look up the value of iError
		assert(("Unable to open server", iError > 32)); //Unable to open server
	}
}

void CClientApp::LoadGame()
{
	m_eMenuState = MS_LOADING;
	m_bGameLoading = true;
	//Start the loading screen
	std::thread LoadingTread = std::thread(&CClientApp::LoadingScreen, (this));

	m_pGame = &(CGame::GetInstance());
	m_pGame->Initialise(m_pRenderManager, m_strUserName);
	m_pGame->AddAllPlayers(m_pClientPacket);

	m_bGameLoading = false;

	LoadingTread.join();

	m_eMenuState = MS_LOBBY;	
}

void CClientApp::LoadingScreen()
{
	//TO DO: comment
	std::string strLoading = "LOADING";
	std::string strLoadingDots = "";
	int iNumDots = 0;
	while (m_bGameLoading)
	{
		m_pRenderManager->StartRender(true, true, false);

		int iYPos = (m_iScreenHeight / 16);
		DWORD dwTextFormat;
		m_pRenderManager->SetBackgroundColor(D3DCOLOR_XRGB(0, 0, 0));

		//***TITLE***
		dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
		RenderText(m_strGameTitle, iYPos, TEXT_TITLE, false, dwTextFormat);

		iYPos += 400;
		for (int i = 0; i < iNumDots; i++)
		{
			strLoadingDots += ".";
		}
		
		dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
		RenderText(strLoading, iYPos, TEXT_MENU, false, dwTextFormat);

		int uiFontHeight = m_pRenderManager->GetFontHeight(TEXT_MENU);
		iYPos += (uiFontHeight + 1);
		dwTextFormat = DT_CENTER | DT_BOTTOM | DT_SINGLELINE;
		RenderText(strLoadingDots, iYPos, TEXT_MENU, false, dwTextFormat);

		if (iNumDots > 6)
		{
			iNumDots = 0;
			strLoadingDots = "";
		}
		iNumDots++;

		m_pRenderManager->EndRender();

		Sleep(100);
	}
	
}

bool CClientApp::AddUser(std::string _strPlayerName, ClientInfo _clientInfo)
{
	std::pair<std::map<std::string, ClientInfo>::iterator, bool> MapClientIter;
	
	//Try add the user
	MapClientIter = m_pMapActiveClients->insert(std::pair<std::string, ClientInfo>(_strPlayerName, _clientInfo));

	//if the user is added to this map add it to the game as well
	if (MapClientIter.second == true)
	{
		m_pGame->AddPlayer(m_pClientPacket, _strPlayerName);
	}

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapClientIter.second;
}

void CClientApp::ResetData()
{
	m_bServerCreated = false;

	m_strServerName = "";
	m_strUserName = "";

	m_bIsHost = false;

	m_pClient->Reset();

	m_eHostState = HS_DEFAULT;

	m_pMapActiveClients->clear();

	if (m_pGame != 0)
	{
		m_pGame->DestroyInstance();
		m_pGame = 0;
	}
	
}