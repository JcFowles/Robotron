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

//TO DO remove unneeded

//Include library
#include <d3d9.h>
#include <d3dx9.h>

//Local includes
#include "IRenderer.h"

class CDebugCamera
{
	//member functions
public:
	//****Creation/Deletion****

	/***********************
	* CCamera: Constructor of the CCamera object
	* @author: Jc Fowles
	* @return:
	********************/
	CDebugCamera();

	/***********************
	* CCamera: Destructor of the CCamera object
	* @author: Jc Fowles
	* @return:
	********************/
	~CDebugCamera();

	//Generic Functions

	/***********************
	* SetPostionVec: Set the Position vector
	* @author: Jc Fowles
	* @return: bool: Successful Initialization or not
	********************/
	bool Initialise(IRenderer* _pRenderManager);

	/***********************
	* process: Process the camera
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();
		
	//****Getters****
	
	//View Stuff

	/***********************
	* GetViewMatrix: Get the View matrix
	* @author: Jc Fowles
	* @return: D3DXMATRIX*: Returns the the View Matrix
	********************/
	D3DXMATRIX* GetViewMatrix(){ return &m_D3DMatView; };

	/***********************
	* GetPosition: Get the Position of the camera
	* @author: Jc Fowles
	* @return: D3DXVECTOR3*: Returns the the Position vector of the camera
	********************/
	D3DXVECTOR3* GetPosition(){	return &m_D3DVecPosition;};
	
	/***********************
	* GetTarget: Get the target of the camera
	* @author: Jc Fowles
	* @return: D3DXVECTOR3*: Returns the the target vector of the camera
	********************/
	D3DXVECTOR3* GetTarget(){ return &m_D3DVecTarget;	};

	/***********************
	* GetUp: Get the Up vector of the camera
	* @author: Jc Fowles
	* @return: D3DXVECTOR3*: Returns the the up vector of the camera
	********************/
	D3DXVECTOR3* GetUp(){ return &m_D3DVecUp; };

	/***********************
	* GetLook: Get the Look vector of the camera
	* @author: Jc Fowles
	* @return: D3DXVECTOR3*: Returns the the target vector of the camera
	********************/
	D3DXVECTOR3* GetLook() { return &m_D3DVeclook; };

	//Projection Stuff
	

	//Limitations

	/***********************
	* GetMaxVelocity: Get the Camera's Max allowed velocity
	* @author: Jc Fowles
	* @return: float: Returns the Camera's Max allowed velocity
	********************/
	float GetMaxVelocity(){	return m_fMaxVelocity;	};

	/***********************
	* GetInvertY: Return if the Y-Axis has been inverted
	* @author: Jc Fowles
	* @return: bool: Returns if the Y-Axis has been inverted
	********************/
	BOOL  GetInvertY(){	return m_bInvertY;}	;

	/***********************
	* GetMaxPitch: Return The maximum allow pitch rotation
	* @author: Jc Fowles
	* @return: float: Returns The maximum allow pitch rotation
	********************/
	float GetMaxPitch(){return m_fMaxPitch;};
	
	//Rotations
		
	/***********************
	* GetPitch: Return the pitch rotation in Degree
	* @author: Jc Fowles
	* @return: float: Returns the pitch rotation in Degree
	********************/
	float GetPitch(){return m_fPitch;};
	
	/***********************
	* GetYaw: Return the Yaw rotation in Degree
	* @author: Jc Fowles
	* @return: float: Returns the yaw rotation in Degree
	********************/
	float GetYaw() { return m_fYaw; };

	
	//****Setters****

	//View
	
	/***********************
	* SetViewProperties: Set the Position, LookAt/target and Up vector for the view matrix
	* @author: Jc Fowles
	* @parameter: _D3DVecCamPos: The position vector to set the camera to
	* @return: void
	********************/
	void SetViewProperties(D3DXVECTOR3& _D3DVecCamPos, D3DXVECTOR3& _D3DVecLookAt, D3DXVECTOR3& _D3DVecUp);
	
	/***********************
	* SetPostionVec: Set the Position vector
	* @author: Jc Fowles
	* @parameter: _D3DVecCamPos: The position vector to set the camera to
	* @return: void
	********************/
	void SetPostionVec(D3DXVECTOR3& _D3DVecCamPos){	m_D3DVecPosition = _D3DVecCamPos;};
	
	/***********************
	* SetLookAtVec: Set the Look At vector
	* @author: Jc Fowles
	* @parameter: _D3DVecLookAt: The Look At vector to set the camera to
	* @return: void
	********************/
	void SetTargetVec(D3DXVECTOR3& _D3DVecLookAt){ m_D3DVecTarget = _D3DVecLookAt;};
	
