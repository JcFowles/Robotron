#include "InputManager.h"


CInputManager::CInputManager()
{
	m_pDirectInput = 0;
	m_pDIKeyboard = 0;
	m_pDIMouse = 0;
}


CInputManager::~CInputManager()
{
}

bool CInputManager::Shutdown()
{

	m_pDIKeyboard->Unacquire();
	//Release the DirectInput device
	m_pDirectInput->Release();
}


bool CInputManager::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iScreenWidth, int _iScreenHeight)
{
	m_hWnd = _hWnd;

	HRESULT hResult;
	
	//Store the screen size to be used for positioning the mouse cursor.
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;
	
	//Initialize the location of the mouse on the screen.
	m_MousePos.x = 0;
	m_MousePos.y = 0;
	

	//Create the DirectInput Interface
	hResult = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (FAILED(hResult))
	{
		return (false);
	}

	//Initialize the DirectInput Interface for the keyboard
	m_pDIKeyboard = 0;
	hResult = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);	//Failed to create
	if (FAILED(hResult))
	{
		return (false);
	}

	//Set the keyboard structure to read in data
	hResult = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	//Failed to set
	if (FAILED(hResult))
	{
		m_pDIKeyboard->Release();
		return (false);
	}


	hResult = m_pDIKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//Failed to set
	if (FAILED(hResult))
	{
		m_pDIKeyboard->Release();
		return (false);
	}

	hr = m_pDIKeyboard->Acquire();
	//Failed to acquire
	if (FAILED(hr))
	{
		m_pDIKeyboard->Release();
		return (false);
	}
		

	return true;
	

}

void GetKey

bool CInputManager::ReadDevice(IDirectInputDevice8* _pDIDevice, void* _pDataBuffer, int _iBufferSize)
{
	HRESULT hr;
	while (true)
	{
		//Poll the device.
		_pDIDevice->Poll();
		//Read in the state.
		hr = _pDIDevice->GetDeviceState(_iBufferSize, (LPVOID)_pDataBuffer);
		if (SUCCEEDED(hr))
		{
			break;
		}
		//Return false on an unknown error!
		if (hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
		{
			return (false);
		}
		//Reacquire the device and try again...
		if (FAILED(_pDIDevice->Acquire()))
		{
			return (false);
		}
	}
	return (true);
}