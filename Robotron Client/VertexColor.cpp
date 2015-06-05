//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Vectex.cpp 
// Description : Implementation file that holds the functionality of the Vertex Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This Include
#include "VertexColor.h"

/***********************
* CVertex: Defualt Constructor of the Vertex 
* @author: Jc Fowles
* @return: 
********************/
CVertexColor::CVertexColor()
{
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_dwColor = 0;
}

/***********************
* CVertex: Overloaded Constructor of the Vertex
* @author: Jc Fowles
* @parameter: _fX: X position of the vertex
* @parameter: _fY: Y position of the vertex
* @parameter: _fZ: Z position of the vertex
* @parameter: _dwColor: 32-bit Unsigned integer for Color value of the vertex
* @return: 
********************/
CVertexColor::CVertexColor(float _fX, float _fY, float _fZ, DWORD _dwColor)
{
	m_fX = _fX;
	m_fY = _fY;
	m_fZ = _fZ;
	m_dwColor = _dwColor;
}

/***********************
* ~CVertex: Destructor of the Vertex
* @author: Jc Fowles
* @return: 
********************/
CVertexColor::~CVertexColor()
{

}

/***********************
* GetX: Returns the X coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the X coordinate of the position vector as a float
********************/
float CVertexColor::GetPosX() const
{
	return m_fX;
}

/***********************
* GetY: Returns the Y coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the Y coordinate of the position vector as a float
********************/
float CVertexColor::GetPosY() const
{
	return m_fY;
}

/***********************
* GetZ: Returns the Z coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the Z coordinate of the position vector as a float
********************/
float CVertexColor::GetPosZ() const
{
	return m_fZ;
}

/***********************
* GetColor: Returns the color of the position vector
* @author: Jc Fowles
* @return: DWORD: Returns 32-bit Unsigned integer that is the Color value of the vertex
********************/
DWORD CVertexColor::GetColor()
{
	return m_dwColor;
}

/***********************
* SetX: Sets the X coordinate of the position vector
* @author: Jc Fowles
* @parameter: _f: The Float value to be set as the X coordinate
* @return: void
********************/
void CVertexColor::SetPosX(float _f)
{
	m_fX = _f;
}

/***********************
* SetY: Sets the Y coordinate of the position vector
* @author: Jc Fowles
* @parameter: _f: The Float value to be set as the Y coordinate
* @return: void
********************/
void CVertexColor::SetPosY(float _f)
{
	m_fY = _f;
}

/***********************
* SetZ: Sets the Z coordinate of the position vector
* @author: Jc Fowles
* @parameter: _f: The Float value to be set as the Z coordinate
* @return: void
********************/
void CVertexColor::SetPosZ(float _f)
{
	m_fZ = _f;
}

/***********************
* SetColor: Sets the color of the position vector using the passed in Red Green Blue values
* @author: Jc Fowles
* @parameter: _iR: The Red value of the color
* @parameter: _iG: The Green value of the color
* @parameter: _iB: The Blue value of the color
* @return: void
********************/
void CVertexColor::SetColor(int _iR, int _iG, int _iB)
{
	DWORD newColor;
	newColor = D3DCOLOR_XRGB(_iR, _iG, _iB);
	m_dwColor = newColor;
}

/***********************
* SetColor: (Overloaded) Sets the color of the position vector using a passed in color refrence
* @author: Jc Fowles
* @parameter: _Color: 32-bit Unsigned integer that is the Color value to set the vertex color to
* @return: void
********************/
void CVertexColor::SetColor(DWORD _Color)
{
	m_dwColor = _Color;
}

