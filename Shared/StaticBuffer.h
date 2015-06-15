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

#if !defined(__STATICBUFFER_H__)
#define __STATICBUFFER_H__

// Local Includes
#include "Graphics_Defines.h"
#include "VertexColor.h"
#include "VertexNormal.h"

// Constants
const int g_kiInvalidStaticBuffer = -1;

// Prototypes
class CStaticBuffer
{
	// Member Functions
public:
	//Creation/Deletion
	CStaticBuffer();
	~CStaticBuffer();
	
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

	//Getters
	int GetNumPrimitives() const;
	int GetNumIndicies() const;
	int GetID() const;
	
	//Funtionality
	void Render(IDirect3DDevice9* _pDevice);
	
protected:
	//Protected Getters
	DWORD GetD3DFvf();
	D3DPRIMITIVETYPE GetD3DPT();

private:
	//Dissallow copyies
	CStaticBuffer(const CStaticBuffer& _kr);
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

