//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CGame.h
// Description : Header file containing declarations for CGame Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once
//TO DO IFDEF

#include <string>

//Local Includes
#include "Client.h"
#include "../Shared/Clock.h"
#include "../Shared/Network_Defines.h"


class CGame
{
	//Member Function
public:
	
	/***********************
	* GetInstance: Returns a refrence to the singleton game object, if one does not excist create one
	* @author: Jc Fowles
	* @return: CGame& : Returns a refrence to the singleton game object
	********************/
	static CGame& GetInstance();

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
	~CGame(void);

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
	
	//TO DO
	void ConvertToServerDataPacket(std::string _srtData);
protected:
	
private:
	/***********************
	* CGame: Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CGame();

	/***********************
	* CGame: Copy Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CGame(const CGame& _kr);

	/***********************
	* operator=: Overloaded
	* @author: Jc Fowles
	* @return:
	********************/
	CGame& operator= (const CGame& _kr);

	//Member Variables
public:
	
protected:

private:
	HWND m_hWnd;
	int m_iScreenWidth;
	int	m_iScreenHeight;
	
	static CGame* s_pGame;
	
	CClock* m_pClock;

	CClient* m_pClient;

	

};

