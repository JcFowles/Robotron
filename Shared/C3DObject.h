/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 3DObject.h
* Description : Header for the functions of the 3D Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __C3DOBJECT_H__
#define __C3DOBJECT_H__

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "Mesh.h"

class C3DObject
{
public:
	//Creation/Deletion
	C3DObject();
	~C3DObject();
		
	//Getters
	D3DXVECTOR3* GetPosition();
	D3DXVECTOR3* GetLook();
	D3DXVECTOR3* GetUp();
	D3DXVECTOR3* GetTarget();
	D3DXMATRIX GetWorldMatrix();
	float GetMoveSpeed();

	//Setters
	void SetMesh(CMesh* _pMesh);
	void SetPosition(float _fX, float _fY, float _fZ);
	void SetMoveSpeed(float _fMoveSpeed);
	void SetTurnSpeed(float _fTurnSpeed);
	
	//Functionality
	void Initialise(CMesh* _pMesh, float _fX = 0, float _fY = 0, float _fZ = 0);
	void Draw(IRenderer* _pRendererManager);
	void Process(float _fDT);

	void MoveForward(float _fDT);
	void Yaw(float _fDT);
	void Pitch(float _fDT);

protected:
	
	void CalcWorldMatrix(IRenderer* _pRendererManager);

protected:
	//Member Variables
	CMesh* m_pMesh;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	
	//Rotations
	float m_fRotationYaw;
	float m_fRotationPitch;
	float m_fRotationRoll;
	
	float m_fYawRevolution;
	float m_fPitchRevolution;
	float m_fRollRevolution;
	
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecTarget;
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecRight;

	D3DXVECTOR3 m_vecVelocity;

	float m_fMoveSpeed;
	float m_fTurnSpeed;
	
	float m_fMaxPitch;
	float m_fPitch;
		
};

#endif // __C3DOBJECT_H__ 