//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : VertexNormal.h 
// Description : Contains the header file for VertexNormal Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once

// Library Includes
#include <d3dx9.h>

class CVertexNormal
{
public:
	// Member Functions

	CVertexNormal();
	CVertexNormal(float _fPosX, float _fPosY, float _fPosZ,
				  float _fNormX, float _fNormY, float _fNormZ );
	~CVertexNormal();

	//Getters
	float GetPosX() const;
	float GetPosZ() const;
	float GetPosY() const;

	float GetNormX() const;
	float GetNormZ() const;
	float GetNormY() const;
	
	//Setters
	void SetPosition(float _fPosX, float _fPosY, float _fPosZ);
	void SetNormal(float _fNormX, float _fNormY, float _fNormZ);
		

protected:
	// Member Variables
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

	float m_fNormX;
	float m_fNormY;
	float m_fNormZ;
	
};
