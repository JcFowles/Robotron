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
#if !defined(__CLIENT_APP_H__)
#define __CLIENT_APP_H__

//library includes
#include <string>
#include <queue>
#include <thread>

//Local Includes
#include "Client.h"
#include "IRenderer.h"
#include "D3D9Renderer.h"
#include "../Shared/Clock.h"
#include "../Shared/App_Defines.h"


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
	* @parameter: _iScreenWidth: Width to the window
	* @parameter: _iScreenHeight: Height to the window
	* @return: bool: Successful Initialization
	********************/
	bool Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight);

	/***********************
	* Process: Process the the ClientApp instance
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();

	/***********************
	* ProcessInputs: Process User inputs
	* @author: Jc Fowles
	* @Parameter: int _iInput: The ascii value of the input
	* @return: void: 
	********************/
	void ProcessInputs(int _iInput);
	
	/***********************
	* ProcessTextInput: Process text with user input
	* @author: Jc Fowles
	* @Parameter: std::string * _pstrText: Pointer to string of text to be processed
	* @Parameter: int _iInput: The ascii value of the input
	* @return: void:
	********************/
	void ProcessTextInput(std::string* _pstrText, int _iInput);
	
	/***********************
	* ProcessHostGame: Process the Host game menu
	* @author: Jc Fowles
	* @Parameter: int _iInput: The ascii value of the user input
	* @return: void:
	********************/
	void ProcessHostGame(int _iInput);

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
	* @return: void: TO DO: Description
	********************/
	void MainMenuDraw();
	
	/***********************
	* StartMenuDraw: Draw the Multi player menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void StartMultiPlayerDraw();
	
	/***********************
	* StartInstructionDraw: Draw the options menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void StartOptionsDraw();

	/***********************
	* StartInstructionDraw: Draw the instructions menu
	* @author: Jc Fowles
	* @return: void:
	********************/
	void StartInstructionDraw();

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
	* HostMenuDraw: Process the Selection in option menu
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void HostMenuDraw();

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
	* @return: void:
	********************/
	void RenderText(std::string _strText, int _iYPos, eTextType _TextType);

	//TO DO: Comment header
	//void ProcessInputs(float _fDt);

	/***********************
	* ConvertToServerDataPacket: Convert passed in data to a ServerDataPacket
	* @author: Jc Fowles
	* @param: //TO DO
	* @return: void
	********************/
	void AddTextToServerDataPacket(std::string _srtData);

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
	* SetMousePos: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: POINT _MousePosition: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void SetMousePos(POINT _MousePosition){ m_MousePosition = _MousePosition; };
	
	/***********************
	* GetMousePos: TO DO: Description
	* @author: Jc Fowles
	* @return: POINT: TO DO: Description
	********************/
	POINT GetMousePos(){ return m_MousePosition; };

	/***********************
	* IsMouseClicked: TO DO: Description
	* @author: Jc Fowles
	* @return: bool*: TO DO: Description
	********************/
	bool* IsMouseClicked() const { return m_bIsClicked; }

	/***********************
	* IsMouseClicked: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: bool * val: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void IsMouseClicked(bool* _bIsClicked) { m_bIsClicked = _bIsClicked; }
	
	/***********************
	* IsKeyDown: Is this key down?
	* @author: Jc Fowles
	* @return: bool*: Returns whether this key is down
	********************/
	bool* IsKeyDown() const { return m_bIsKeyDown; }
	
	/***********************
	* IsKeyDown: Set whether this key is down
	* @author: Jc Fowles
	* @Parameter: bool * _IsKeyDown: bool value of whether this key is down
	* @return: void:
	********************/
	void IsKeyDown(bool* _IsKeyDown) { m_bIsKeyDown = _IsKeyDown; }

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
	




	//Member Variables
public:
	//TO DO: make private and create getters/Setters
	//bool* m_bIsKeyDown;
protected:

private:
	//Window variables
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;
	
	//Game Variables
	static CClientApp* s_pClientApp;
	CClock* m_pClock;
	eGameStates m_eGameState;
	eMenuStates m_eMenuState;
	eHostStates m_eHostState;
	std::string m_strGameTitle;
	std::vector<std::string> m_strMainMenuOptions;
	std::vector<std::string> m_strStartOptions;
	std::vector<std::string> m_strOptionsMenu;
	std::vector<std::string> m_strInstructions;
	std::vector<std::string> m_strExitOptions;
	POINT m_MousePosition;
	bool* m_bIsClicked;
	bool m_bMenuClicked;
	std::string m_strClickedMenu;
	bool* m_bIsKeyDown;
	bool m_bIsHost;

	//Networking variables
	CClient* m_pClient;
	std::thread m_RecieveThread;
	ServerDataPacket* m_pServerPacket;
	ClientDataPacket* m_pClientPacket;
	std::queue<ClientDataPacket>* m_pClientDataQueue;
	static CMySemaphore s_Mutex;
	//To DO 
	std::string m_strServerName;
	//char* m_strServerName;
	std::string m_strUserName;
	
	//Graphic Variables
	IRenderer* m_pRenderManager;
	
	
	
	
};

#endif //__CLIENT_APP_H__
