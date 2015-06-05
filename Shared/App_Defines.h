//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : App_Defines.h 
// Description : Header file containing Main application Defines, Structures, Enum, ect
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once
#if !defined(__APP_DEFINES_H__)
#define __APP_DEFINES_H__

#pragma comment( lib, "winmm.lib" )
#pragma comment(lib, "Ws2_32.lib")

//library includes

//Local Includes

enum eGameStates
{
	//Main menu
	GS_MAIN_MENU,
	GS_START_MENU,
	GS_OPTIONS_MENU,
	GS_INSTRUCTIONS,
	GS_EXIT_MENU,

	//Game
	GS_JOIN_GAME,
	GS_HOST_GAME
};

#define VALIDATE(a) if (!a) return (false)

#endif //__APP_DEFINES_H__


