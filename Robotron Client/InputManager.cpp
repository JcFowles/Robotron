//TO DO
#include "InputManager.h"


CInputManager::CInputManager()
{
	m_hWnd = 0;
	m_pDirectInput = 0;
	m_pDIKeyboard = 0;
	m_pDIMouse = 0;
}

CInputManager::~CInputManager()
{
	//Release the mouse
	if (m_pDIMouse)
	{
		m_pDIMouse->Unacquire();
		m_pDIMouse->Release();
		m_pDIMouse = 0;
	}

	//Release the keyboard
	if (m_pDIKeyboard)
	{
		m_pDIKeyboard->Unacquire();
		m_pDIKeyboard->Release();
		m_pDIKeyboard = 0;
	}

	//Release the direct input interface
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
		m_pDirectInput = 0;
	}
}

bool CInputManager::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	//Initialise member variable
	m_hWnd = _hWnd;
	//Initialise the player controls
	m_InputStates.bUpPress = false;
	m_InputStates.bDownPress = false;
	m_InputStates.bLeftPress = false;
	m_InputStates.bRightPress = false;
	m_InputStates.bActivate = false;
	m_InputStates.bEscape = false;

	//Local variable
	HRESULT hResult;
	
	//Store the screen size to be used for positioning the mouse cursor.
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;
	
	//Create the DirectInput Interface
	hResult = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (FAILED(hResult))
	{
		m_pDirectInput->Release();
		return false;
	}

	//Initialise the keyboard and mouse
	if (InitialiseKeyBoard(_hWnd) == false)
	{
		m_pDIKeyboard->Release();
		return false;
	}
	if (InitialiseMouse(_hWnd) == false)
	{
		m_pDIMouse->Release();
		return false;
	}
		
	return true;
	
}

bool CInputManager::InitialiseKeyBoard(HWND _hWnd)
{
	HRESULT hResult;

	//Initialize the DirectInput Interface for the keyboard
	m_pDIKeyboard = 0;
	hResult = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	VALIDATE(SUCCEEDED(hResult));

	//Set the data format of the keyboard.
	hResult = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	VALIDATE(SUCCEEDED(hResult));

	//Set the cooperative level of the mouse
	hResult = m_pDIKeyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	VALIDATE(SUCCEEDED(hResult));

	//Acquire the keyboard
	hResult = m_pDIKeyboard->Acquire();
	VALIDATE(SUCCEEDED(hResult));


	return true;
}

bool CInputManager::InitialiseMouse(HWND _hWnd)
{
	HRESULT hResult;
		
	//Initialize the location of the mouse on the screen.
	m_MousePos.x = 0;
	m_MousePos.y = 0;

	//Initialize the direct input interface for the mouse
	hResult = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	VALIDATE(SUCCEEDED(hResult));
	
	//Set the data format of the mouse
	hResult = m_pDIMouse->SetDataFormat(&c_dfDIMouse);
	VALIDATE(SUCCEEDED(hResult));
	
	//Set the cooperative level of the mouse
	hResult = m_pDIMouse->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	VALIDATE(SUCCEEDED(hResult));

	//Acquire the mouse
	hResult = m_pDIMouse->Acquire();
	VALIDATE(SUCCEEDED(hResult));

	return true;
}

bool CInputManager::ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize)
{
	HRESULT hResult;

	//Run while loop until we has successfully got the passed in device state
	while (true)
	{
		//No device
		if (_pDIDevice == 0)
		{
			return false;
		}
		//Poll the device.
		_pDIDevice->Poll();

		//Read in the state.
		hResult = _pDIDevice->GetDeviceState(_iBufferSize, (LPVOID)_pDataBuffer);
		if (SUCCEEDED(hResult))
		{
			break;
		}

		//Return false on all errors except not lost device or un-acquired device
		if ((hResult != DIERR_INPUTLOST) &&
			(hResult != DIERR_NOTACQUIRED))
		{
			return false;
		}

		//Reacquire the device if we have lost the device or need to reacquire it
		//Then try again
		if (FAILED(_pDIDevice->Acquire()))
		{
			//unable to acquire device
			return false;
		}
		
		
	}
	return true;
}

void CInputManager::ProcessInput()
{
	ReadDevice(m_pDIMouse, (void*)&m_mouseState, sizeof(DIMOUSESTATE));
	ReadDevice(m_pDIKeyboard, (void*)m_cKeyStateBuffer, 256);
	ProcessMouse();
	ProcessKeyBoard();
	
}

void CInputManager::ProcessMouse()
{
	//Update the mouse cursor location based on the change of the mouse location during the frame
	//m_MousePos.x += m_mouseState.lX;
	//m_MousePos.y += m_mouseState.lY;

	//Set mouse position to the cursor position
	GetCursorPos(&m_MousePos);
	ScreenToClient(m_hWnd, &m_MousePos);
		
	//Ensure the mouse location doesn't exceed the screen width or height.
	if (m_MousePos.x < 0)  { m_MousePos.x = 0; }
	if (m_MousePos.x > m_iScreenWidth)  { m_MousePos.x = m_iScreenWidth; }
	if (m_MousePos.y < 0)  { m_MousePos.y = 0; }
	if (m_MousePos.y > m_iScreenHeight) { m_MousePos.y = m_iScreenHeight; }

	//TO DO Control remap
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		m_InputStates.bActivate = true;
	}
	else
	{
		m_InputStates.bActivate = false;
	}

	m_InputStates.CursorPos = m_MousePos;
}

void CInputManager::ProcessKeyBoard()
{
	//TO DO remapping
	((m_cKeyStateBuffer[DIK_W] & 0x80))		 ? (m_InputStates.bUpPress = true)		: (m_InputStates.bUpPress = false);
	((m_cKeyStateBuffer[DIK_S] & 0x80))		 ? (m_InputStates.bDownPress = true)	: (m_InputStates.bDownPress = false);
	((m_cKeyStateBuffer[DIK_A] & 0x80))		 ? (m_InputStates.bLeftPress = true)	: (m_InputStates.bLeftPress = false);
	((m_cKeyStateBuffer[DIK_D] & 0x80))		 ? (m_InputStates.bRightPress = true)	: (m_InputStates.bRightPress = false);
	((m_cKeyStateBuffer[DIK_ESCAPE] & 0x80)) ? (m_InputStates.bEscape = true)		: (m_InputStates.bEscape = false);
	((m_cKeyStateBuffer[DIK_TAB] & 0x80))	 ? (m_InputStates.bTab = true)			: (m_InputStates.bTab = false);
	((m_cKeyStateBuffer[DIK_F1] & 0x80))	 ? (m_InputStates.bToggle = true)		: (m_InputStates.bToggle = false);
	((m_cKeyStateBuffer[DIK_F4] & 0x80))	 ? (m_InputStates.bDebug = true)		: (m_InputStates.bDebug = false);
}

void CInputManager::ResetInputStates()
{
	m_InputStates.bActivate = false;
	m_InputStates.bEscape = false;
	
	m_InputStates.bLeftPress = false;
	m_InputStates.bRightPress = false;
	m_InputStates.bUpPress = false;
	m_InputStates.bDownPress = false;

	m_InputStates.CursorPos = { 0, 0 };
}
