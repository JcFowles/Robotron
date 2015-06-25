//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CDebugCamera.cpp
// Description : Contains the main functionality of CCamera Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "DebugCamera.h"

CDebugCamera::CDebugCamera()
{
	
}

CDebugCamera::~CDebugCamera()
{

}

bool CDebugCamera::Initialise(IRenderer* _pRenderManager)
{
	m_pRenderManager = _pRenderManager;

	m_fMaxPitch = (89.0f);
	m_fMaxVelocity = 100.0f;
	m_bInvertY = FALSE;
	m_bEnableYMovement = TRUE;
	m_fSpeed = 15.0f;

	m_D3DVecPosition = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_D3DVecTarget = D3DXVECTOR3(0.0f, 0.0f, 150.0f);
	
	m_D3DVecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_D3DVecUp, &m_D3DVecUp);
	
	m_D3DVecVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_D3DVeclook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&m_D3DVeclook, &m_D3DVeclook);

	
	//float fAspectRatio = (_fScreenWidth / _fScreenHeight);
	//CreateProjectionMatrix(D3DXToRadian(45), fAspectRatio, 0.1f, 100000.0f);
	
	Process();
		
	return true;
}

void CDebugCamera::SetViewProperties(D3DXVECTOR3& _D3DVecCamPos, D3DXVECTOR3& _D3DVecLookAt, D3DXVECTOR3& _D3DVecUp)
{
	m_D3DVecPosition = _D3DVecCamPos;
	m_D3DVecTarget = _D3DVecLookAt;
	m_D3DVecUp = _D3DVecUp;
}

void CDebugCamera::Process()
{
	
	//Cap velocity to max velocity
	if (D3DXVec3Length(&m_D3DVecVelocity) > m_fMaxVelocity)
	{
		m_D3DVecVelocity = *(D3DXVec3Normalize(&m_D3DVecVelocity, &m_D3DVecVelocity)) * m_fMaxVelocity;
	}
		
	//Move the camera
	//m_D3DVecVelocity += m_D3DVecVelocity * (_fDt);

	m_D3DVecPosition += m_D3DVecVelocity ;
	
	//Could decelerate here. I'll just stop completely.
	m_D3DVecVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_D3DVecTarget = m_D3DVecPosition + m_D3DVeclook;

	//Calculate the new view matrix
	D3DXVECTOR3 D3DVecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_D3DMatView, &m_D3DVecPosition, &m_D3DVecTarget, &D3DVecUp);

	//Set the camera axis from the view matrix
	m_D3DVecRight.x = m_D3DMatView._11;
	m_D3DVecRight.y = m_D3DMatView._21;
	m_D3DVecRight.z = m_D3DMatView._31;
	m_D3DVecUp.x = m_D3DMatView._12;
	m_D3DVecUp.y = m_D3DMatView._22;
	m_D3DVecUp.z = m_D3DMatView._32;
	m_D3DVeclook.x = m_D3DMatView._13;
	m_D3DVeclook.y = m_D3DMatView._23;
	m_D3DVeclook.z = m_D3DMatView._33;

	//Calculate yaw and pitch
	float lookLengthOnXZ = sqrtf(m_D3DVeclook.z * m_D3DVeclook.z + m_D3DVeclook.x * m_D3DVeclook.x);
	m_fPitch = D3DXToDegree(atan2f(m_D3DVeclook.y, lookLengthOnXZ)); // atan2f(m_D3DVeclook.y, lookLengthOnXZ);
	
	//Get the render to create the view matrix and set the view port
	m_pRenderManager->CreateViewMatrix(m_D3DVecPosition, m_D3DVecTarget, D3DVecUp);

}

void CDebugCamera::Pitch(float _fDeg)
{
	
	float fRotInRads = _fDeg * m_fSpeed;
	if (fRotInRads == 0.0f)
	{
		return;
	}
			
	m_fPitch -= fRotInRads;
	
	if (m_fPitch > m_fMaxPitch)
	{
		fRotInRads += m_fPitch - m_fMaxPitch;
	}
	else if (m_fPitch < -m_fMaxPitch)
	{
		fRotInRads += m_fPitch + m_fMaxPitch;
	}

	D3DXMATRIX D3DXRotation;
	D3DXMatrixRotationAxis(&D3DXRotation, &m_D3DVecRight, D3DXToRadian(fRotInRads));
	D3DXVec3TransformNormal(&m_D3DVecUp, &m_D3DVecUp, &D3DXRotation);
	D3DXVec3TransformNormal(&m_D3DVeclook, &m_D3DVeclook, &D3DXRotation);
}

void CDebugCamera::Yaw(float _fDeg)
{
	float fRotInRads = D3DXToRadian(_fDeg);
	fRotInRads = fRotInRads * m_fSpeed;
	//TO DO - comment
	if (fRotInRads == 0.0f)
	{
		return;
	}

	D3DXMATRIX D3DXRotation;
	D3DXMatrixRotationAxis(&D3DXRotation, &m_D3DVecUp, fRotInRads);
	D3DXVec3TransformNormal(&m_D3DVecRight, &m_D3DVecRight, &D3DXRotation);
	D3DXVec3TransformNormal(&m_D3DVeclook, &m_D3DVeclook, &D3DXRotation);
}

void CDebugCamera::Roll(float _fDeg)
{
	float fRotInRads = D3DXToRadian(_fDeg);
	//TO DO - comment
	if (fRotInRads == 0.0f)
	{
		return;
	}
	
	D3DXMATRIX D3DXRotation;
	D3DXMatrixRotationAxis(&D3DXRotation, &m_D3DVeclook, fRotInRads);
	D3DXVec3TransformNormal(&m_D3DVecRight, &m_D3DVecRight, &D3DXRotation);
	D3DXVec3TransformNormal(&m_D3DVecUp, &m_D3DVecUp, &D3DXRotation);
}

void CDebugCamera::Strafe(float _fDistance)
{
	//TO DO - comment
	_fDistance = _fDistance * m_fSpeed;
	m_D3DVecVelocity += m_D3DVecRight * _fDistance;
}

void CDebugCamera::Fly(float _fDistance)
{
		if (m_bEnableYMovement)
	{
		_fDistance = _fDistance * m_fSpeed;
		m_D3DVecVelocity.y += _fDistance;
	}
}

void CDebugCamera::Move(float _fDistance)
{
	if (m_bEnableYMovement)
	{
		_fDistance = _fDistance * m_fSpeed;
		m_D3DVecVelocity += m_D3DVeclook * _fDistance;
	}
	else
	{
		D3DXVECTOR3 D3DMoveVector(m_D3DVeclook.x, 0.0f, m_D3DVeclook.z);
		D3DXVec3Normalize(&D3DMoveVector, &D3DMoveVector);
		D3DMoveVector *= _fDistance;
		m_D3DVecVelocity += D3DMoveVector;
	}
}

