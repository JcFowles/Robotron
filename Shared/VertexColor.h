//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : VertexColor.h 
// Description : Contains the header file for VertexColor Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once

#ifndef __VERTEX_COLOR_H__
#define __VERTEX_COLOR_H__

// Library Includes
#include <d3dx9.h>

class CVertexColor
{
public:
	// Member Functions


	CVertexColor();
	CVertexColor(float _fX, float _fY, float _fZ, DWORD _dwColor);
	~CVertexColor();

	//Getters
	float GetPosX() const;
	float GetPosZ() const;
	float GetPosY() const;
	DWORD GetColor();

	//Setters
	void SetPosX(float _f);
	void SetPosY(float _f);
	void SetPosZ(float _f);
	void SetColor(int _iR, int _iG, int _iB);
	void SetColor(DWORD _dwColor);
	
protected:
	// Member Variables
	float m_fX;
	float m_fY;
	float m_fZ;

	DWORD m_dwColor;
};

#endif //__VERTEX_COLOR_H__