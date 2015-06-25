//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2005 - 2015 Media Design School
// 
// File Name : Terrain.cpp
// Description :  Implementation file that holds the functionality of the Terrain Class
// Author :	Jc Fowles
// Mail :	Jc.Fowles@mediadesign.school.nz
//

// Local Include
#include "Terrain.h"

CTerrain::CTerrain()
{
	m_fRotPitch = 0;
	m_fRotYaw = 0;
	m_fRotRoll = 0;

	m_f3Center = { 0, 0, 0 };
}

CTerrain::~CTerrain()
{
}

void CTerrain::SetCenter(float3 _f3Center)
{
	m_f3Center.x = _f3Center.x - ((m_fSurfaceWidth * m_VertexScalar.fScalarWidth) / 2);
	m_f3Center.y = _f3Center.y;
	m_f3Center.z = _f3Center.z - ((m_fSurfaceDepth * m_VertexScalar.fScalarDepth) / 2);
}

bool CTerrain::Initialise(IRenderer* _pRenderer, std::string _strHeightMapPath, std::string _strTexturePath, ScalarVertex _VertexScalar, int _iTiled)
{
	m_VertexScalar = _VertexScalar;

	//Structs for holding Image information
	D3DXIMAGE_INFO imageInfo;
	ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO));

	//Create the surface
	m_iSurfaceID = _pRenderer->CreateOffScreenSurface(_strHeightMapPath, imageInfo);

	//Get the surface info and save them
	m_fSurfaceWidth = (float)imageInfo.Width;
	m_fSurfaceDepth = (float)imageInfo.Height;

	//std::vector<CVertex> pVertices;
	std::vector<CVertexUV> pVertices;
	_pRenderer->RetrieveVertices(&pVertices, m_iSurfaceID, imageInfo, _VertexScalar, _iTiled);

	std::vector<int> pIndices;
	int iIndicesNum = GenerateStripIndices(&pIndices, imageInfo.Width, imageInfo.Height);

	//Set the Index Format
	eIGIndexType indexType = (sizeof(pIndices.front()) >= 4) ? IGIT_32 : IGIT_16;

	m_iBufferID = _pRenderer->CreateStaticBuffer(sizeof(CVertexUV),
											     IGPT_TRIANGLESTRIP,
											     (&pVertices)->size(),
											     iIndicesNum,
												 sizeof(CVertexUV),
											     (&pVertices),
											     indexType,
											     &pIndices);

	//Set up and create the material of the terrain
	MaterialValues Material;
	Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower = { 0.0f };
	
	m_iMaterialID = _pRenderer->CreateMaterial(Material);

	//Set up the Texture for the terrain 

	m_iTextureID = _pRenderer->CreateTexture(_strTexturePath);

	return true;
}

void CTerrain::Process(float _fDT)
{
	//No process required
}

void CTerrain::Draw(IRenderer* _pRendererManager)
{
	CalcWorldMatrix(_pRendererManager);
	
	//Set the material
	_pRendererManager->SetMaterial(m_iMaterialID);
	//Set the Texture
	_pRendererManager->SetTexture(m_iTextureID, 0);
	
	_pRendererManager->Render(m_iBufferID);
}

int CTerrain::GenerateStripIndices(std::vector<int>* _pVecIndices, UINT _uiWidth, UINT _uiDepth)
{
	//Go through all rows except last one
	for (unsigned int iRow = 0; iRow < _uiDepth - 1; iRow++)
	{
		if (iRow % 2 == 0)	
		//Even Row
		{
			//Even Rows go towards the Left for Clockwise winding order
			for (int iCol = (int)(_uiWidth - 1); iCol >= 0; iCol--)
			{
				_pVecIndices->push_back(iCol + (iRow * _uiWidth));
				_pVecIndices->push_back(iCol + ((iRow + 1) * _uiWidth));
			}

			//Add Degenerate triangle at end of each row
			if (iRow != _uiDepth - 2)
			{
				_pVecIndices->push_back(0 + ((iRow + 1) * _uiWidth));
			}
		}
		else
		//Odd Rows
		{
			//Odd Rows go towards the Right for Clockwise winding order
			for (unsigned int iCol = 0; iCol < _uiWidth; iCol++)
			{
				_pVecIndices->push_back(iCol + (iRow * _uiWidth));
				_pVecIndices->push_back(iCol + ((iRow + 1) * _uiWidth));
			}

			//Add Degenerate triangle at end of each row
			if (iRow != _uiDepth - 2)
			{
				_pVecIndices->push_back((_uiWidth - 1) + ((iRow + 1)  * _uiWidth));
			}
		}
	}
	
	//Return the Number of Indices created
	return ((int)_pVecIndices->size());
}

void CTerrain::CalcWorldMatrix(IRenderer* _pRenderManager)
{
	//Matrices to make up the World Matrix, mostly unused and set to identity
	D3DXMATRIX matRotateAroundX;
	D3DXMatrixIdentity(&matRotateAroundX);
	D3DXMATRIX matRotateAroundY;
	D3DXMatrixIdentity(&matRotateAroundY);
	D3DXMATRIX matRotateAroundZ;
	D3DXMatrixIdentity(&matRotateAroundZ);
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	//Reset World matrix
	D3DXMatrixIdentity(&m_matWorld);

	//Create Rotational Matrices for the terrain based the Yaw/Pitch/Roll values
	D3DXMatrixRotationX(&matRotateAroundX, m_fRotPitch);
	D3DXMatrixRotationY(&matRotateAroundY, m_fRotYaw);
	D3DXMatrixRotationZ(&matRotateAroundZ, m_fRotRoll);

	//Create the Translation Matrix from the terrain Position coordinates, which should never change
	D3DXMatrixTranslation(&matTranslation, m_f3Center.x, m_f3Center.y, m_f3Center.z);
	m_matWorld = (matRotateAroundX * matRotateAroundY * matRotateAroundZ * matTranslation);

	//Set The World Matrix for this Terrain on the Device
	_pRenderManager->SetWorldMatrix(m_matWorld);
}