	/***********************
	* SetUpVec: Set the Look At vector
	* @author: Jc Fowles
	* @parameter: _D3DVecUp: The Up vector to set the camera to
	* @return: void
	********************/
	void SetUpVec(D3DXVECTOR3& _D3DVecUp) { m_D3DVecUp = _D3DVecUp; };
	
	
	/***********************
	* SetMaxVelocity: Set max velocity
	* @author: Jc Fowles
	* @Parameter: float maxVelocity: value to set max velocity to
	* @return: void: 
	********************/
	void CDebugCamera::SetMaxVelocity(float maxVelocity) { m_fMaxVelocity = maxVelocity; }
	
	/***********************
	* SetMaxPitch: Set what the max pitch is allowed
	* @author: Jc Fowles
	* @Parameter: float maxPitch:Value to set max pitch to
	* @return: void: 
	********************/
	void CDebugCamera::SetMaxPitch(float maxPitch)     { m_fMaxPitch = maxPitch; }
	
	/***********************
	* SetInvertY: Weather or not to invert the Y
	* @author: Jc Fowles
	* @Parameter: BOOL invert: Value to set invert to 
	* @return: void: 
	********************/
	void CDebugCamera::SetInvertY(BOOL invert)          { m_bInvertY = invert; }
	
	/***********************
	* SetSpeed: Set the speed
	* @author: Jc Fowles
	* @Parameter: float _fSpeed: Value to set speed to 
	* @return: void: 
	********************/
	void CDebugCamera::SetSpeed(float _fSpeed)			{ m_fSpeed = _fSpeed; }

	
	
	//****Functionality****
	
	//Rotations
	
	/***********************
	* Pitch: Rotate camera on the X-Axis
	* @author: Jc Fowles
	* @parameter: _fDeg: The amount in degree you would like to rotate
	* @return: void
	********************/
	void Pitch(float _fDeg);	//X-Axis

	/***********************
	* Pitch: Rotate camera on the Y-Axis
	* @author: Jc Fowles
	* @parameter: _fDeg: The amount in degree you would like to rotate
	* @return: void
	********************/
	void Yaw(float _fDeg);		//Y-Axis


	/***********************
	* Pitch: Rotate camera on the Z-Axis
	* @author: Jc Fowles
	* @parameter: _fDeg: The amount in degree you would like to rotate
	* @return: void
	********************/
	void Roll(float _fDeg);		//Z-Axis

	//Movements
	// 
	/***********************
	* Strafe: Move the camera on the X-Axis
	* @author: Jc Fowles
	* @parameter: _fDistance: The Distance you would like to move
	* @return: void
	********************/
	void Strafe(float _fDistance);	//X-Axis
	
	/***********************
	* Fly: Move the camera on the Y-Axis
	* @author: Jc Fowles
	* @parameter: _fDistance: The Distance you would like to move
	* @return: void
	********************/
	void Fly(float _fDistance);		//Y-Axis
	
	/***********************
	* Move: Move the camera on the Z-Axis
	* @author: Jc Fowles
	* @parameter: _fDistance: The Distance you would like to move in units per velocity
	* @return: void
	********************/
	void Move(float _fDistance);	//Z-Axis
	
	

	
	//Member variables
private:
	
	//Matrices
	D3DXMATRIX  m_D3DMatView;
		
	//Camera Axis
	D3DXVECTOR3 m_D3DVecRight;	//Camera Right(X-Axis)
	D3DXVECTOR3 m_D3DVecUp;		//Camera Up(Y-Axis)
	D3DXVECTOR3 m_D3DVeclook;	//Camera Look(Z-Axis)

	//Camera Properties
	D3DXVECTOR3 m_D3DVecPosition;
	D3DXVECTOR3 m_D3DVecTarget;		//Target 
	D3DXVECTOR3 m_D3DVecVelocity;	
	
	//Rotation Values
	float       m_fYaw;
	float       m_fPitch;
	float		m_fRoll;
	
	//Rotation Limitations
	float       m_fMaxPitch;
	float       m_fMaxVelocity;

	//Projection Values
	float       m_fFOV;
	float       m_fAspect;
	float       m_fNearPlane;
	float       m_fFarPlane;
	
	//Camera Booleans
	BOOL        m_bInvertY;
	BOOL        m_bEnableYMovement;

	float m_fSpeed;

	IRenderer* m_pRenderManager; 
		
};

