//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : SourceClient.cpp 
// Description : source file containing the main() function of the client- simply just creates the windows and then runs the Game app
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
//	#include "vld.h"
	#define D3D_DEBUG_INFO
#endif

//Library includes
#include <windowsx.h>
#include <ctime>
//Local Includes
#include "ClientApp.h"

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

	//Process the given message
    switch(_uiMsg)
    {
		case WM_ACTIVATEAPP:
		{

		}break;

		//Closing the window
		case WM_CLOSE:
		case WM_QUIT:
		case WM_DESTROY:
		{
			//Send message to close the entire application
			CClientApp::GetInstance().PreQuit();
			CClientApp::GetInstance().DestroyInstance();
			PostQuitMessage(0);
			return 0;
		}break;
		
		//***KEYBOARD***
		case WM_KEYUP:
		{
			//Set the IsKeyDown bool of this key to false
			CClientApp::GetInstance().IsKeyDown()[_wParam] = false;
			
		}break;

		case WM_KEYDOWN:
		{
			//Set the IsKeyDown bool of this key to true
			CClientApp::GetInstance().IsKeyDown()[_wParam] = true;
			CClientApp::GetInstance().ProcessInputs(_wParam);
			

		}break;

		//***MOUSE***
		case WM_LBUTTONDOWN:
		{
			//Left mouse button is down
			CClientApp::GetInstance().IsMouseClicked()[MK_LBUTTON] = true;

		}break;

		case WM_LBUTTONUP:
		{
			//Left mouse button is up
			CClientApp::GetInstance().IsMouseClicked()[MK_LBUTTON] = false;
			
		}break;
		
		case WM_RBUTTONDOWN:
		{
			//Right mouse button is down
			CClientApp::GetInstance().IsMouseClicked()[MK_RBUTTON] = true;
		}break;

		case WM_RBUTTONUP:
		{
			//Right mouse button is up
			CClientApp::GetInstance().IsMouseClicked()[MK_RBUTTON] = false;
		}break;
		
		case WM_MOUSEMOVE:
		{
			POINT MousePos; 
			MousePos.x = GET_X_LPARAM(_lParam);
			MousePos.y = GET_Y_LPARAM(_lParam);
			CClientApp::GetInstance().SetMousePos(MousePos);
		}break;
	
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
	WNDCLASSEX winClass;

	//Clear out the window class for use
    ZeroMemory(&winClass, sizeof(WNDCLASSEX));

	// Fills in the window class structure.
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = _hInstance;
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	// Registers the window class
	if (!RegisterClassEx(&winClass))
	{
		return (0);
	}
	

	//Create the window and return the result as the handle
	HWND hWnd;


	hWnd = CreateWindowEx(NULL,				// Extended style.
		WINDOW_CLASS_NAME,					// Class.
		_pcTitle,							// Title.
		WS_VISIBLE | WS_CAPTION |
		WS_BORDER | WS_SYSMENU,				// Windowed Mode
		0, 0,								// Initial x,y position for the top left corner of the window
		_iWidth, _iHeight,					// Initial width, height of the window
		NULL,								// Handle to parent.
		NULL,								// Handle to menu.
		_hInstance,							// Instance of this application.
		NULL);								// Extra creation parameters.


	
	//Check if window was created successfully
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
	CClientApp& rClientApp = CClientApp::GetInstance();
	rClientApp.Initialise(hWnd, kiWidth, kiHeight);
	


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
			if (rClientApp.RenderSingleFrame() == false)
			{
				break;
			}
		}
	}

	//Destroy the game instance
	rClientApp.PreQuit();
	rClientApp.DestroyInstance();
	
	
    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

