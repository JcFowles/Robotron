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

// Local Includes
#include "IRenderer.h"
#include "StaticBuffer.h"
#include "VertexUV.h"

#pragma once
class CTerrain
{
public:

	/***********************
	* CTerrain: Default Constructor for Terrain class
	* @author: Jc Fowles
	* @return:
	********************/
	CTerrain();

	/***********************
	* ~CTerrain: Destructor for Terrain class
	* @author: Jc Fowles
	* @return:
	********************/
	~CTerrain();
		
	//Getters

	//Setters
		
	/***********************
	* SetCenter: Set the Center of the terrain in world space, in relation to the scale values that the image has been scaled by
	* @author: Jc Fowles
	* @Parameter: float3 _f3Center: Value of the positional vector to set as the terrain center
	* @return: void: 
	********************/
	void SetCenter(float3 _f3Center); 

	//Prototypes

	/***********************
	* Initialise: Initialise the Terrain for use
	* @author: Jc Fowles
	* @parameter: IRenderer* _pRenderer: Pointer to the renderer manager
	* @parameter: std::string _strImagePath: File path to texture
	* @parameter: _VertexScalar: Structure holding the scale data for vertices
	* @return: bool: Initialise successfulness
	********************/
	bool Initialise(IRenderer* _pRenderer, std::string _strImagePath, ScalarVertex _VertexScalar);
	
	/***********************
	* Process: Process the Terrain
	* @author: Jc Fowles
	* @author: _fDT: The current Delta Tick
	* @return: void
	********************/
	void Process(float _fDT);

	/***********************
	* Draw: Draws the Terrain
	* @author: Jc Fowles
	* @author: _pRendererManager: Render manager use to draw the Terrain
	* @return: void
	********************/
	void Draw(IRenderer* _pRenderer);

	/***********************
	* GenerateStripIndices: Generate and TriangleStrip indices list
	* @author: Jc Fowles
	* @parameter: _pVecIndices: vector to store the indices
	* @parameter: _uiWidth: Width of the Image
	* @parameter: _uiDepth: Depth of the Image
	* @return: bool: Initialise successfulness
	********************/
	int GenerateStripIndices(std::vector<int>* ppIndices, UINT _uiWidth, UINT _uiDepth);
	
	/***********************
	* CalcWorldMatrix: Calculates the World Matrix for the Terrain
	* @author: Jc Fowles
	* @parameter: _pRenderer: Render manager for the application
	* @return: void
	********************/
	void CalcWorldMatrix(IRenderer* _pRenderer);
	
	// Member Variables
private:
	
	D3DXMATRIX m_matWorld;
	
	ScalarVertex m_VertexScalar;

	int m_iSurfaceID;
	int m_iBufferID;

	float m_fSurfaceWidth;
	float m_fSurfaceDepth;

	float m_fRotPitch;
	float m_fRotYaw;
	float m_fRotRoll;

	float3 m_f3Center;

	int m_iMaterialID;
	int m_iTextureID;
};

#endif //__TERRAIN_H__