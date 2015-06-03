//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CGame.cpp
// Description : Implementation file that holds the functionality of the Game Class
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

void CGame::Initialise(HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	
}

void CGame::Process()
{
	
}

void CGame::Draw()
{
	
}

void CGame::RenderSingleFrame()
{

}


