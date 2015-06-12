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

/***********************
* C3DObject: Default Contructor for 3DObject class
* @author: Jc Fowles
* @return:
********************/
C3DObject::C3DObject()
{
	m_pMesh = 0;
	
	m_vecPosition.x = 0;
	m_vecPosition.y = 0;
	m_vecPosition.z = 0;
		
	m_fRotationYaw = 0;
	m_fYawRevolution = 0;
	m_fRotationPitch = 0;
	
	m_fPitchRevolution = 0;
	m_fRotationRoll = 0;
	m_fRollRevolution = 0;
}

/***********************
* ~C3DObject: Destructor for 3DObject class
* @author: Jc Fowles
* @return:
********************/
C3DObject::~C3DObject()
{
}

/***********************
* Initialise: inisiatise all the member variables for the 3D Object
* @author: Jc Fowles
* @parameter: _pMesh: Mesh for this 3D Object
* @parameter: _fX: Starting X coordinate. Default to 0 if not specified
* @parameter: _fY: Starting Y coordinate. Default to 0 if not specified
* @parameter: _fZ: Starting Z coordinate. Default to 0 if not specified
* @return: void
********************/
void C3DObject::Initialise(CMesh* _pMesh, float _fX, float _fY, float _fZ)
{
	m_pMesh = _pMesh;

	m_vecPosition.x = _fX;
	m_vecPosition.y = _fY;
	m_vecPosition.z = _fZ;

	m_fMoveSpeed = 50.0f;
	m_fTurnSpeed = 60.0f;
	
	m_vecLook = {0.0f,0.0f,1.0f};
	m_vecVelocity = { 0.0f, 0.0f, 0.0f };
	m_vecUp = { 0.0f, 1.0f, 0.0f };

	m_fMaxPitch = 89.0f;
}

/***********************
* Draw: Draws the 3D Object
* @author: Jc Fowles
* @return: void
********************/
void C3DObject::Draw(IRenderer* _pRendererManager)
{
	//Calculates the World Matrix for this 3D Object
	CalcWorldMatrix(_pRendererManager);
	DWORD dwMatColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	_pRendererManager->SetMaterial(dwMatColor);
	m_pMesh->Draw();
}

/***********************
* Process: Processes the 3D Object for the Delta tick
* @author: Jc Fowles
* @parameter: _fDT: The Delta Tick
* @return: void
********************/
void C3DObject::Process(float _fDT)
{
	//Set the new position based on the current velocity
	m_vecPosition += m_vecVelocity;
	//Stop the the cube from continualy moving
	m_vecVelocity = { 0.0f, 0.0f, 0.0f };
	m_vecTarget = m_vecPosition + 10*m_vecLook;
	//Create the view matric of the object
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecTarget, &m_vecUp);
	//Update the right vector
	m_vecRight.x = m_matView._11;
	m_vecRight.y = m_matView._21;
	m_vecRight.z = m_matView._31;
	//Update the look vector	
	m_vecLook.x = m_matView._13;
	m_vecLook.y = m_matView._23;
	m_vecLook.z = m_matView._33;

	//Calculate Pitch
	float flookMagnitudeOnXZ = sqrtf(m_vecLook.z * m_vecLook.z + m_vecLook.x * m_vecLook.x);
	m_fPitch = D3DXToDegree(atan2f(m_vecLook.y, flookMagnitudeOnXZ));
}

/***********************
* SetMesh: Sets a new Mesh for the 3D Object
* @author: Jc Fowles
* @parameter: _pMesh: Pointer to a new mesh for the 3D Object
* @return: void
********************/
void C3DObject::SetMesh(CMesh* _pMesh)
{
	m_pMesh = _pMesh;
}

/***********************
* SetPos: Set the position of the 3D object
* @author: Jc Fowles
* @parameter: _fX: X pos of 3D Object
* @parameter: _fY: Y pos of 3D Object
* @parameter: _fZ: Z pos of 3D Object
* @return: void
********************/
void C3DObject::SetPosition(float _fX, float _fY, float _fZ)
{
	m_vecPosition.x = _fX;
	m_vecPosition.y = _fY;
	m_vecPosition.z = _fZ;
}

/***********************
* SetMoveSpeed: Sets Speed the object moves at
* @author: Jc Fowles
* @parameter: _fMoveSpeed: the float value to set as the move speed
* @return: void
********************/
void C3DObject::SetMoveSpeed(float _fMoveSpeed)
{
	m_fMoveSpeed = _fMoveSpeed;
}

/***********************
* SetTurnSpeed: Sets Speed the object turns at
* @author: Jc Fowles
* @parameter: _fTurnSpeed: the float value to set as the turn speed
* @return: void
********************/
void C3DObject::SetTurnSpeed(float _fTurnSpeed)
{
	m_fTurnSpeed = _fTurnSpeed;
}

/***********************
* GetSpeed: Return the move Speed of the object
* @author: Jc Fowles
* @return: int: The move Speed of the object
********************/
float C3DObject::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/***********************
* GetPosition: Return the postion of the object in 3D space
* @author: Jc Fowles
* @return: D3DXVECTOR3: The postion of the object in 3D space
********************/
D3DXVECTOR3* C3DObject::GetPosition()
{
	return &m_vecPosition;
}

