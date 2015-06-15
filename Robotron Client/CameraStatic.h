//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CCamera.h
// Description : Contains the header file for CCamera Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

//Local includes
#include "IRenderer.h"
//#include "Defines.h"

class CCameraStatic
{
	//Member Functions
public:
	//Creation/Deletion
	CCameraStatic();
	~CCameraStatic();

	//Generic functions
	bool Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecTarget, bool m_bFirstPerson);
	void Process(IRenderer* _pRenderManager);
		
	//Setters
	void SetPosition(D3DXVECTOR3 _vecPosition);
	void SetType(bool _bFirstPerson);
	void SetCamera( D3DXVECTOR3 _vecTarget,
					D3DXVECTOR3 _vecPosition, 
					D3DXVECTOR3 _vecUp);

	//Getters
	bool GetCameraType();
	D3DXVECTOR3* GetPosition();
	D3DXVECTOR3* GetTarget();

	//Functionality
	void ToggleType();

	//Member varialble
private:

	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecTarget;
	D3DXVECTOR3 m_VecUp;

	float m_f3PCDistBehind;
	float m_f3PCDistUp;

	bool m_bFirstPerson;


};