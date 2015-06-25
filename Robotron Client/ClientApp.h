//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CClientApp.h
// Description : Header file containing declarations for CClientApp Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__

//library includes
#include <string>
#include <queue>
#include <thread>

//Local Includes
#include "Game.h"
#include "Client.h"
#include "D3D9Renderer.h"
#include "InputManager.h"
#include "DebugCamera.h"
#include "..//Shared//Clock.h"


class CClientApp
{
	//Member Function
public:
		
	/***********************
	* GetInstance: Returns a reference to the singleton ClientApp object, if one does not exist create one
	* @author: Jc Fowles
	* @return: CClientApp& : Returns a reference to the singleton ClientApp object
	********************/
	static CClientApp& GetInstance();
		
	/***********************
	* PreQuit: Before we destroy the client app send Leave or Quit messages to server
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void PreQuit();
	
	/***********************
	* DestroyInstance: Destroys the singleton ClientApp object
	* @author: Jc Fowles
	* @return: void
	********************/
	static void DestroyInstance();

	/***********************
	* ~CClientApp: Destructor of the ClientApp instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CClientApp(void);

	/***********************
	* Initialize: Initializes the ClientApp instance
	* @author: Jc Fowles
	* @parameter: _hWnd: Handle to the window
	* @parameter: _hInstance: Handle to the window instance
	* @parameter: _iScreenWidth: Width to the window
	* @parameter: _iScreenHeight: Height to the window
	* @return: bool: Successful Initialization
	********************/
	bool Initialise(HINSTANCE _hInstance ,HWND _hWnd, int _iScreenWidth, int _iScreenHeight);

	/***********************
	* Process: Process the the ClientApp instance
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();

	/***********************
	* ProcessTextInputs: Process User text inputs
	* @author: Jc Fowles
	* @Parameter: int _iInput: The ascii value of the input
	* @return: void: 
	********************/
	void ProcessTextInputs(int _iInput);

	/***********************
	* ProcessGameInput: Process User inputs for the game and send them to server
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessGameInput();
	
	/***********************
	* ConvertTextInput: convert passed in ascii input values into a standard sting 
	* @author: Jc Fowles
	* @Parameter: std::string * _pstrText: Standard string for ascii value to be added to 
	* @Parameter: int _iInput: The ascii value of the input
	* @return: void:
	********************/
	void ConvertTextInput(std::string* _pstrText, int _iInput);
	
	/***********************
	* ProcessHostGame: Process the Host game menu
	* @author: Jc Fowles
	* @Parameter: int _iInput: The ascii value of the user input
	* @return: void:
	********************/
	void ProcessHostGame(int _iInput);

	/***********************
	* ProcessSinglePlayer: Process the Single player menu
	* @author: Jc Fowles
	* @Parameter: int _iInput: The ascii value of the user input
	* @return: void:
	********************/
	void ProcessSinglePlayer(int _iInput);

	
	/***********************
	* PauseMenuSelect: Process the pause menu selection
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Which pause item was selected
	* @return: void: 
	********************/
	void PauseMenuSelect(std::string _strMenuItem);
		
	/***********************
	* GameOverSelect: Process the game over menu selection
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Which game over item was selected
	* @return: void: 
	********************/
	void GameOverSelect(std::string _strMenuItem);

	/***********************
	* ProcessMenuSelection: process the menu selection
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: which menu was seleceted
	* @return: void:
	********************/
	void ProcessMenuSelection(std::string _strMenuItem);
	
	/***********************
	* Draw: Draw the all the objects 
	* @author: Jc Fowles
	* @return: void
	********************/
	void Draw();
		
	
	/***********************
	* DrawPause: Draw the pause menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void DrawPause();

	/***********************
	* DrawPause: Draw the game over menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void DrawGameOver();
	
	/***********************
	* MainMenuDraw: Draw the main menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void MainMenuDraw();
	
	/***********************
	* SinglePlayerMenuDraw: Draw the Single player menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void SinglePlayerMenuDraw();
	
	/***********************
	* StartMenuDraw: Draw the Multi player menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void MultiPlayerMenuDraw();
		
	/***********************
	* JoinMenuDraw: Draw the join game menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void JoinMenuDraw();

	/***********************
	* HostMenuDraw: Draw the host menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void HostMenuDraw();
	
	/***********************
	* LobbyMenuDraw: Draw the Lobby menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void LobbyMenuDraw();

	/***********************
	* StartInstructionDraw: Draw the options menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void OptionsMenuDraw();

	/***********************
	* StartInstructionDraw: Draw the instructions menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void InstructionMenuDraw();

	/***********************
	* ExitMenuDraw: Draw the Exit menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ExitMenuDraw();

	/***********************
	* MainMenuSelect: Process the Selection in Multi player menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void:
	********************/
	void MainMenuSelect(std::string _strMenuItem);

