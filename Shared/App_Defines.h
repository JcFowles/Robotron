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

enum eMenuStates
{
	//Main menu
	MS_MAIN,
	MS_SINGLE_PLAYER,
	MS_MULTI_PLAYER,
	MS_OPTIONS,
	MS_INSTRUCTIONS,
	MS_EXIT,

	//Game
	MS_JOIN_GAME,
	MS_HOST_GAME
};

enum eGameStates
{
	GS_MENU,
	GS_PLAY
};

enum eHostStates
{
	HS_DEFAULT,
	HS_SERVER_NAME,
	HS_USER_NAME,
	HS_DONE
};


#define VALIDATE(a) if (!a) return (false)

#endif //__APP_DEFINES_H__


