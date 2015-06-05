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
	* GetInstance: Returns a refrence to the singleton ClientApp object, if one does not excist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a refrence to the singleton game object
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
	* @return: bool: Succesful Initialisation
	********************/
	bool Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight);

	/***********************
	* Process: Process the the ClientApp instance
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();

	/***********************
	* Draw: Draw the all the objects 
	* @author: Jc Fowles
	* @return: void
	********************/
	void Draw();

	/***********************
	* RenderSingleFrame: Render a single frame to the screen
	* @author: Jc Fowles
	* @return: void
	********************/
	void RenderSingleFrame();

	//TO DO: Comment header
	//void ProcessInputs(float _fDt);

	/***********************
	* ConvertToServerDataPacket: Convert passed in data to a ServerDataPacket
	* @author: Jc Fowles
	* @param: //TO DO
	* @return: void
	********************/
	void ConvertToServerDataPacket(std::string _srtData);

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
	void IsMouseClicked(bool* val) { m_bIsClicked = val; }

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

	//TO DO: Main menu
	void MainMenuDraw();
	void RenderText(std::string _strText, int _iYPos, eTextType _TextType);
	
	/***********************
	* ProcessMouseLClick: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void MenuSelection(std::string _strMenuItem);
	
	/***********************
	* StartMenuDraw: TO DO: Description
	* @author: Jc Fowles
	* @return: void: TO DO: Description
	********************/
	void StartMenuDraw();
	/***********************
	* ExitMenuDraw: TO DO: Description
	* @author: Jc Fowles
	* @return: void: TO DO: Description
	********************/
	void ExitMenuDraw();
	
	/***********************
	* MainMenuSelect: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void MainMenuSelect(std::string _strMenuItem);
	
	/***********************
	* StartMenuSelect: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void StartMenuSelect(std::string _strMenuItem);
	
	/***********************
	* ExitMenuSelect: TO DO: Description
	* @author: Jc Fowles
	* @Parameter: std::string _strMenuItem: TO DO: Description
	* @return: void: TO DO: Description
	********************/
	void ExitMenuSelect(std::string _strMenuItem);
	
	/***********************
	* HostGameDraw: TO DO: Description
	* @author: Jc Fowles
	* @return: void: TO DO: Description
	********************/
	void HostGameDraw();
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
	eGameStates m_GameState;
	std::string m_strGameTitle;
	std::vector<std::string> m_strMainMenuOptions;
	//TO DO
	std::vector<std::string> m_strStartOptions;
	/*std::vector<std::string> m_strOptionsMenu;
	std::vector<std::string> m_strInstructions;*/
	std::vector<std::string> m_strExitOptions;
	POINT m_MousePosition;
	bool* m_bIsClicked;
	
	//Networking variables
	CClient* m_pClient;
	std::thread m_ClientThread;
	ServerDataPacket* m_ServerPacket;
	ClientDataPacket* m_ClientPacket;
	std::queue<ClientDataPacket>* m_pClientDataQueue;
	static CMySemaphore s_Mutex;
	
	//Graphic Variables
	IRenderer* m_pRenderManager;
	
};

#endif //__CLIENT_APP_H__
