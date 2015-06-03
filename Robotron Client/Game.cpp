//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CGame.cpp
// Description : The core game app and its functionality for the client
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "Game.h"

CGame* CGame::s_pGame = 0;

CGame::CGame(void)
{

}

CGame::~CGame(void)
{

}

CGame& CGame::GetInstance()
{
	//Create the game intance if it doesnt exist 
	if (s_pGame == 0)
	{
		s_pGame = new CGame();
	}
	return (*s_pGame);
}

void CGame::DestroyInstance()
{	
	delete s_pGame;
	s_pGame = 0;
}

bool CGame::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	//Initialise game Window variables
	m_hWnd = _hWnd;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	std::string filename = "H:\\GitHub\\Robotron\\Debug\\Robotron Server";
	int errer = (int)ShellExecuteA(_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);
	
	return true;
		
}

void CGame::Process()
{
	
}

void CGame::Draw()
{
	
}

void CGame::RenderSingleFrame()
{
	int c = 9;
}


