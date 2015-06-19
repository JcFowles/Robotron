
//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CServerApp.h
// Description : Header file containing declarations for CServerApp Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

//Library includes
#include <thread>

//Local Includes
#include "Server.h"
#include "../Shared/Clock.h"
#include "Game.h"

class CServerApp
{
	//Member Function
public:

	/***********************
	* GetInstance: Returns a reference to the singleton game object, if one does not exist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a reference to the singleton game object
	********************/
	static CServerApp& GetInstance();

	/***********************
	* DestroyInstance: Destroys the singleton game object
	* @author: Jc Fowles
	* @return: void
	********************/
	static void DestroyInstance();

	/***********************
	* ~CGame: Destructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CServerApp(void);

	/***********************
	* Initialize: Initializes the Game instance
	* @author: Jc Fowles
	* @parameter: _hWnd: Handle to the window
	* @parameter: _iScreenWidth: Width to the window
	* @parameter: _iScreenHeight: Height to the window
	* @parameter: _pCmdArgs: Command line arguments
	* @return: bool: Successful Initialization
	********************/
	bool Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight, LPWSTR* _pCmdArgs);

	/***********************
	* Process: Process the the game instance
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();

	/***********************
	* Draw: Draw the objects to the D3D Device
	* @author: Jc Fowles
	* @return: void
	********************/
	void Draw();

	/***********************
	* RenderSingleFrame: Render a single frame to the screen
	* @author: Jc Fowles
	* @return: void
	********************/
	bool RenderSingleFrame();


protected:

private:
	/***********************
	* CGame: Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CServerApp();

	/***********************
	* CGame: Copy Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CServerApp(const CServerApp& _kr);

	/***********************
	* operator=: Overloaded
	* @author: Jc Fowles
	* @return:
	********************/
	CServerApp& operator= (const CServerApp& _kr);

	/***********************
	* ReceiveDataThread: Threaded function to receive data from the client and add them 
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
	* ProcessCreation: Process the CREATE message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessCreation();

	/***********************
	* ProcessFind: Process the FIND message 
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessFind();
	
	/***********************
	* ProcessFind: Process the JOIN_REQUEST message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessJoinRequest();

	/***********************
	* ProcessActive: Process the ACTIVE message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessActive();

	/***********************
	* ProcessFind: Process the DEFAULT message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessDefault();

	/***********************
	* ProcessLeave: Process the LEAVE message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessLeave();

	/***********************
	* ProcessQuit: Process the QUIT message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessQuit();
	
	/***********************
	* SetServerInfo: Set the Server info of the packet before sending
	* @author: Jc Fowles
	* @return: void:
	********************/
	void SetServerInfo();
		
	/***********************
	* SetGameState: Reset and set the game states on the data to send before sending
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: pointer to the data to send that game states need to be reset and set
	* @return: void: 
	********************/
	void SetGameState(ClientDataPacket* _pDataToSend);
	
	/***********************
	* SetActiveClientList: Reset and set the active clients on the data to send before sending
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: pointer to the data to send that Active client list need to be reset and set
	* @return: void: 
	********************/
	void SetActiveClientList(ClientDataPacket* _pDataToSend);
		
	/***********************
	* SendToAll: Send to all Clients in the map
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Data to be sent to all clients
	* @return: bool: Successful sending
	********************/
	bool SendToAll(ClientDataPacket* _pDataToSend);
		
	/***********************
	* AddUser: Add a user to the map of clients
	* @author: Jc Fowles
	* @Parameter: std::string _UserName: Name of the the client used as the key to the map
	* @Parameter: ClientInfo _ClientInfo: Client info as the body of the map
	* @return: bool: 
	********************/
	bool AddUser(std::string _UserName, ClientInfo _ClientInfo);
		
	/***********************
	* AllActive: Check to see if all clients are 'ready' and active
	* @author: Jc Fowles
	* @return: bool: state of all active clients, (false if not all are ready, true if all are ready)
	********************/
	bool AllActive();
	
	//Member Variables
public:

protected:

private:
	//Window variables
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;

	//Server Variables
	static CServerApp* s_pServerApp;
	std::string m_strServerName;
	
	//Game variables
	CGame* m_pGame;
	CClock* m_pClock;
	bool m_bGameStart;
	bool m_bSinglePlayer;

	//Networking Variables
	CServer* m_pServer;
	std::thread m_RecieveThread;
	ServerDataPacket* m_pServerPacket;
	ClientDataPacket* m_pClientPacket;
	std::queue<ServerDataPacket>* m_pServerDataQueue;
	static CMySemaphore s_Mutex;
	bool m_bHaveHost;
		
	std::string m_strHostClient;
	
	//TO DO move to class sever game
	//Game Variables
	//std::map<std::string, PlayerStates>* m_pMapPlayerStates;
	std::map< std::string, ClientInfo>* m_pMapClientInfo;

};

