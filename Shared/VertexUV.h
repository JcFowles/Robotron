//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : VertexUV.h 
// Description : Contains the header file for VertexUV Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once

// Library Includes
#include <d3dx9.h>

//Local Includes
#include "../Shared/App_Defines.h"

class CVertexUV
{
public:
		
	/***********************
	* CVertexUV: Default constructor for the CVertexUC Class
	* @author: Jc Fowles
	* @return: : 
	********************/
	CVertexUV();
	
	/***********************
	* CVertexUV: Overloaded constructor for the CVertexUC Class
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: Position Vector
	* @Parameter: float3 _f3Norm: Normal Vector
	* @Parameter: float2 _f2TexCoord: UV Texture Coords
	* @return: : 
	********************/
	CVertexUV(float3 _f3Pos, float3 _f3Norm, float2 _f2TexCoord);
	
	/***********************
	* ~CVertexUV: Default destructor for the CVertexUC Class
	* @author: Jc Fowles
	* @return: : 
	********************/
	~CVertexUV();

	//Getters
	
	/***********************
	* GetPos: Return the position vector
	* @author: Jc Fowles
	* @return: float3: The position vector
	********************/
	float3 GetPos() const { return m_f3Position; };

	/***********************
	* GetNorm: Return the Normal vector
	* @author: Jc Fowles
	* @return: float3: The Normal vector
	********************/
	float3 GetNorm() const { return m_f3Normals; };

	/***********************
	* GetTexCoord: Return the Texture UV Coords
	* @author: Jc Fowles
	* @return: float3: The Texture UV Coords
	********************/
	float2 GetTexCoord() const { return m_f2TextureCoords; }

	//Setters
	
	/***********************
	* SetPosition: Set the position vector
	* @author: Jc Fowles
	* @Parameter: float3 _f3Pos: Value position vector to be set to
	* @return: void: 
	********************/
	void SetPosition(float3 _f3Pos) { m_f3Position = _f3Pos; };

	/***********************
	* SetNormal: Set the Normal vector
	* @author: Jc Fowles
	* @Parameter: float3 _f3Norm: Value Normal vector to be set to
	* @return: void:
	********************/
	void SetNormal(float3 _f3Norm) { m_f3Normals = _f3Norm; };

	/***********************
	* SetTexCoord: Set the texture Coords
	* @author: Jc Fowles
	* @Parameter: float2 _f2TextCoord: Value texture Coords to be set to
	* @return: void: 
	********************/
	void SetTexCoord(float2 _f2TextCoord) { m_f2TextureCoords = _f2TextCoord; };

	// Member Variables
private:
		
	float3 m_f3Position;
	float3 m_f3Normals;
	float2 m_f2TextureCoords;

};
