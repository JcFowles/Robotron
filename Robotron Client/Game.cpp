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
	delete m_pClient;
	m_pClient = 0;
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

	//Initialise member variables
	m_pClient = new CClient();
	VALIDATE(m_pClient->Initialise());


	std::string filename;
#ifdef _DEBUG
	filename = "..\\Debug\\Robotron Server";
#endif
#ifndef _DEBUG
	//TO DO: change file path for final release
	filename = "..\\Release\\Robotron Server";
#endif
	
	//int errer = (int)ShellExecuteA(_hWnd, "open", filename.c_str(), NULL, NULL, SW_NORMAL);
	
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
	ServerDataPacket* packet = new ServerDataPacket;
	packet->iNumber = 78;
	std::string steTest = "12345678901234567890123456789012345678901234567890123456789012";
	strcpy_s(packet->cText, steTest.c_str());

	m_pClient->SendData(packet);
}

void CGame::ConvertToServerDataPacket(std::string _srtData)
{
	ServerDataPacket* packet = new ServerDataPacket;
	if (_srtData.length() < NetworkValues::MAX_CHAR_LENGTH)
	{
		strcpy_s(packet->cText, _srtData.c_str());
	}
	
}
