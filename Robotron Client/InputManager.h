#pragma once

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "DXGuid.lib" )


#include <dinput.h>

//http://www.rastertek.com/dx10tut13.html

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	bool Initialise(HINSTANCE _hInstance, HWND _hWnd);
	bool ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize);
	bool Shutdown();

	//Member variables
public :
	IDirectInput8* m_pDirectInput;

	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;

	unsigned char m_cKeyStateBuffer[256];
	DIMOUSESTATE m_mouseState;

	int m_iScreenWidth;
	int m_iScreenHeight;
	//int m_iMouseX, m_iMouseY;

	POINT m_MousePos;

private:
	HWND m_hWnd;

	
};

