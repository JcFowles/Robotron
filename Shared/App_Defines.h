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
	MS_LOADING
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

enum ePowerType
{
	PU_SHIELD, //Limited time invulnerable
	//TO DO: More
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
		return float3(x - _f3.x, y - _f3.y, z - _f3.z);
	}

	/***********************
	* operator-=: Subtraction and assignment operator
	* @author: Jc Fowles
	* @Parameter: const float3 _f3: right hand side of the operator
	* @return: float3: The result of the Subtraction and assignment
	********************/
	float3 operator-= (const float3 _f3)
	{
		return float3(x -= _f3.x, y -= _f3.y, z -= _f3.z);
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
	* operator*: Multiplication operator
	* @author: Jc Fowles
	* @Parameter: const float _f: right hand side of the operator
	* @return: float3: The result of the Multiplication
	********************/
	float3 operator* (const float _f) const
	{
		return float3(_f * x, _f * y, _f * z);
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
	* @return: float3: The normalized float3 vector 
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

	/***********************
	* Limit: limit the magnitude of a vector
	* @author: Jc Fowles
	* @return: float3: The limit float3 vector
	********************/
	float3 Limit(float _fLimit)
	{
		float mag = this->Magnitude();
		if (mag > _fLimit)
		{
			(*this) = this->Normalise() * _fLimit;
		}
		return (*this);
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

//Functions

/***********************
* StringToStruct: Copies characters from a char array into a the struct property
* @author: Jc Fowles
* @parameter: _cText: Char array to save into the struct
* @parameter: _pcStruct: Struct property for char array to be saved in
* @parameter: _uiMaxLength: Maximum allowed length of the char array to copy
* @return: bool: Successful copy
********************/
inline bool StringToStruct(const char* _cText, UINT _uiMaxLength, char* _pcStruct)
{
	//Prevent buffer overrun, when copying directly to memory
	if ((strlen(_cText) + 1) <= (_uiMaxLength))
	{
		// Copy the characters into the struct
		strcpy_s(_pcStruct, (strlen(_cText) + 1), _cText);
	}
	else
	{
		//_cText is too long, buffer overrun would occur so return false
		return false;
	}
	return true;
}

/***********************
* WideStringToString: Convert a Wide string to a standard string
* @author: Jc Fowles
* @Parameter: wchar_t * _wstr: Wide string to be converted
* @return: std::string: The converted Wide string as a standard string
********************/
inline std::string WideStringToString(wchar_t* _wstr)
{
	//Convert the Wide String to a standard string
	size_t lengthWstr = (wcslen(_wstr) + 1);
	size_t convertedCount = 0;
	char* cConversion = new char[lengthWstr * 2];
	wcstombs_s(&convertedCount, cConversion, lengthWstr, _wstr, _TRUNCATE);

	std::string strConverted = (std::string)(cConversion);

	//Deallocate memory
	delete cConversion;
	cConversion = 0;

	//Return the converted standard string
	return strConverted;
}


#define VALIDATE(a) if (!a) return (false)



#endif //__APP_DEFINES_H__


