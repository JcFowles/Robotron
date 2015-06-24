//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Graphics_Defines.h 
// Description : Header file containing Graphical Defines, Structures, Enum, etc
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once

#ifndef __GRAPHICS_DEFINES_H__
#define __GRAPHICS_DEFINES_H__

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

// Local Includes
#include "../Shared/App_Defines.h"

// Types
typedef long VertexType;

//Enumerators defining primitive types for DirectX9
enum eIGPrimitiveType
{
	INVALID_IGPT,
	IGPT_POINTLIST,
	IGPT_LINELIST,
	IGPT_LINESTRIP,
	IGPT_TRIANGLELIST,
	IGPT_TRIANGLESTRIP,
	IGPT_TRIANGLEFAN
};

//Enumerators defining index types for DirectX9
enum eIGIndexType
{
    IGIT_NONE,
    IGIT_16,
    IGIT_32
};

//Color text Check enums
enum eColorType
{
	COLORTYPE_AMBIENT,
	COLORTYPE_DIFFUSE,
	COLORTYPE_SPECULAR
};

//Text Type enums
enum eTextType
{
	TEXT_TITLE,
	TEXT_MENU,
	TEXT_LIST
	
};

//Light Parameters
struct D3DLightParameter
{
	//All lights 
	int iID;
	bool bIsTurnedOn = false;
	D3DLIGHTTYPE eLightType;
	D3DXCOLOR colorAmbient ;
	D3DXCOLOR colorDiffuse;
	D3DXCOLOR colorSpecular;

	//Spot and Direction
	D3DXVECTOR3 vecDirection;

	//Spot and Point
	D3DXVECTOR3 vecPosition;
	float fRange = 1.0f;
	float fAttnConstant  = 0.0f;
	float fAttnLinear = 0.0f;
	float fAttnExponential = 0.0f;

	//Spot
	float fInnerAngle = 0.0f;
	float fOuterAngle = 0.0f;
	float fFallOff = 0.0f;
};


struct ScalarVertex
{
	float fScalarWidth;		//X-axis
	float fScalarHeight;	//Y-axis
	float fScalarDepth;		//Z-axis
};

struct MaterialValues
{
	float4 f4Ambient;
	float4 f4Specular;
	float4 f4Diffuse;
	float4 f4Emissive;

	float fPower;
};



#endif //__GRAPHICS_DEFINES_H__
