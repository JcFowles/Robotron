/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Mesh.h
* Description : Header file for the Mesh class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __MESH_H__
#define __MESH_H__

// Local Includes
#include "IRenderer.h"
#include "VertexColor.h"
#include "VertexNormal.h"

class CMesh
{
public:
	/***********************
	* CMesh: Constructor for Mesh class
	* @author: Jc Fowles
	* @parameter: _pRenderer: Pointer to the Renderer for this application
	* @return:
	********************/
	CMesh(IRenderer* _pRenderer, float _fSize);
	
	/***********************
	* ~CMesh: Default Destructor for Mesh class
	* @author: Jc Fowles
	* @return:
	********************/
	~CMesh();

	/***********************
	* Draw: Draws the Mesh
	* @author: JC Fowles
	* @return: void
	********************/
	void Draw();

	/***********************
	* AddVertex: Add a new vertex
	* @author: Jc Fowles
	* @parameter: _vert: The new vertex with normals to be added
	* @return: void
	********************/
	void AddVertex(CVertexNormal _vert);

	/***********************
	* CreateStaticBuffer: Creates the Static Buffer
	* @author: Jc Fowles
	* @return: void
	********************/
	void CreateStaticBuffer();

	//Setter

	/***********************
	* SetPrimitiveType: Set the primitive type
	* @author: Jc Fowles
	* @parameter: _primType: Primitive type to be set as
	* @return: void
	********************/
	void SetPrimitiveType(eIGPrimitiveType _primType) { m_ePrimitiveType = _primType; };
	
	/***********************
	* SetIndexList: Set the index list
	* @author: Jc Fowles
	* @parameter: _vecIndices: The list of indices to be set
	* @return: void
	********************/
	void SetIndexList(std::vector<int> _vecIndices) { m_vecIndices = _vecIndices; };
	
	//Getter
		
	/***********************
	* GetSize: Return the size of the mesh
	* @author: Jc Fowles
	* @return: float: The mesh size
	********************/
	float GetSize() { return m_fSize; };

	//Member Variables
private:
	
	float m_fSize;

	//Renderer Variables
	IRenderer* m_pRenderManager;
	int m_iBufferID;
	std::vector<CVertexColor> m_vecVertices;

	std::vector<CVertexNormal> m_vecVerticesNormal;

	std::vector<int> m_vecIndices;

	eIGPrimitiveType m_ePrimitiveType;
};

#endif // __MESH_H__ 