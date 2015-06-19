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
#include "../Shared/D3D9Renderer.h"
#include "InputManager.h"


class CClientApp
{
	//Member Function
public:
		
	/***********************
	* GetInstance: Returns a reference to the singleton ClientApp object, if one does not exist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a reference to the singleton game object
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

protected:

private:
	/***********************
	* CGame: Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp();

	/***********************
	* CGame: Copy Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp(const CClientApp& _kr);

	/***********************
	* operator=: Overloaded
	* @author: Jc Fowles
	* @return:
	********************/
	CClientApp& operator= (const CClientApp& _kr);

	void SinglePlayerSelect(std::string _strMenuItem);
	void EnterUserName(int _iYpos);


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
	
	eGameStates m_eGameState;
	eMenuStates m_eMenuState;
	eHostStates m_eHostState;
	std::string m_strGameTitle;
	std::vector<std::string> m_strMainMenuOptions;
	std::vector<std::string> m_strMultiPlayerOptions;
	std::vector<std::string> m_strOptionsMenu;
	std::vector<std::string> m_strInstructions;
	std::vector<std::string> m_strExitOptions;
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

};

#endif //__CLIENT_APP_H__
