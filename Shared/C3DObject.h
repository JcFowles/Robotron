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
#include "../Shared/Graphics_Defines.h"


class C3DObject
{
public:
	
	/***********************
	* C3DObject: Default Constructor for 3DObject class
	* @author: Jc Fowles
	* @return:
	********************/
	C3DObject();

	/***********************
	* ~C3DObject: Destructor for 3DObject class
	* @author: Jc Fowles
	* @return:
	********************/
	~C3DObject();
		
	/***********************
	* Initialise: initialize all the member variables for the 3D Object
	* @author: Jc Fowles
	* @parameter: _pMesh: Mesh for this 3D Object
	* @parameter: _fX: Starting X coordinate. Default to 0 if not specified
	* @parameter: _fY: Starting Y coordinate. Default to 0 if not specified
	* @parameter: _fZ: Starting Z coordinate. Default to 0 if not specified
	* @return: void
	********************/
	void Initialise(IRenderer* _pRenderManager, MaterialValues _material, CMesh* _pMesh, float3 _f3Pos = { 0, 0, 0 });
	
	/***********************
	* Draw: Draws the 3D Object
	* @author: Jc Fowles
	* @return: void
	********************/
	void Draw();

	/***********************
	* Process: Processes the 3D Object using the Delta tick
	* @author: Jc Fowles
	* @parameter: _fDT: The Delta Tick
	* @return: void
	********************/
	void Process(float _fDT);

	//Getters
	
	/***********************
	* GetMaxSpeed: Returns the max speed of the object
	* @author: Jc Fowles
	* @return: float: 
	********************/
	float GetMaxSpeed() const { return m_fMaxSpeed; }
	
	/***********************
	* GetPosition: Return the position of the object
	* @author: Jc Fowles
	* @return: float3: The position of the object
	********************/
	float3 GetPosition() const { return m_f3Position; }
	
	/***********************
	* GetVelocity: Return the Velocity of the object
	* @author: Jc Fowles
	* @return: float3: The Velocity of the object
	********************/
	float3 GetVelocity() const { return m_f3Velocity; }
	
	/***********************
	* GetDirection: Return the Direction of the object
	* @author: Jc Fowles
	* @return: float3: The Direction of the object
	********************/
	float3 GetDirection() const { return m_f3Direction; }
	
	//Setters
		
	/***********************
	* SetRotation: Set the Yaw rotation of the 3D object 
	* @author: Jc Fowles
	* @Parameter: float _Rotation: value to rotate the 3D object by
	* @return: void: 
	********************/
	void SetRotation(float _Rotation) { m_fRotation = _Rotation; }
	
	/***********************
	* SetPosition: Set the position vector of the 3D object
	* @author: Jc Fowles
	* @Parameter: float3 _f3Position: position vector for the 3D object 
	* @return: void:
	********************/
	void SetPosition(float3 _f3Position) { m_f3Position = _f3Position; }
	
	/***********************
	* SetVelocity: Set the Velocity of the 3D object
	* @author: Jc Fowles
	* @Parameter: float3 _f3Velocity: Velocity vector for the 3D object
	* @return: void:
	********************/
	void SetVelocity(float3 _f3Velocity) { m_f3Velocity = _f3Velocity; }

	/***********************
	* SetDirection: Set the Direction vector of the 3D object
	* @author: Jc Fowles
	* @Parameter: float3 _f3Velocity: Velocity vector for the 3D object
	* @return: void:
	********************/
	void SetDirection(float3 _f3Direction) { m_f3Direction = _f3Direction; }
	

protected:
	
	/***********************
	* CalcWorldMatrix: Calculates the World Matrix for this Object
	* @author: Jc Fowles
	* @return: void
	********************/
	void CalcWorldMatrix();
	
	//Member variables
protected:
	
	IRenderer* m_pRenderManager;

	//Material
	int m_iMaterialID;
	
	float m_fRotation;  //Yaw Rotation (Radians)
	float m_fSize;		
	float m_fMaxSpeed;
	
	float3 m_f3Position;
	float3 m_f3Velocity;
	float3 m_f3Direction;
		
	CMesh* m_pMesh;
	

	//collision box
	//Structs min max float3 

};

#endif // __C3DOBJECT_H__ 