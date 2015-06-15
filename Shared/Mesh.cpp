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

/***********************
* CMesh: Contructor for Mesh class
* @author: Jc Fowles
* @parameter: _pRenderer: Pointer to the Renderer for this application
* @return:
********************/
CMesh::CMesh(IRenderer* _pRenderManager)
{
	m_pRenderManager = _pRenderManager;
	
	//Set default Primative
	m_ePrimitiveType = IGPT_TRIANGLELIST;
}

/***********************
* ~CMesh: Default Destructor for Mesh class
* @author: Jc Fowles
* @return:
********************/
CMesh::~CMesh()
{

}

/***********************
* SetPrimitiveType: Set the primitive type
* @author: Jc Fowles
* @parameter: _primType: Primitive type to be set as
* @return: void
********************/
void CMesh::SetPrimitiveType(eIGPrimitiveType _primType)
{
	m_ePrimitiveType = _primType;
}

/***********************
* AddVertex: Add a new vertex 
* @author: Jc Fowles
* @parameter: _vert: The new vertex with color to be added
* @return: void
********************/
void CMesh::AddVertex(CVertexColor _vert)
{
	m_vecVertices.push_back(_vert);
}

/***********************
* AddVertex: Overloaded AddVertex to Add a new vertexNormal
* @author: Jc Fowles
* @parameter: _vert: The new vertex with normals to be added
* @return: void
********************/
void CMesh::AddVertex(CVertexNormal _vert)
{
	m_vecVerticesNormal.push_back(_vert);
}

/***********************
* SetIndexList: Set the index list
* @author: Jc Fowles
* @parameter: _vecIndices: The list of indices to be set
* @return: void
********************/
void CMesh::SetIndexList(std::vector<int> _vecIndices)
{
	m_vecIndices = _vecIndices;
}

/***********************
* CreateStaticBuffer: Creates the Static Buffer 
* @author: Jc Fowles
* @return: void
********************/
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

/***********************
* Draw: Draws the Mesh 
* @author: JC Fowles
* @return: void
********************/
void CMesh::Draw()
{
	//RenderManger to render the mesh with its unique buffer ID passed in
	m_pRenderManager->Render(m_iBufferID);
}


