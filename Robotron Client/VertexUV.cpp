//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CVertexUV.cpp 
// Description : Implementation file that holds the functionality of the Vertex With UV texture coord Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This Include
#include "VertexUV.h"

CVertexUV::CVertexUV()
{
}

CVertexUV::CVertexUV(float3 _f3Pos, float3 _f3Norm, float2 _f2TexCoord)
{
	m_f3Position = _f3Pos;
	m_f3Normals = _f3Norm;
	m_f2TextureCoords = _f2TexCoord;
}

CVertexUV::~CVertexUV()
{
}
