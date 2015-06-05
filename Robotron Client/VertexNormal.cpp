//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : VertexNormal.cpp 
// Description : Implementation file that holds the functionality of the Vertex Normal Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This Include
#include "VertexNormal.h"

/***********************
* CVertexNormal: Defualt Constructor of the VertexNormal
* @author: Jc Fowles
* @return:
********************/
CVertexNormal::CVertexNormal()
{
	m_fPosX = 0.0f; 
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;

	m_fNormX = 0.0f;
	m_fNormY = 0.0f;
	m_fNormZ = 0.0f;

}

/***********************
* CVertexNormal: Overloaded Constructor of the VertexNormal
* @author: Jc Fowles
* @parameter: _fPosX: X position of the vertex
* @parameter: _fPosY: Y position of the vertex
* @parameter: _fPosZ: Z position of the vertex
* @parameter: _fNormX: X Normal of the vertex
* @parameter: _fNormY: Y Normal of the vertex
* @parameter: _fNormZ: Z Normal of the vertex
* @return:
********************/
CVertexNormal::CVertexNormal(float _fPosX, float _fPosY, float _fPosZ, 
							 float _fNormX, float _fNormY, float _fNormZ)
{
	m_fPosX = _fPosX;
	m_fPosY = _fPosY;
	m_fPosZ = _fPosZ;

	m_fNormX = _fNormX;
	m_fNormY = _fNormY;
	m_fNormZ = _fNormZ;
}

/***********************
* ~CVertexNormal: Destructor of the VertexNormal
* @author: Jc Fowles
* @return:
********************/
CVertexNormal::~CVertexNormal()
{

}

/***********************
* GetPosX: Returns the X coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the X coordinate of the position vector as a float
********************/
float CVertexNormal::GetPosX() const
{
	return m_fPosX;
}

/***********************
* GetPosY: Returns the Y coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the Y coordinate of the position vector as a float
********************/
float CVertexNormal::GetPosY() const
{
	return m_fPosY;
}

/***********************
* GetPosZ: Returns the Z coordinate of the position vector
* @author: Jc Fowles
* @return: float: Returns the Z coordinate of the position vector as a float
********************/
float CVertexNormal::GetPosZ() const
{
	return m_fPosZ;
}

/***********************
* GetNormX: Returns the X coordinate of the normal vector
* @author: Jc Fowles
* @return: float: Returns the X coordinate of the normal vector as a float
********************/
float CVertexNormal::GetNormX() const
{
	return m_fNormX;
}

/***********************
* GetNormY: Returns the Y coordinate of the normal vector
* @author: Jc Fowles
* @return: float: Returns the Y coordinate of the position vector as a float
********************/
float CVertexNormal::GetNormY() const
{
	return m_fNormY;
}

/***********************
* GetNormZ: Returns the Z coordinate of the normal vector
* @author: Jc Fowles
* @return: float: Returns the Z coordinate of the normal vector as a float
********************/
float CVertexNormal::GetNormZ() const
{
	return m_fNormZ;
}

/***********************
* SetPosition: Sets the coordinates of the position vector
* @author: Jc Fowles
* @parameter: _fPosX: The Float value to be set as the X coordinate
* @parameter: _fPosY: The Float value to be set as the Y coordinate
* @parameter: _fPosZ: The Float value to be set as the Z coordinate
* @return: void
********************/
void CVertexNormal::SetPosition(float _fPosX, float _fPosY, float _fPosZ)
{
	m_fPosX = _fPosX;
	m_fPosY = _fPosY;
	m_fPosZ = _fPosZ;
}

/***********************
* SetPosition: Sets the coordinates of the position vector
* @author: Jc Fowles
* @parameter: _fNormX: The Float value to be set as the X of the Normal vector
* @parameter: _fNormY: The Float value to be set as the Y of the Normal vector
* @parameter: _fNormZ: The Float value to be set as the Z of the Normal vector
* @return: void
********************/
void CVertexNormal::SetNormal(float _fNormX, float _fNormY, float _fNormZ)
{
	m_fNormX = _fNormX;
	m_fNormY = _fNormY;
	m_fNormZ = _fNormZ;
}


