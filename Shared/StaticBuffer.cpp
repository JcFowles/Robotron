//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : StaticBuffer.cpp 
// Description : Implementation file that holds the functionality of the StaticBuffer class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "StaticBuffer.h"

/***********************
* CStaticBuffer: Defualt constructor of the CStaticBuffer class
* @author: Jc Fowles
* @return: 
********************/
CStaticBuffer::CStaticBuffer(void)
{

}

/***********************
* ~CStaticBuffer: Defualt destructor of the CStaticBuffer class
* @author: Jc Fowles
* @return: 
********************/
CStaticBuffer::~CStaticBuffer(void)
{
	//release the Vertex buffer if one exists
	if (m_pVertexBuffer != 0)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	//Release the index buffer if one exists
	if(m_pIndexBuffer != 0)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
}

/***********************
* Initialise: Initialises the StaticBuffer
* @author: Jc Fowles
* @parameter: _pDevice : Pointer to the DirectX9 device
* @parameter: _iID : Unique ID for the static buffer
* @parameter: _VertexType : Long number to define the type of vertex 
* @parameter: _ePrimitiveType : a enum to define to the Primative type 
* @parameter: _uiTotalVerts : Total number of vertices in the static buffer
* @parameter: _uiTotalIndices : Total number of indices of different verts in the static buffer
* @parameter: _iStride : Size of the vertices in the buffer
* @parameter: _pVertexData : pointer to the the vertex data
* @parameter: _eIndexType : enum for the bit size(32 or 64) for the index type
* @parameter: _pIndices : pointer to data to be assigned to the index buffer
* @return: bool: If succeded or not
********************/
bool CStaticBuffer::Initialise(IDirect3DDevice9* _pDevice, 
							   int _iID, 
							   VertexType _VertexType, 
							   eIGPrimitiveType _ePrimitiveType, 
							   unsigned int _uiTotalVerts, 
							   unsigned int _uiTotalIndices, 
							   int _iStride, 
							   void* _pVertexData, 
							   eIGIndexType _eIndexType, 
							   void* _pIndices)
{
	//Check parameters are valid
	if (_uiTotalVerts == 0 || _pVertexData == 0)
	{
		return false;
	}

	//Initialize member variables
	//Populate member data from parameters
	m_ePrimitiveType = _ePrimitiveType;
	m_iNumVerts = _uiTotalVerts;
	m_iNumIndices = _uiTotalIndices;
	m_iStride = _iStride;
	m_iID = _iID;
	_pDevice->GetFVF(&m_dwFvf);
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	
	//Creates a void pointer for locking memory
	VOID* pVoidVertex;
	VOID* pVoidIndex;

	//Create vertex buffer
	HRESULT hr = _pDevice->CreateVertexBuffer((m_iNumVerts * (m_iStride)), 0, m_dwFvf, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	
	//Based on vertex Type cast _pVertexData to that type
	switch (_VertexType)
	{
		case (sizeof(CVertexColor)) :
		{
			//Convert vertexData to type CVertex
			std::vector<CVertexColor>* pVertices = static_cast<std::vector<CVertexColor>*>(_pVertexData);

			//Add the vertices to the vertex buffer
			//Locks Memory from being moved or used by another process
			m_pVertexBuffer->Lock(0, (_uiTotalVerts * _iStride), (void**)&pVoidVertex, 0);
			memcpy(pVoidVertex, &pVertices->front(), (_uiTotalVerts * _iStride));
			//Unlocks the locked memory for general use again
			m_pVertexBuffer->Unlock();
		}
		break;
		case (sizeof(CVertexNormal)) :
		{
			//Convert vertexData to type CVertexNormal
			std::vector<CVertexNormal>* pVertices = static_cast<std::vector<CVertexNormal>*>(_pVertexData);

			//Add the vertices to the vertex buffer
			//Locks Memory from being moved or used by another process
			m_pVertexBuffer->Lock(0, (_uiTotalVerts * _iStride), (void**)&pVoidVertex, 0);
			memcpy(pVoidVertex, &pVertices->front(), (_uiTotalVerts * _iStride));
			//Unlocks the locked memory for general use again
			m_pVertexBuffer->Unlock();
		
		}
		break;
	}

	
	//Create the Index Buffer
	D3DFORMAT D3DIndexFormat;
	switch (_eIndexType)
	{
		//Create Index buffer if one is specified
		case IGIT_16:
		{
			D3DIndexFormat = D3DFMT_INDEX16;
		}
		break;
		case IGIT_32:
		{
			D3DIndexFormat = D3DFMT_INDEX32;
		}
		break;
		case IGIT_NONE:
		{
			//If no Index buffer is specified set pointer to null pointer
			m_pIndexBuffer = 0;
		}
		default:
		{
			D3DIndexFormat = D3DFMT_UNKNOWN;
		}
		break;
	}

	//If a index buffer exists		
	if (_eIndexType != IGIT_NONE)
	{
		//Create the  Index buffer
		_pDevice->CreateIndexBuffer((_uiTotalIndices * sizeof(int)), 0, D3DIndexFormat, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);
				
		//Cast void poinet _pIndices to vector of ints
		std::vector<int>* pIndices = static_cast<std::vector<int>*>(_pIndices);
		//Add the indices to the index buffer
		//Locks Memory from being moved or used by another process
		m_pIndexBuffer->Lock(0, (_uiTotalIndices * sizeof(int)), (void**)&pVoidIndex, 0);	
		memcpy(pVoidIndex, &pIndices->front(), (_uiTotalIndices * sizeof(int)));
		//Unlocks the locked memory for general use again
		m_pIndexBuffer->Unlock();	
	
	}
	
	//If Vertex buffer wasn't created return Initialise false
	if (m_pVertexBuffer == 0)
	{
		return false;
	}

	//Set set the stream source to 0, and set the vertex buffer to m_pVertexBuffer
	_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, m_iStride);

	//Set the FVF
	_pDevice->SetFVF(m_dwFvf);

	//Set the active static buffer to this buffer ID
	s_iActiveStaticBuffer = m_iID;

	
	//Return Initialise true
	return true;
}

