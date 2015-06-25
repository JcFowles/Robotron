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

#ifndef __CCAMERASTATIC_H__
#define __CCAMERASTATIC_H__

//TO DO: Rename, edit function headers, change to used float3 instead of D3DXVECTOR3

//Local includes
#include "IRenderer.h"

class CCameraStatic
{
	//Member Functions
public:

	/***********************
	* CCameraStatic: Default Constructor for Static Camera class
	* @author: Jc Fowles
	* @return:
	********************/
	CCameraStatic();

	/***********************
	* ~CCam: destructor for Cam class
	* @author: Jc Fowles
	* @return:
	********************/
	~CCameraStatic();
		
	//Getters
		
	/***********************
	* GetCameraType: Returns whether camera is in first person
	* @author: Jc Fowles
	* @return: bool: Return true for first person, false for third person
	********************/
	bool GetCameraType() { return m_bFirstPerson; };

	/***********************
	* GetPosition: Returns the camera position
	* @author: Jc Fowles
	* @return: D3DXVECTOR3: position of the camera
	********************/
	D3DXVECTOR3* GetPosition() { return &m_vecPosition; };

	/***********************
	* GetTarget: Returns the camera target
	* @author: Jc Fowles
	* @return: D3DXVECTOR3: Target of the camera
	********************/
	D3DXVECTOR3* GetTarget() { return &m_vecTarget; };

	//Setters

	/***********************
	* SetPosition: Set up the camera in the correct position
	* @author: Jc Fowles
	* @parameter: _vecPosition: Position to set the camera to
	* @return: void
	********************/
	void SetPosition(D3DXVECTOR3 _vecPosition){ m_vecPosition = _vecPosition; };

	/***********************
	* SetType: Sets the camera type; true - first person or false - third  person
	* @author: Jc Fowles
	* @parameter: _bFirstPerson: Camera type to be set as: true - first person or false - third person
	* @return: void
	********************/
	void SetType(bool _bFirstPerson) { m_bFirstPerson = _bFirstPerson; };
	
	/***********************
	* SetCamera: Set up the camera in the correct position and correct target
	* @author: Jc Fowles
	* @parameter: _vecTarget: Target vector of the avatar
	* @parameter: _vecPosition: Position vector of the avatar
	* @parameter: _vecUp: Up vector of the avatar
	* @return: void
	********************/
	void SetCamera( D3DXVECTOR3 _vecTarget,
					D3DXVECTOR3 _vecPosition, 
					D3DXVECTOR3 _vecUp);

	//Prototypes

	/***********************
	* Initialise:Initialise the the camera
	* @author: Jc Fowles
	* @parameter: _vecPosition: position of the camera
	* @parameter: _vecTarget: direction camera is looking
	* @parameter: _bFirstPerson: Whether camera is in first or third person mode
	* @return: bool:
	********************/
	bool Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecTarget, bool m_bFirstPerson);

	/***********************
	* Process: Process the camera, setting the view matrix of the Render manager
	* @author: Jc Fowles
	* @parameter: _pRenderManager: Pointer to the render manager, who will ultimately set the view
	* @return: void
	********************/
	void Process(IRenderer* _pRenderManager);

	/***********************
	* ToggleType: Toggle camera between first and third person views
	* @author: Jc Fowles
	* @return: void
	********************/
	void ToggleType();

	//Member variable
private:

	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecTarget;
	D3DXVECTOR3 m_VecUp;

	float m_f3PCDistBehind;
	float m_f3PCDistUp;

	bool m_bFirstPerson;


};

#endif //__CCAMERASTATIC_H__