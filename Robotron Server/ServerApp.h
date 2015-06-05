
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
	* GetInstance: Returns a refrence to the singleton game object, if one does not excist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a refrence to the singleton game object
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
	* @return: bool: Succesful Initialisation
	********************/
	bool Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight);

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
	void RenderSingleFrame();


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

	//Member Variables
public:

protected:

private:
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;

	static CServerApp* s_pServerApp;

	CClock* m_pClock;
	
	CServer* m_pServer;
	std::thread m_RecieveThread;

	ServerDataPacket* m_ServerPacket;
	ClientDataPacket* m_ClientPacket;
	std::queue<ServerDataPacket>* m_pServerDataQueue;

	static CMySemaphore s_Mutex;
};

