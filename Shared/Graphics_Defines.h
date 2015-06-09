//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Graphics_Defines.h 
// Description : Header file containing Graphical Defines, Structures, Enum, ect
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once

#if !defined(__GRAPHICS_DEFINES_H__)
#define __GRAPHICS_DEFINES_H__

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

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
	bool bIsTurnedOn;
	D3DLIGHTTYPE eLightType;
	D3DXCOLOR colorAmbient;
	D3DXCOLOR colorDiffuse;
	D3DXCOLOR colorSpecular;

	//Spot and Direction
	D3DXVECTOR3 vecDirection;

	//Spot and Point
	D3DXVECTOR3 vecPosition;
	float fRange;
	float fAttnConstant;
	float fAttnLinear;
	float fAttnExponential;

	//Spot
	float fInnerAngle;
	float fOuterAngle;
	float fFallOff;
};



struct ScalarVertex
{
	float fScalarWidth;		//X-axis
	float fScalarHeight;	//Y-axis	
	float fScalarDepth;		//Z-axis
};

#define VALIDATE(a) if (!a) return (false)

#endif //__GRAPHICS_DEFINES_H__
