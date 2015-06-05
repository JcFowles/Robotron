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
#include "../Shared/Clock.h"


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
	bool* m_bIsKeyDown;
protected:

private:
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;

	static CClientApp* s_pClientApp;
	CClock* m_pClock;

	CClient* m_pClient;

	std::thread m_ClientThread;

	ServerDataPacket* m_ServerPacket;
	ClientDataPacket* m_ClientPacket;
	std::queue<ClientDataPacket>* m_pClientDataQueue;

	static CMySemaphore s_Mutex;
	
	//Gamestate

	//std::queue<bool*>
};

#endif //__CLIENT_APP_H__