/***********************
* GetLook: Return the directional vector of where the object is looking
* @author: Jc Fowles
* @return: D3DXVECTOR3: The directional vector of where the object is looking
********************/
D3DXVECTOR3* C3DObject::GetLook()
{
	return &m_vecLook;
}

/***********************
* GetLook: Return the directional vector of where the object is looking
* @author: Jc Fowles
* @return: D3DXVECTOR3: The directional vector of where the object is looking
********************/
D3DXVECTOR3* C3DObject::GetTarget()
{
	return &m_vecTarget;
}

/***********************
* GetUp: Return the up vector of the object 
* @author: Jc Fowles
* @return: D3DXVECTOR3: The up vector of the object 
********************/
D3DXVECTOR3* C3DObject::GetUp()
{
	return &m_vecUp;
}

/***********************
* GetWorldMat: Return the world matrix of the object
* @author: Jc Fowles
* @return: D3DXMATRIX: The world matrix of the object
********************/
D3DXMATRIX C3DObject::GetWorldMatrix()
{
	return m_matWorld;
}

/***********************
* CalcWorldMatrix: Calculates the World Matrix for this Object
* @author: Jc Fowles
* @parameter: _pRendererManager: a pointer to the render manager who will ultimately set the world matrix
* @return: void
********************/
void C3DObject::CalcWorldMatrix(IRenderer* _pRendererManager)
{
	//Matrices to make up the World Matrix
	D3DXMATRIX matRotateYaw;
	D3DXMatrixIdentity(&matRotateYaw);
	D3DXMATRIX matRotatePitch;
	D3DXMatrixIdentity(&matRotatePitch);
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	//Reset world matrix
	D3DXMatrixIdentity(&m_matWorld);
		
	//Increases the rotation but the revolution amount
	m_fRotationYaw	 += (m_fYawRevolution);
	m_fRotationPitch += (m_fPitchRevolution);
	m_fRotationRoll  += (m_fRollRevolution);
   
	//Create rotational Transform matrix
	D3DXMatrixRotationYawPitchRoll(&m_matWorld, m_fRotationYaw, m_fRotationPitch, m_fRotationRoll);

	//Create the translation matrix from the objects position coordinates
	D3DXMatrixTranslation(&matTranslation, m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);
	
	//Calculate the world matrix
	m_matWorld *= matTranslation;
	
	//Set the world matrix of this object
	_pRendererManager->SetWorldMatrix(m_matWorld);
	
	//Reset the revolutions
	m_fYawRevolution = 0;
	m_fPitchRevolution = 0;
	m_fRollRevolution = 0;
}

/***********************
* MoveForward: Moves the object forward in the direction of where it is looking
* @author: Jc Fowles
* @author: _fDT: delta tick as Number of units to move 
* @return: void
********************/
void C3DObject::MoveForward(float _fDT)
{
	//Movement Dependant on delta tick
	_fDT *= m_fMoveSpeed;
	//set velocity to Move in the direction of the look vector
	D3DXVECTOR3 normalisedLook; 
	D3DXVec3Normalize(&normalisedLook, &m_vecLook);
	m_vecVelocity += normalisedLook * _fDT;
}

/***********************
* Yaw: Rotates the object around its Up vector
* @author: Jc Fowles
* @author: _fDT: delta tick as the Number of Degrees to rotate
* @return: void
********************/
void C3DObject::Yaw(float _fDT)
{
	//Return if _fDT is too small to process
	if (_fDT == 0.0f)
	{
		return;
	}
	//Calculate its turning rate based on speed and delta tick, also make it 1.8 times fast at turning than it is at moving 
	_fDT *= m_fTurnSpeed;
	//set the yaw revolutions in radians	
	m_fYawRevolution = D3DXToRadian(_fDT);

	//Calculate the rotation matrix around the up vector
	D3DXMATRIX D3DXRotation;
	D3DXMatrixRotationAxis(&D3DXRotation, &m_vecUp, D3DXToRadian(_fDT));
	
	//Set the new look vector by transforming it by the rotation martix around the up vector
	D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &D3DXRotation);
	//Set the new right vector by transforming it by the rotation martix around the up vector
	D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &D3DXRotation);
	
}

/***********************
* Pitch: Rotates the object around its Up vector
* @author: Jc Fowles
* @author: _fDT: delta tick as the Number of Degrees to rotate 
* @return: void
********************/
void C3DObject::Pitch(float _fDT)
{
	//Return if _fDT is too small to process
	if (_fDT == 0.0f)
	{
		return;
	}

	//Calculate its turning rate based on turn speed
	_fDT *= m_fTurnSpeed;

	//Minus speed*deltaTick from pitch
	m_fPitch -= _fDT;

	bool bRotate = true;

	//Check if pitch is with in range, If not do not rotate
	if (m_fPitch > m_fMaxPitch)
	{
		bRotate = false;
	}
	else if (m_fPitch < -m_fMaxPitch)
	{
		bRotate = false;
	}
		
	if (bRotate)
	{
		//Set pitch rotation to passed in degrees, but as radians
		m_fPitchRevolution = D3DXToRadian(_fDT);
		//Calculate transfomation matrix
		D3DXMATRIX D3DXRotation;
		D3DXMatrixRotationAxis(&D3DXRotation, &m_vecRight, D3DXToRadian(_fDT));
		
		//Transform remaining axis
		D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &D3DXRotation);
		D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &D3DXRotation);
	}

}