/***********************
* Render: Render the static buffer using the D3D device
* @author: Jc Fowles
* @parameter: _pDevice : Pointer to the DirectX9 device
* @return: void
********************/
void CStaticBuffer::Render(IDirect3DDevice9* _pDevice)
{

	if (s_iActiveStaticBuffer != m_iID)
	{
		//Set set the stream source to 0, and set the vertex buffer to m_pVertexBuffer
		_pDevice->SetStreamSource(0, m_pVertexBuffer,0 , m_iStride );

		//Set the FVF
		_pDevice->SetFVF(m_dwFvf);
		
		//Set the active static buffer to this buffer ID
		s_iActiveStaticBuffer = m_iID;			
	}

	if(m_pIndexBuffer != 0)
	{
		//If using a Index buffer
		//Set the Indices to the Index buffer
		_pDevice->SetIndices(m_pIndexBuffer);
		//Draw the index primitives
		
		_pDevice->DrawIndexedPrimitive(GetD3DPT(), 0, 0, m_iNumVerts, 0, GetNumIndicies());
	}
	else
	{
		//If using a vertex buffer draw all the primitives
		_pDevice->DrawPrimitive(GetD3DPT(), 0, GetNumPrimitives());
	}
}

/***********************
* GetNumPrimitives: returns the number of primitve(points/lines/triangles) dependant of the primative type
* @author: Jc Fowles
* @return: const int: number of primative shapes(point/lines/triangles)
********************/
int CStaticBuffer::GetNumPrimitives() const
{
	//Dependant on the primitive returns the number of primative shapes(point/lines/triangles)
	switch (m_ePrimitiveType)
	{
		case IGPT_POINTLIST:
		{
			return (m_iNumVerts);
		}
		break;
		case IGPT_LINELIST:
		{
			return (m_iNumVerts/2);
		}
		break;
		case IGPT_LINESTRIP:
		{
			return (m_iNumVerts - 1);
		}
		break;
		case IGPT_TRIANGLEFAN:
		{
			return (m_iNumVerts - 2);
		}
		break;
		case IGPT_TRIANGLELIST:
		{
			return (m_iNumVerts/3);
		}
		break;
		case IGPT_TRIANGLESTRIP:
		{
			return (m_iNumVerts - 2);
		}
		break;
		case INVALID_IGPT:
		//Fall through
		default:
		{
			return (0);
		}
		break;
	}

}

/***********************
* GetNumIndicies: Returns the number of indicis, based on the primative type
* @author: Jc Fowles
* @return: const int: Returns the number of indicis
********************/
int CStaticBuffer::GetNumIndicies() const
{
	//Dependant on the primitive returns the number of primative shapes(point/lines/triangles)
	switch (m_ePrimitiveType)
	{
		case IGPT_POINTLIST:
		{
			return (m_iNumIndices);
		}
		break;
		case IGPT_LINELIST:
		{
			return (m_iNumIndices / 2);
		}
		break;
		case IGPT_LINESTRIP:
		{
			return (m_iNumIndices - 1);
		}
		break;
		case IGPT_TRIANGLEFAN:
		{
			return (m_iNumIndices - 2);
		}
		break;
		case IGPT_TRIANGLELIST:
		{
			return (m_iNumIndices / 3);
		}
		break;
		case IGPT_TRIANGLESTRIP:
		{
			return (m_iNumIndices - 2);
		}
		break;
		case INVALID_IGPT:
		//Fall through
		default:
		{
			return (0);
		}
		break;
	}

}

/***********************
* GetID: returns the ID of the Static Buffer
* @author: Jc Fowles
* @return: const int: returns the ID of the Static Buffer
********************/
int CStaticBuffer::GetID() const
{
	return m_iID;
}

/***********************
* GetD3DFvf: returns the D3D FVF
* @author: Jc Fowles
* @return: DWORD: The D3D FVF of the device
********************/
DWORD CStaticBuffer::GetD3DFvf()
{
	return m_dwFvf;
}

/***********************
* GetD3DPT: returns the D3DPRIMITIVETYPE dependant on the enum of the same type
* @author: Jc Fowles
* @return: D3DPRIMITIVETYPE: 3DPRIMITIVETYPE dependant on the enum of the same type
********************/
D3DPRIMITIVETYPE CStaticBuffer::GetD3DPT()
{
	switch (m_ePrimitiveType)
	{
		case IGPT_POINTLIST:
		{
			return (D3DPT_POINTLIST);
		}
		break;
		case IGPT_LINELIST:
		{
			return (D3DPT_LINELIST);
		}
		break;
		case IGPT_LINESTRIP:
		{
			return (D3DPT_LINESTRIP);
		}
		break;
		case IGPT_TRIANGLEFAN:
		{
			return (D3DPT_TRIANGLEFAN);;
		}
		break;
		case IGPT_TRIANGLELIST:
		{
			return (D3DPT_TRIANGLELIST);
		}
		break;
		case IGPT_TRIANGLESTRIP:
		{
			return (D3DPT_TRIANGLESTRIP);
		}
		break;
		case INVALID_IGPT:
		//Fall through
		default:
		{
			return (D3DPT_FORCE_DWORD);
		}
		break;
	}

}

