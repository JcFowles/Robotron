/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : PlayerObj.cpp
* Description : Implementation for the functions of the Player Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "PlayerObj.h"


CPlayerObj::CPlayerObj(IRenderer* _pRenderManager)
{
	m_f3UpVector = { 0.0f, 1.0f, 0.0f };
		
	m_bIsAlive = true;

	m_uiMaxHealth = 100;
	m_uiHealth = m_uiMaxHealth;
	m_uiScore = 0;
	
	m_fLightRange = 0.5;

	//Create light
	D3DLightParameter PointLightParam;
	PointLightParam.eLightType = D3DLIGHT_POINT;
	PointLightParam.colorDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	PointLightParam.colorAmbient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	PointLightParam.colorSpecular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	PointLightParam.vecPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PointLightParam.fRange = m_fLightRange;
	PointLightParam.fAttnLinear = 0.01f;
	
	m_uiLightID = _pRenderManager->CreateLights(PointLightParam);
	int c = 9;

}

CPlayerObj::~CPlayerObj()
{

}
