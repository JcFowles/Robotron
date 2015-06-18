/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 3DObject.cpp
* Description : Implementation for the functions of the 3DObject class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "C3DObject.h"

UINT C3DObject::s_uiNextID = 0;

C3DObject::C3DObject()
{
	//Initialise Member variables
	m_pRenderManager = 0;
	m_pMesh = 0;
	
	m_iMaterialID = -1; 
	
	m_fSize = 0;
	m_fMaxSpeed = 0; 
	m_fRotation = 0; //Yaw

	m_f3Position = { 0.0f, 0.0f, 0.0f };
	m_f3Direction = { 0.0f, 0.0f, 1.0f };
	m_f3Velocity = { 0.0f, 0.0f, 0.0f };

	m_uiObjectID = s_uiNextID++;
}

C3DObject::~C3DObject()
{
	//delete m_pMesh;
	//m_pMesh = 0;
}

void C3DObject::Initialise(IRenderer* _pRenderManager, int _iMaterialID, CMesh* _pMesh, float3 _f3Pos)
{
	//Initialise member variables
	m_pRenderManager = _pRenderManager;
	m_pMesh = _pMesh;
	m_f3Position = _f3Pos;
	
	m_iMaterialID = _iMaterialID;
	
	m_fSize = m_pMesh->GetSize();
}

void C3DObject::Draw()
{
	//Calculates the World Matrix for this 3D Object
	CalcWorldMatrix();
	//Set the Material for the renderer to the material of this object
	m_pRenderManager->SetMaterial(m_iMaterialID);
	m_pMesh->Draw();
}

void C3DObject::Process(float _fDT)
{
	
}

void C3DObject::CalcWorldMatrix()
{
	//Matrices to make up the World Matrix
	D3DXMATRIX matWorld;
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixIdentity(&matWorld);
	   
	//Create rotational Transform matrix
	D3DXMatrixRotationY(&matWorld, m_fRotation);

	//Create the translation matrix from the objects position coordinates
	D3DXMatrixTranslation(&matTranslation, m_f3Position.x, m_f3Position.y, m_f3Position.z);
	
	//Calculate the world matrix
	matWorld *= matTranslation;
	
	//Set the world matrix of this object
	m_pRenderManager->SetWorldMatrix(matWorld);
}




