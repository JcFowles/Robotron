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

//TO DO : Comment Headers

class CInputManager
{
public:
	
	/***********************
	* CInputManager: Default Constructor for Input manager class 
	* @author: Jc Fowles
	* @return: : 
	********************/
	CInputManager();

	/***********************
	* ~CInputManager: Destructor for Input manager class
	* @author: Jc Fowles
	* @return: :
	********************/
	~CInputManager();
			
	/***********************
	* Initialise: Initialise the input manager for use
	* @author: Jc Fowles
	* @Parameter: HINSTANCE _hInstance: Handle to the instance
	* @Parameter: HWND _hWnd: Handle to the window
	* @Parameter: int _iScreenWidth: width of the window
	* @Parameter: int _iScreenHeight: height of the window
	* @return: bool: successful initialization
	********************/
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight);
		
	/***********************
	* InitialiseKeyBoard: Initialise the Keyboard
	* @author: Jc Fowles
	* @Parameter: HWND _hWnd: Handle to the window
	* @return: bool: successful initialization
	********************/
	bool InitialiseKeyBoard(HWND _hWnd);

	/***********************
	* InitialiseMouse: Initialise the Mouse
	* @author: Jc Fowles
	* @Parameter: HWND _hWnd: Handle to the window
	* @return: bool: successful initialization
	********************/
	bool InitialiseMouse(HWND _hWnd);
		
	/***********************
	* ReadDevice: Read and acquire the state of the device 
	* @author: Jc Fowles
	* @Parameter: IDirectInputDevice8 * _pDIDevice: Input Device to read
	* @Parameter: void * _pDataBuffer: Buffer to save input state
	* @Parameter: int _iBufferSize: size of buffer to save input states
	* @return: bool: successful Reading
	********************/
	bool ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize);
	
	/***********************
	* ProcessInput: Process the input from user
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessInput();

	/***********************
	* ProcessMouse: Process the mouse input from user
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessMouse();

	/***********************
	* ProcessKeyBoard: Process the Keyboard input from user
	* @author: Jc Fowles
	* @return: void:
	********************/
	void ProcessKeyBoard();

	/***********************
	* GetInputStates: Returns the input states (which controls have been pushed, etc
	* @author: Jc Fowles
	* @return: InputStates: The input states
	********************/
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
	unsigned char m_cKeyStateBuffer[256];
	
	IDirectInputDevice8* m_pDIMouse;
	DIMOUSESTATE m_mouseState;
	POINT m_MousePos;
	
	InputStates m_InputStates;

};

#endif //__CINPUTMANAGER_H__