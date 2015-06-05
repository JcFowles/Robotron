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

//librabry includes

//Local Includes

enum eGameStates
{
	MAIN_MENU
};

#define VALIDATE(a) if (!a) return (false)

#endif //__APP_DEFINES_H__


