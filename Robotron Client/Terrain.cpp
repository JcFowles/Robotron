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

/***********************
* CTerrain: Default Constructor for Terrain class
* @author: Jc Fowles
* @return: 
********************/
CTerrain::CTerrain()
{
	m_fDT = 0;

	m_fRotPitch = 0;
	m_fRotYaw = 0;
	m_fRotRoll = 0;

	m_fX = 0;
	m_fY = 0;
	m_fZ = 0;
}

/***********************
* ~CTerrain: Destructor for Terrain class
* @author: Jc Fowles
* @return:
********************/
CTerrain::~CTerrain()
{
}

/***********************
* Initialise: Initialise the Terrain for use
* @author: Jc Fowles
* @parameter: _pRenderer: Pointer to the renderer manager
* @parameter: _VertexScalar: Structure holding the scale data for vertices
* @return: bool: Initialise successfulness
********************/
bool CTerrain::Initialise(IRenderer* _pRenderer, std::string _strImagePath, ScalarVertex _VertexScalar)
{
	m_VertexScalar = _VertexScalar;

	//Structs for holding Image information
	D3DXIMAGE_INFO imageInfo;
	ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO));

	//Create the surface
	m_iSurfaceID = _pRenderer->CreateOffScreenSurface(_strImagePath, imageInfo);

	//Get the surface info and save them
	m_fSurfaceWidth = (float)imageInfo.Width;
	m_fSurfaceDepth = (float)imageInfo.Height;

	//std::vector<CVertex> pVertices;
	std::vector<CVertexUV> pVertices;
	_pRenderer->RetrieveVertices(&pVertices, m_iSurfaceID, imageInfo, m_VertexScalar);

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
	Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	Material.f4Diffuse = { 0.4f, 0.3f, 0.3f, 1.0f };
	Material.f4Emissive = { 0.4f, 0.3f, 0.3f, 1.0f };
	Material.f4Specular = { 0.4f, 0.3f, 0.3f, 1.0f };
	Material.fPower = { 1.0f };


	m_iMaterialID = _pRenderer->CreateMaterial(Material);

	std::string strFilePath = "Assets\\Heightmap.bmp";

	m_iTextureID = _pRenderer->CreateTexture(strFilePath);

	return true;
}

/***********************
* GenerateStripIndices: Generate and TriangleStrip indices list
* @author: Jc Fowles
* @parameter: _pVecIndices: vector to store the indices
* @parameter: _uiWidth: Width of the Image
* @parameter: _uiDepth: Depth of the Image
* @return: bool: Initialise successfulness
********************/
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

/***********************
* Process: Process the Terrain
* @author: Jc Fowles
* @author: _fDT: The current Delta Tick
* @return: void
********************/
void CTerrain::Process(float _fDT)
{
	//No process required
}

/***********************
* Draw: Draws the Terrain
* @author: Jc Fowles
* @author: _pRendererManager: Render manager use to draw the Terrain
* @return: void
********************/
void CTerrain::Draw(IRenderer* _pRendererManager)
{
	CalcWorldMatrix(_pRendererManager);
	
	_pRendererManager->SetMaterial(m_iMaterialID);
	_pRendererManager->SetTexture(m_iTextureID, 0);
	//Set the material
	_pRendererManager->Render(m_iBufferID);
}

/***********************
* SetScaleWidth: Set the Scale Width
* @author: Jc Fowles
* @author: _fWidth: The new Scale Width for the terrain to be set to
* @return: void
********************/
void CTerrain::SetScaleWidth(float _fWidth)
{
	m_VertexScalar.fScalarWidth = _fWidth;
}

/***********************
* SetScaleHeight: Set the Scale Height
* @author: Jc Fowles
* @author: _fHeight: The new Scale Height for the terrain to be set to
* @return: void
********************/
void CTerrain::SetScaleHeight(float _fHeight)
{
	m_VertexScalar.fScalarHeight = _fHeight;
}

/***********************
* SetScaleDepth: Set the Scale Depth
* @author: Jc Fowles
* @author: _fDepth: The new Scale Depth for the terrain to be set to
* @return: void
********************/
void CTerrain::SetScaleDepth(float _fDepth)
{
	m_VertexScalar.fScalarDepth = _fDepth;
}

/***********************
* SetCenter: Set the Center of the Terrain
* @author: Jc Fowles
* @return: void
********************/
void CTerrain::SetCenter(float _fX, float _fY, float _fZ )
{
	m_fX = _fX - ((m_fSurfaceWidth * m_VertexScalar.fScalarWidth) / 2);
	m_fY = _fY;
	m_fZ = _fZ - ((m_fSurfaceDepth * m_VertexScalar.fScalarDepth) / 2);
}

/***********************
* CalcWorldMatrix: Calculates the World Matrix for the Terrain
* @author: Jc Fowles
* @parameter: _pRenderer: Render manager for the application
* @return: void
********************/
void CTerrain::CalcWorldMatrix(IRenderer* _pRenderManager)
{
	//Matrices to make up the World Matrix, mostly unsused and set to identity
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
	D3DXMatrixTranslation(&matTranslation, m_fX, m_fY, m_fZ);
	m_matWorld = (matRotateAroundX * matRotateAroundY * matRotateAroundZ * matTranslation);

	//Set The World Matrix for this Terrain on the Device
	_pRenderManager->SetWorldMatrix(m_matWorld);
}