/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : CCameraStatic.cpp
* Description : Contains the main functionality of camera Class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "Camera.h"

CCamera::CCamera()
{
	m_vecPosition = { 0, 50, 0 };
	m_vecTarget = { 0, 0, 0 };

	m_bFirstPerson = true;
}

CCamera::~CCamera()
{
}

void CCamera::SetCamera(D3DXVECTOR3 _vecTarget, D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecUp)
{
	if (m_bFirstPerson)
		//First person view
	{
		//look where the avatar is looking, look at the same target
		m_vecTarget = _vecPosition;
		m_vecTarget.z = m_vecTarget.z + 5.0f;
		//Position with in the avatar
		m_vecPosition = _vecPosition;
		//Which way is up for the avatar
		m_VecUp = _vecUp;
	}
	else
		//Top down view 
	{
		//set target to be the avatar
		m_vecTarget = _vecPosition;

		//Set position above player
		m_vecPosition = _vecPosition;
		D3DXVECTOR3 AddHeight = { 0.0f, 50.0f, 0.0f };
		m_vecPosition += AddHeight;

		//Which way is up for the avatar
		m_VecUp = { 0.0f, 0.0f, 1.0f };
	}

}

bool CCamera::Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecTarget, bool _bFirstPerson)
{
	m_vecPosition = _vecPosition;
	m_vecTarget = _vecTarget;
	m_bFirstPerson = _bFirstPerson;

	//Initialise Up vector on z axis
	m_VecUp = { 0.0f, 0.0f, 1.0f };
	
	return true;
}

void CCamera::Process(IRenderer* _pRenderManager)
{
	//CS: Viewport
	_pRenderManager->CreateViewMatrix(m_vecPosition, m_vecTarget, m_VecUp);
}

void CCamera::ToggleType()
{
	m_bFirstPerson = !m_bFirstPerson;
}



