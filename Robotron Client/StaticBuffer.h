//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : StaticBuffer.h 
// Description : Header for the StaticBuffer class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __STATICBUFFER_H__
#define __STATICBUFFER_H__

// Local Includes
#include "..//Shared//Graphics_Defines.h"
#include "VertexUV.h"

// Constants
const int g_kiInvalidStaticBuffer = -1;

// Prototypes
class CStaticBuffer
{
	// Member Functions
public:

	/***********************
	* CStaticBuffer: Default constructor of the CStaticBuffer class
	* @author: Jc Fowles
	* @return:
	********************/
	CStaticBuffer();

	/***********************
	* ~CStaticBuffer: Default destructor of the CStaticBuffer class
	* @author: Jc Fowles
	* @return:
	********************/
	~CStaticBuffer();
	
	//Getters

	/***********************
	* GetNumPrimitives: returns the number of primitive(points/lines/triangles) Dependant of the primitive type
	* @author: Jc Fowles
	* @return: const int: number of primitive shapes(point/lines/triangles)
	********************/
	int GetNumPrimitives() const ;

	/***********************
	* GetNumIndicies: Returns the number of indices, based on the primitive type
	* @author: Jc Fowles
	* @return: const int: Returns the number of indices
	********************/
	int GetNumIndicies() const;

	/***********************
	* GetD3DPT: returns the D3DPRIMITIVETYPE Dependant on the enum of the same type
	* @author: Jc Fowles
	* @return: D3DPRIMITIVETYPE: 3DPRIMITIVETYPE Dependant on the enum of the same type
	********************/
	D3DPRIMITIVETYPE GetD3DPT();

	/***********************
	* GetID: returns the ID of the Static Buffer
	* @author: Jc Fowles
	* @return: const int: returns the ID of the Static Buffer
	********************/
	int GetID() const{ return m_iID; };

	//Setters

	//Prototypes
	
	/***********************
	* Initialise: Initializes the StaticBuffer
	* @author: Jc Fowles
	* @parameter: _pDevice : Pointer to the DirectX9 device
	* @parameter: _iID : Unique ID for the static buffer
	* @parameter: _VertexType : Long number to define the type of vertex
	* @parameter: _ePrimitiveType : a enum to define to the Primitive type
	* @parameter: _uiTotalVerts : Total number of vertices in the static buffer
	* @parameter: _uiTotalIndices : Total number of indices of different vertices in the static buffer
	* @parameter: _iStride : Size of the vertices in the buffer
	* @parameter: _pVertexData : pointer to the the vertex data
	* @parameter: _eIndexType : enum for the bit size(32 or 64) for the index type
	* @parameter: _pIndices : pointer to data to be assigned to the index buffer
	* @return: bool: If succeeded or not
	********************/
	bool Initialise(IDirect3DDevice9* _pDevice,
					int _iID, 
					VertexType _VertexType,
					eIGPrimitiveType _ePrimitiveType, 
					unsigned int _uiTotalVerts, 
					unsigned int _uiTotalIndices, 
					int _iStride, 
					void* _pVertexData, 
					eIGIndexType _eIndexType,
					void* _pIndices);
		
	/***********************
	* Render: Render the static buffer using the D3D device
	* @author: Jc Fowles
	* @parameter: _pDevice : Pointer to the DirectX9 device
	* @return: void
	********************/
	void Render(IDirect3DDevice9* _pDevice);
	
protected:
	

private:
	
	/***********************
	* CStaticBuffer: Constructor of the StaticBuffer class
	* @author: Jc Fowles
	* @return:
	********************/
	CStaticBuffer(const CStaticBuffer& _kr);

	/***********************
	* operator=: Overloaded assignment operator
	* @author: Jc Fowles
	* @return:
	********************/
	CStaticBuffer& operator=(const CStaticBuffer& _kr);

	// Member Variables
protected:

	IDirect3DVertexBuffer9*	m_pVertexBuffer;
	IDirect3DIndexBuffer9*	m_pIndexBuffer;
	DWORD m_dwFvf;
	eIGPrimitiveType m_ePrimitiveType;
	static int s_iActiveStaticBuffer;
	int m_iID;
	int m_iNumVerts;
	int m_iNumIndices;
	int m_iStride;	
};

#endif //__STATICBUFFER_H__

