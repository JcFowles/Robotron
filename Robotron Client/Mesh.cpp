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

CMesh::CMesh(IRenderer* _pRenderManager, float _fSize, int _iTextureID)
{
	m_pRenderManager = _pRenderManager;
	m_fSize = _fSize;

	//Set default Primitive type to triangle list
	m_ePrimitiveType = IGPT_TRIANGLELIST;
	
	m_iTextureID = _iTextureID;
}

CMesh::~CMesh()
{

}

void CMesh::AddVertex(CVertexUV _vert)
{
	m_vecVerticesUV.push_back(_vert);
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
		
	//Create the static Buffer on the device
	m_iBufferID = m_pRenderManager->CreateStaticBuffer( sizeof(CVertexUV),
															m_ePrimitiveType,
															m_vecVerticesUV.size(),
															m_vecIndices.size(),
															sizeof(CVertexUV),
															&m_vecVerticesUV,
															indexType,
															&m_vecIndices);
}

void CMesh::Draw()
{
	//Set texture to be used
	m_pRenderManager->SetTexture(m_iTextureID, 0);
	//RenderManger to render the mesh with its unique buffer ID 
	m_pRenderManager->Render(m_iBufferID);
}



