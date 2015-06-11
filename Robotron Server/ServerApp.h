
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
	* ProcessFind: Process the DEFAULT message
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessDefault();

	/***********************
	* AddTextToClientDataPacket: Convert passed in data to a ClientDataPacket
	* @author: Jc Fowles
	* @param: std::string _srtText: String Data to be converted
	* @return: void
	********************/
	void AddTextToClientDataPacket(std::string _srtText);
	
	/***********************
	* AddHostNameToServerInfo: Add the Host name to server info
	* @author: Jc Fowles
	* @param: std::string _srtHostName: Host name to be added
	* @return: void
	********************/
	void AddHostNameToServerInfo(std::string _srtHostName);

	/***********************
	* AddServerToServerInfo: Add the server name to server info
	* @author: Jc Fowles
	* @Parameter: std::string _srtServerName: Server name to be added
	* @return: void: 
	********************/
	
	
	
	
	/***********************
	* AddServerToServerInfo: TO DO:
	* @author: Jc Fowles
	* @Parameter: std::string _srtServerName: TO DO:
	* @return: void: 
	********************/
		void AddServerToServerInfo(std::string _srtServerName);
	
	/***********************
	* WideStringToString: Convert a Wide string to a standard string
	* @author: Jc Fowles
	* @Parameter: wchar_t * _wstr: Wide string to be converted
	* @return: std::string: The converted Wide string as a standard string
	********************/
	std::string WideStringToString(wchar_t* _wstr);
	
	/***********************
	* SetServerInfo: Set the Server info of the packet before sending
	* @author: Jc Fowles
	* @return: void:
	********************/
	void SetServerInfo();

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
	CClock* m_pClock;
	std::string m_strServerName;
	bool m_bGameStart;

	//Networking Variables
	CServer* m_pServer;
	std::thread m_RecieveThread;
	ServerDataPacket* m_pServerPacket;
	ClientDataPacket* m_pClientPacket;
	std::queue<ServerDataPacket>* m_pServerDataQueue;
	static CMySemaphore s_Mutex;
	bool m_bHaveHost;

	
	std::string m_strHostClient;
	

};

