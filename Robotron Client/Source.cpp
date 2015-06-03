//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Source.cpp 
// Description :Implementation file that holds the functionality of the program
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )

//Library includes
#include <ctime>
#include <windows.h>
#include <windowsx.h>

#ifdef _DEBUG
//#include "vld.h"
#endif

//Local Includes
#include "Game.h"

#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"ROBOTRON"

/***********************
* WindowProc: This is the message handler for the Window, 
* @author: Jc Fowles
* @Parameter: _hWnd: Handle to the Window sending the message
* @Parameter: _uiMsg: The message ID being sent
* @Parameter: _wParam: Additional detail about the message being sent
* @Parameter: _lParam: Additional detail about the message being sent
* @return: LRESULT: Program termination value
********************/
LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{

	//Procces the given message
    switch(_uiMsg)
    {
	case WM_ACTIVATEAPP:
		{

		}
		break;
        //Closing the window
        case WM_DESTROY:
            {
                //Send message to close the entire application
                PostQuitMessage(0);
                return 0;
            } 
		break;
		case WM_KEYUP:
		{
			////Set the IsKeyDown bool of this key to false
			//CGame::GetInstance().m_bIsKeyDown[_wParam] = false;	
			//if (_wParam == VK_NUMPAD8)
			//{
			//	CGame::GetInstance().GetCube()->SetMoveSpeed(50.0f);
			//}
		}
		break;
		case WM_KEYDOWN:
		{
			//Set the IsKeyDown bool of this key to true
			//CGame::GetInstance().m_bIsKeyDown[_wParam] = true;
			switch (_wParam)
			{
				case VK_ESCAPE:
				{
					PostQuitMessage(0);
					return 0;
				}
				break;
			}
		}
		break;
    }

	 //Handles any messages the switch statement doesn't
	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

/***********************
* CreateAndRegisterWindow: Create and register the window of the application 
* @author: Jc Fowles
* @Parameter: _hInstance: Instance handler for the application
* @Parameter: _iWidth: Width of the window
* @Parameter: _iHeight: Height of the window
* @Parameter: _pcTitle: Title of the window
* @return: HWND: Handle to the window
********************/
HWND CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, LPCWSTR _pcTitle)
{
	WNDCLASSEX winclass;

	//Clear out the window class for use
    ZeroMemory(&winclass, sizeof(WNDCLASSEX));

	//Fill in the struct with the needed information
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)WHITE_BRUSH;
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register the window class
	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	//Create the window and return the result as the handle
	HWND hWnd;
	
	hWnd = CreateWindowEx(	NULL,												// Extend style.
							WINDOW_CLASS_NAME,									// Class.		
							_pcTitle,											// Title.
							//WS_EX_TOPMOST | WS_POPUP,							//Window Style(Full Screan)
							WS_OVERLAPPEDWINDOW | WS_BORDER | 
							WS_CAPTION | WS_SYSMENU | WS_VISIBLE,				//Window stlye(Windowed)
							CW_USEDEFAULT, CW_USEDEFAULT,						// Initial x,y.
							_iWidth, _iHeight,									// Initial width, height.
							NULL,												// Handle to parent.
							NULL,												// Handle to menu.
							_hInstance,											// Instance of this application.
							NULL);												// Extra creation parameters.
	
	//Check if window was created succesfully
	if (!hWnd)
	{
		// Failed to create.
		return (0);
	}

	//Return the handle to the window
	return (hWnd);
}

/***********************
* WinMain: The entry point for any Windows program
* @author: Jc Fowles
* @Parameter: _hInstance: Instance handle that Windows generates for your application
* @Parameter: _hPrevInstance: Tracker for the previous instance for the application
* @Parameter: _lpCmdline: Wide char string that contains the passed in arguments 
* @Parameter: _iCmdshow: Integer passed in during launch, indicating how the application window is opened
* @return: int: Program termination value
********************/
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	//Seed the random generator
	srand ((unsigned int)time(NULL));

	//Screen Resolution
	const int kiWidth = 1000; 
	const int kiHeight = 1000;
	
	//This holds Windows event messages
	MSG msg;

	//Clear out the event message for use
	ZeroMemory(&msg, sizeof(MSG));
	
	//Create and register the window
	HWND hWnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"Robotron");
	
	// display the window on the screen
    ShowWindow(hWnd, _iCmdshow);

	//Create and initialize the Direct3D Device
	CGame& rGame = CGame::GetInstance();
	rGame.Initialise(hWnd, kiWidth, kiHeight);
			

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Render a single frame
			rGame.RenderFrame();		
		}
	}

	//Destroy the game instance
	rGame.DestroyInstance();
	
    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

