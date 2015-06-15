//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CInputManager.h
// Description : Header file containing declarations for InputManager Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CINPUTMANAGER_H__
#define __CINPUTMANAGER_H__

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "DXGuid.lib" )

//library include
#include <dinput.h>

//local include
#include "..\shared\App_Defines.h"

//TO DO : remove comment
//http://www.rastertek.com/dx10tut13.html

class CInputManager
{
public:
	CInputManager();
	~CInputManager();
		
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight);
	bool InitialiseKeyBoard(HWND _hWnd);
	bool InitialiseMouse(HWND _hWnd);
	bool ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize);
	
	void ProcessInput();
	void ProcessMouse();
	void ProcessKeyBoard();


	InputStates GetInputStates() { return m_InputStates; };
	void SetInputStates(InputStates _PlayCont){ m_InputStates = _PlayCont; };
	void ResetInputStates();
	//Member variables
public :
	
private:

	HWND m_hWnd;
	int m_iScreenWidth;
	int m_iScreenHeight;

	IDirectInput8* m_pDirectInput;

	IDirectInputDevice8* m_pDIKeyboard;
	unsigned char m_cKeyStateBuffer[AppDefines::BUFFER_SIZE];
	
	IDirectInputDevice8* m_pDIMouse;
	DIMOUSESTATE m_mouseState;
	POINT m_MousePos;
	
	InputStates m_InputStates;

};

#endif //__CINPUTMANAGER_H__