/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Terrain.h
* Description : Contains the header file for Terrain Class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/
#pragma once

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

// Library Includes
#include <vector>
#include <stdlib.h>

// Local Includes
#include "../Shared/IRenderer.h"
#include "../Shared/StaticBuffer.h"
#include "../Shared/VertexColor.h"
#include "../Shared/VertexNormal.h"

#pragma once
class CTerrain
{
public:
	//Creation/Deletion
	CTerrain();
	~CTerrain();

	//General functions
	bool Initialise(IRenderer* _pRenderer, std::string _strImagePath, ScalarVertex _VertexScalar);
	void Process(float _fDT);
	void Draw(IRenderer* _pRenderer);
	
	//Setters
	void SetScaleWidth(float _fWidth);
	void SetScaleHeight(float _fHeight);
	void SetScaleDepth(float _fDepth);
	void SetCenter(float _fX, float _fY, float _fZ);

	//Functionality
	void CalcWorldMatrix(IRenderer* _pRenderer);
	int GenerateStripIndices(std::vector<int>* ppIndices, UINT _uiWidth, UINT _uiDepth);

	// Member Variables
private:
	ScalarVertex m_VertexScalar;

	D3DXMATRIX m_matWorld;

	float m_fCenter;

	int m_iSurfaceID;
	int m_iBufferID;

	float m_fSurfaceWidth;
	float m_fSurfaceDepth;

	float m_fDT;

	float m_fRotPitch;
	float m_fRotYaw;
	float m_fRotRoll;

	float m_fX;
	float m_fY;
	float m_fZ;

	int m_iMaterialID;
};

#endif //__TERRAIN_H__