	/***********************
	* MultiPlayerMenuSelect: Process the Selection in Multi player menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void:
	********************/
	void MultiPlayerMenuSelect(std::string _strMenuItem);

	/***********************
	* HostMenuSelect: Process the Selection in the Host menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void:
	********************/
	void HostMenuSelect(std::string _strMenuItem);

	/***********************
	* OptionsMenuSelect: Process the Selection in option menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void: 
	********************/
	void OptionsMenuSelect(std::string _strMenuItem);
		
	/***********************
	* InstructMenuSelect: Process the Selection in option menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void: 
	********************/
	void InstructMenuSelect(std::string _strMenuItem);

	/***********************
	* ExitMenuSelect: Process the Selection in option menu
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Selected menu item
	* @return: void: 
	********************/
	void ExitMenuSelect(std::string _strMenuItem);
	
	/***********************
	* OpenServerApp: Open and the the Server app executable
	* @author: Jc Fowles
	* @return: void:
	********************/
	void OpenServerApp();

	/***********************
	* RenderSingleFrame: Render a single frame to the screen
	* @author: Jc Fowles
	* @return: bool: return weather to continue rendering 
	********************/
	bool RenderSingleFrame();
		
	/***********************
	* RenderText: Render text to the screen
	* @author: Jc Fowles
	* @Parameter: std::string _strText: String of text to be rendered
	* @Parameter: int _iYPos: Y position of the text to be rendered
	* @Parameter: eTextType _TextType: The type of text to be rendered
	* @Parameter: bool _bSelectable: Weather the text should be select able
	* @Parameter: DWORD _format: Text format
	* @return: void:
	********************/
	void RenderText(std::string _strText, int _iYPos, eTextType _TextType, bool _bSelectable, DWORD _format);
		
	/***********************
	* ReceiveDataThread: Threaded function to receive data from the server and add them
						 to a work queue
	* @author: Jc Fowles
	* @return: void
	********************/
	void ReceiveDataThread();

	/***********************
	* ProcessReceiveData: Process the Received Data
	* @author: Jc Fowles
	* @return: void
	********************/
	void ProcessReceiveData();

	/***********************
	* ProcessCreation: Process the creation of the server
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessCreation();
		
	/***********************
	* ProcessSuccesfulHost: Process the successful client to server hosting
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessSuccesfulHost();
	
	/***********************
	* ProcessJoinRequest: Process the Join Request state received from server
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessJoinRequest();

	/***********************
	* ProcessJoinRequest: Process the found servers
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessServerFind();
	
	/***********************
	* LimitFrame: Limits the number of frames renders per second to roughly 60 frames
	* @author: Jc Fowles
	* @return: void
	********************/
	void LimitFrame();

	/***********************
	* IsKeyDown: Return the bool array of keyboard keys, used as a getter and setter
	* @author: Jc Fowles
	* @return: bool*: Bool array of keyboard keys
	********************/
	bool* IsKeyDown() const { return m_bIsKeyDown; }
	
	/***********************
	* GetisHost: Is this client hosting a server?
	* @author: Jc Fowles
	* @return: bool: Returns whether this client is hosting a server
	********************/
	bool GetisHost() const { return m_bIsHost; }

	/***********************
	* SetIsHost: Set whether this client is a host or not
	* @author: Jc Fowles
	* @Parameter: bool _bIsHost: bool value of whether this client is a host
	* @return: void:
	********************/
	void SetIsHost(bool _bIsHost) { m_bIsHost = _bIsHost; }
		
	/***********************
	* FindServers: Get the client to broadcast out and find all servers, then save them
	* @author: Jc Fowles
	* @return: void:
	********************/
	void FindServers();
	
	/***********************
	* AddServer: Add the passed in server to the list of active servers
	* @author: Jc Fowles
	* @Parameter: std::string _ServerName: Server name
	* @Parameter: ServerInfo _serverInfo: struct holding server info
	* @return: bool: Successful add
	********************/
	bool AddServer(std::string _ServerName, ServerInfo _serverInfo);

	/***********************
	* RequestUserList: Send user list request
	* @author: Jc Fowles
	* @return: void:
	********************/
	void RequestUserList();
		
