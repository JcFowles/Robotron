/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : CCamera.cpp
* Description : Contains the main funtionality of camera Class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "CameraStatic.h"

/***********************
* CCam: Default Contructor for Cam class
* @author: Jc Fowles
* @return:
********************/
CCameraStatic::CCameraStatic()
{
	m_vecPosition = { 0, 0, 0 };
	m_vecTarget = { 0, 0, 1 };

	m_bFirstPerson = true;
}

/***********************
* ~CCam: destructor for Cam class
* @author: Jc Fowles
* @return:
********************/
CCameraStatic::~CCameraStatic()
{
}

/***********************
* Initialise:Initialise the the camera
* @author: Jc Fowles
* @parameter: _vecPosition: position of the camera
* @parameter: _vecTarget: direction camera is looking
* @parameter: _bFirstPerson: Whether camera is in first or third person mode
* @return: bool: 
********************/
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

/***********************
* Process: Process the camera, setting the view matrix of the Render manager
* @author: Jc Fowles
* @parameter: _pRenderManager: Pointer to the render manager, who will untimaly set the view
* @return: void
********************/
void CCameraStatic::Process(IRenderer* _pRenderManager)
{
	_pRenderManager->CreateViewMatrix(m_vecPosition, m_vecTarget, m_VecUp);
}

/***********************
* SetPosition: Set up the camera in the correct postion
* @author: Jc Fowles
* @parameter: _vecPosition: Position to set the camera to
* @return: void
********************/
void CCameraStatic::SetPosition(D3DXVECTOR3 _vecPosition)
{
	m_vecPosition = _vecPosition;
}

/***********************
* SetType: Sets the camera type; true - first person or false - third  person
* @author: Jc Fowles
* @parameter: _bFirstPerson: Camera type to be set as: true - first person or false - third person
* @return: void
********************/
void CCameraStatic::SetType(bool _bFirstPerson)
{
	m_bFirstPerson = _bFirstPerson;
}

/***********************
* SetCamera: Set up the camera in the correct position and correct target
* @author: Jc Fowles
* @parameter: _vecTarget: Target vector of the avatar
* @parameter: _vecPosition: Position vector of the avatar
* @parameter: _vecUp: Up vector of the avatar
* @return: void
********************/
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
		
		//Set position directly 100 unit above (on Y-axis) past in position
		m_vecPosition = _vecPosition;
		D3DXVECTOR3 AddHeight = { 0.0f, 100.0f, 0.0f  };
		m_vecPosition += AddHeight;
		
		//Which way is up for the avatar
		m_VecUp = {0.0f,0.0f,1.0f};
	}

}

/***********************
* GetCameraType: Returns whether camera is in first person
* @author: Jc Fowles
* @return: bool: Return true for first person, false for third person
********************/
bool CCameraStatic::GetCameraType()
{
	return m_bFirstPerson;
}

/***********************
* GetPosition: Returns the camera position
* @author: Jc Fowles
* @return: D3DXVECTOR3: position of the camera
********************/
D3DXVECTOR3* CCameraStatic::GetPosition()
{
	return &m_vecPosition;
}

/***********************
* GetTarget: Returns the camera target
* @author: Jc Fowles
* @return: D3DXVECTOR3: Target of the camera
********************/
D3DXVECTOR3* CCameraStatic::GetTarget()
{
	return &m_vecTarget;
}

/***********************
* ToggleType: Toggle camera between first and third person views
* @author: Jc Fowles
* @return: void
********************/
void CCameraStatic::ToggleType()
{
	m_bFirstPerson = !m_bFirstPerson;
}



