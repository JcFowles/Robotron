/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : CCameraStatic.cpp
* Description : Contains the main functionality of Static camera Class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "CameraStatic.h"

CCameraStatic::CCameraStatic()
{
	m_vecPosition = { 0, 0, 0 };
	m_vecTarget = { 0, 0, 0 };

	m_bFirstPerson = true;
}

CCameraStatic::~CCameraStatic()
{
}

void CCameraStatic::SetCamera(D3DXVECTOR3 _vecTarget, D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecUp)
{
	if (m_bFirstPerson)
		//First person view
	{
		//look where the avatar is looking, look at the same target
		m_vecTarget = _vecTarget;

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

		//Set position 
		m_vecPosition = _vecPosition;
		D3DXVECTOR3 AddHeight = { 0.0f, 50.0f, -50.0f };
		m_vecPosition += AddHeight;

		//Which way is up for the avatar
		m_VecUp = { 0.0f, 0.0f, 1.0f };
	}

}

bool CCameraStatic::Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecTarget, bool _bFirstPerson)
{
	m_vecPosition = _vecPosition;
	m_vecTarget = _vecTarget;
	m_bFirstPerson = _bFirstPerson;

	//Initialise Up vector on z axis
	m_VecUp = { 0.0f, 0.0f, 1.0f };

	//Distance behind the the avatar
	//m_f3PCDistBehind = 15.0f;
	//Distance above the avatar
	//m_f3PCDistUp = 3.0f;

	return true;
}

void CCameraStatic::Process(IRenderer* _pRenderManager)
{
	_pRenderManager->CreateViewMatrix(m_vecPosition, m_vecTarget, m_VecUp);
}

void CCameraStatic::ToggleType()
{
	m_bFirstPerson = !m_bFirstPerson;
}



