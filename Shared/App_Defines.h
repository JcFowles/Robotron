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

#ifndef __APP_DEFINES_H__
#define __APP_DEFINES_H__

#pragma comment( lib, "winmm.lib" )
#pragma comment(lib, "Ws2_32.lib")

//library includes
#include <cassert>

//Local Includes

namespace AppDefines
{
	unsigned const BUFFER_SIZE = 256;
};

enum eMenuStates
{
	MS_MAIN,
	MS_SINGLE_PLAYER,
	MS_MULTI_PLAYER,
	MS_OPTIONS,
	MS_INSTRUCTIONS,
	MS_EXIT,
	MS_JOIN_GAME,
	MS_HOST_GAME,
	MS_LOBBY,
};

enum eGameStates
{
	GS_MENU,
	GS_PLAY,
};

enum eHostStates
{
	HS_DEFAULT,
	HS_SERVER_NAME,
	HS_USER_NAME,
	HS_DONE
};

struct InputStates
{
	bool bUpPress;
	bool bDownPress;
	bool bLeftPress;
	bool bRightPress;
	bool bActivate;
	bool bEscape;
	POINT CursorPos;
};

//Float with 3 values
struct float3
{
	float x;
	float y;
	float z;
		
	/***********************
	* float3: constructor for the float3 type
	* @author: Jc Fowles
	* @Parameter: float _x: float to be stored in the x value of the float3
	* @Parameter: float _y: float to be stored in the y value of the float3
	* @Parameter: float _z: float to be stored in the z value of the float3
	* @return: : 
	********************/
	float3(float _x = 0, float _y = 0, float _z = 0) 
	{
		x = _x;
		y = _y;
		z = _z;
	}
		
	/***********************
	* operator=: Assignment operator 
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: value for 'this' to be assigned to
	* @return: float3: return the new assigned float3
	********************/
	float3 operator= (const float3 _f3)
	{
		x = _f3.x; 
		y = _f3.y; 
		z = _f3.z; 
		return *this; 
	}

	/***********************
	* operator+: Addition operator
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: right hand side of the operator
	* @return: float3: The result of the addition
	********************/
	float3 operator+ (const float3 _f3) const 
	{ 
		return float3(_f3.x + x, _f3.y + y, _f3.z + z);
	}

	/***********************
	* operator-: Subtraction operator
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: right hand side of the operator
	* @return: float3: The result of the subtraction
	********************/
	float3 operator- (const float3 _f3) const
	{
		return float3(_f3.x - x, _f3.y - y, _f3.z - z);
	}

	/***********************
	* operator*: Multiplication operator
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: right hand side of the operator
	* @return: float3: The result of the Multiplication
	********************/
	float3 operator* (const float3 _f3) const
	{
		return float3(_f3.x * x, _f3.y * y, _f3.z * z);
	}

};

struct float4
{
	float r; //Red
	float g; //Green
	float b; //Blue
	float a; //Alpha

};



#define VALIDATE(a) if (!a) return (false)

#endif //__APP_DEFINES_H__


