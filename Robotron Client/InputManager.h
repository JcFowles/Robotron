//TO DO
#pragma once

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "DXGuid.lib" )

//library include
#include <dinput.h>

//local include
#include "..\shared\App_Defines.h"

//http://www.rastertek.com/dx10tut13.html

class CInputManager
{
public:
	CInputManager();
	~CInputManager();
	
	void Shutdown();
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight);
	bool InitialiseKeyBoard(HWND _hWnd);
	bool InitialiseMouse(HWND _hWnd);
	bool ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize);
	
	void ProcessInput();
	void ProcessMouse();
	void ProcessKeyBoard();


	POINT GetMousePos();
	PlayControls GetControlState() { return m_playControls; };
	void SetControlState(PlayControls _PlayCont){ m_playControls = _PlayCont; };
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
	
	PlayControls m_playControls;

};