	/***********************
	* SetClientInfo: Set the Client info of the packet before sending
	* @author: Jc Fowles
	* @return: void:
	********************/
	void SetClientInfo();
		
	/***********************
	* LobbySelect: TO DO:
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: TO DO:
	* @return: void: 
	********************/
	void LobbyMenuSelect(std::string _strMenuItem);

	/***********************
	* ProcessLightning: Process when to flash lighting
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessLightning();
		
	/***********************
	* SinglePlayerSelect: Process the single player options menu selection
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: Menu item that was selected
	* @return: void: 
	********************/
	void SinglePlayerSelect(std::string _strMenuItem);
		
	/***********************
	* DrawUserName: Renders the user name that the users enters as they enter it
	* @author: Jc Fowles
	* @Parameter: int _iYpos: screen y position to render the user name
	* @return: void: 
	********************/
	void DrawUserName(int _iYpos);
		
	/***********************
	* AddUser: Add a user to the list of clients 
	* @author: Jc Fowles
	* @Parameter: std::string _strPlayerName: The clients user name
	* @Parameter: ClientInfo _clientInfo: The clients information
	* @return: bool: successful add of user
	********************/
	bool AddUser(std::string _strPlayerName, ClientInfo _clientInfo);
		
	/***********************
	* ResetData: Reset the client app states 
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void ResetData();
		
	/***********************
	* LoadGame: Process loading up the games
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void LoadGame();
		
	/***********************
	* LoadingScreen: Render a active loading screen
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void LoadingScreen();

	/***********************
	* ProcessGamePlay: Process the game play
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessGamePlay();

	/***********************
	* ProcessDebugInput: Process the Debug input states
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessDebugInput();

protected:

private:
	/***********************
	* CClientApp: Constructor of the Client app instance
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp();

	/***********************
	* CClientApp: Copy Constructor of the Client app instance
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp(const CClientApp& _kr);

	/***********************
	* operator=: Overloaded assignment operator
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp& operator= (const CClientApp& _kr);
	

	//Member Variables
public:
	
protected:

private:
	//Window variables
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;
	

	//Game Variables
	static CClientApp* s_pClientApp;
	CGame* m_pGame;
	CClock* m_pClock;
	float m_fDeltaTick;
	eGameStates m_eGameState;
	eMenuStates m_eMenuState;
	eHostStates m_eHostState;
	std::string m_strGameTitle;
	std::vector<std::string> m_strMainMenuOptions;
	std::vector<std::string> m_strMultiPlayerOptions;
	std::vector<std::string> m_strOptionsMenu;
	std::vector<std::string> m_strInstructions;
	std::vector<std::string> m_strExitOptions;
	std::vector<std::string> m_strPauseOptions;
	std::vector<std::string> m_strGameOverOptions;
	bool m_bGameLoading;
	bool m_bsinglePlayer;
	bool m_bMenuClicked;
	std::string m_strClickedMenu;

	//Inputs
	CInputManager* m_pInputManager;
	bool* m_bIsKeyDown;
	
	
	//Networking variables
	CClient* m_pClient;
	bool m_bIsHost;
	bool m_bServerCreated;
	std::thread m_RecieveThread;
	ServerDataPacket* m_pServerPacket;
	ClientDataPacket* m_pClientPacket;
	std::queue<ClientDataPacket>* m_pClientDataQueue;
	static CMySemaphore s_Mutex;
	bool m_bJoinError;
	std::string m_strErrorReason;
	
	std::string m_strServerName;
	std::string m_strUserName;
	bool m_bClientActive;



	//Maps
	std::multimap< std::string, ServerInfo>* m_pMapActiveServers;
	
	std::map< std::string, ClientInfo>* m_pMapActiveClients;
	std::pair< std::string, ServerInfo>* m_pSelectedServer;
		
	//Graphic Variables
	IRenderer* m_pRenderManager;

	bool m_bCamToggle;
	bool m_bTab;
	bool m_bPauseToggle;
	bool m_bGamePause;
	bool m_bDebugToggle;
	bool m_bDebug;

	//Frame Limiter
	int m_iFrameTimeStart;
	int m_iFrameTimeEnd;
	int m_iFrameTimeDifference;

	int m_iSecondCounter;
	int m_iFrameCounter;
	int m_iFPS;

	int m_iLightningTimer;
	bool m_bGameOver;
};

#endif //__CLIENT_APP_H__
