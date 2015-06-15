// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2005 - 2015 Media Design School
// 
// File Name : Mesh.cpp
// Description : Implementation file that holds the functionality of the Mesh class
// Author :	Jc Fowles
// Mail :	Jc.Fowles@mediadesign.school.nz
// 

// Local Includes
#include "Mesh.h"


CMesh::CMesh(IRenderer* _pRenderManager, float _fSize)
{
	m_pRenderManager = _pRenderManager;
	m_fSize = _fSize;

	//Set default Primitive type to triangle list
	m_ePrimitiveType = IGPT_TRIANGLELIST;
}

CMesh::~CMesh()
{

}

void CMesh::AddVertex(CVertexNormal _vert)
{
	m_vecVerticesNormal.push_back(_vert);
};

void CMesh::CreateStaticBuffer()
{
	eIGIndexType indexType;
	if (m_vecIndices.size() == 0)
	{
		//No index buffer, set index type to NONE
		indexType = IGIT_NONE;
	}
	else
	{
		//Dependant on the memory size of the Index, select the index type
		indexType = ((sizeof(m_vecIndices.front())) >= 4) ? IGIT_32 : IGIT_16;
	}
		
	if ((m_vecVertices.size() != 0) && (m_vecVerticesNormal.size() == 0))
	//Using vertices with color
	{
		//Create the static Buffer on the device
		m_iBufferID = m_pRenderManager->CreateStaticBuffer(sizeof(CVertexColor),
															m_ePrimitiveType,
															m_vecVertices.size(),
															m_vecIndices.size(),
															sizeof(CVertexColor),
															&m_vecVertices,
															indexType,
															&m_vecIndices);
	}
	else if ((m_vecVertices.size() == 0) && (m_vecVerticesNormal.size() != 0))
	//Using vertices with normal
	{
		//Create the static Buffer on the device
		m_iBufferID = m_pRenderManager->CreateStaticBuffer( sizeof(CVertexNormal),
															m_ePrimitiveType,
															m_vecVerticesNormal.size(),
															m_vecIndices.size(),
															sizeof(CVertexNormal),
															&m_vecVerticesNormal,
															indexType,
															&m_vecIndices);
	}

}

void CMesh::Draw()
{
	//RenderManger to render the mesh with its unique buffer ID passed in
	m_pRenderManager->Render(m_iBufferID);
}



