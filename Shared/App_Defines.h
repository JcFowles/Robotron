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
#include <string>

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

enum eEnemyTypes
{
	ET_LUST,	//Grabbing enemy that tries to hold onto you slowing you down and damaging you over time
	ET_PRIDE,	//Runs(flee) away and heals it self when hurt
	ET_WRATH,	//Big angry 
	ET_GREED,	//Points stealer
	ET_SLOTH,	//BIG Wanderer passive till attacked
	ET_ENVY,	//
	ET_GLUTTONY	//Eats other objects gets bigger, slower, harder to kill, more points, and more damage
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

//Vector with 2 float values
struct float2
{
	float u;
	float v;

	/***********************
	* float2: constructor for the float2 type
	* @author: Jc Fowles
	* @Parameter: float _u: float to be stored in the u value of the float2
	* @Parameter: float _v: float to be stored in the v value of the float2
	* @return: :
	********************/
	float2(float _u = 0, float _v = 0)
	{
		u = _u;
		v = _v;
	}

};

//Vector with 3 float values
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
	* operator+: Addition and assignment operator
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: right hand side of the operator
	* @return: float3: The result of the addition and assignment
	********************/
	float3 operator+= (const float3 _f3) 
	{
		return float3(x += _f3.x, y += _f3.y, z += _f3.z);
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
	
	/***********************
	* Magnitude: Calculate and return the magnitude of the float3 vector
	* @author: Jc Fowles
	* @return: float: The magnitude of the float3 vector
	********************/
	float Magnitude()
	{
		return ( sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) ) );
	}

	/***********************
	* Normalise: Calculate and return a normalized vector of the float3 vector
	* @author: Jc Fowles
	* @return: float: The normalized float3 vector 
	********************/
	float3 Normalise()
	{
		float mag = this->Magnitude();
		if (mag == 0.0f)
		{
			//Return a zero vector if magnitude is zero
			return float3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			//Return the normalized vector 
			return ((*this)*(1 / mag));
		}
	}

};

//Vector with 4 float values
struct float4
{
	float r; //Red
	float g; //Green
	float b; //Blue
	float a; //Alpha

};

#define VALIDATE(a) if (!a) return (false)

#endif //__APP_DEFINES_H__


