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
	//Creation/Deletion
	CMesh(IRenderer* _pRenderer);
	~CMesh();

	//Setter
	void SetPrimitiveType(eIGPrimitiveType _primType);


	void AddVertex(CVertexColor _vert);
	void AddVertex(CVertexNormal _vert);
	void SetIndexList(std::vector<int> _vecIndices);
	
	void CreateStaticBuffer();
	void Draw();

protected:
	//Member Variables
	
	//Renderer Variables
	IRenderer* m_pRenderManager;
	int m_iBufferID;
	std::vector<CVertexColor> m_vecVertices;

	std::vector<CVertexNormal> m_vecVerticesNormal;

	std::vector<int> m_vecIndices;

	eIGPrimitiveType m_ePrimitiveType;
};

#endif // __MESH_H__ 