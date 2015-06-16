/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : IntelligentObj.h
* Description : Header for the functions of the  Intelligent Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __INTELLIGENT_OBJ_H__
#define __INTELLIGENT_OBJ_H__

//local includes
#include "..\Shared\C3DObject.h"

class CIntelligentObj :
	public C3DObject
{
public:
	
	/***********************
	* CIntelligentObj: Default Constructor for Intelligent object class
	* @author: Jc Fowles
	* @return: : 
	********************/
	CIntelligentObj();

	/***********************
	* C3DObject: Default destructor for Intelligent object class
	* @author: Jc Fowles
	* @return:
	********************/
	~CIntelligentObj();

	//Getters
	
	/***********************
	* GetPointValue: Return the points value
	* @author: Jc Fowles
	* @return: UINT: The points value
	********************/
	UINT GetPointValue() const { return m_uiPoints; }
		
	/***********************
	* GetMaxForce: Return the Max Force value
	* @author: Jc Fowles
	* @return: float: The Max Force value
	********************/
	float GetMaxForce() const { return m_fMaxForce; }

	/***********************
	* GetAcceleration: Return the acceleration vector
	* @author: Jc Fowles
	* @return: float3: The acceleration vector
	********************/
	float3 GetAcceleration() const { return m_f3Acceleration; }

	/***********************
	* Getf3DesiredVelocity: Return the Desired Velocity vector
	* @author: Jc Fowles
	* @return: float3: The Desired Velocity vector
	********************/
	float3 Getf3DesiredVelocity() const { return m_f3DesiredVelocity; }
	
	//Setters
	
	/***********************
	* SetPointValue: Set the points value of this object
	* @author: Jc Fowles
	* @Parameter: UINT _uiPointValue: value to set the points to
	* @return: void: 
	********************/
	void SetPointValue(UINT _uiPointValue) { m_uiPoints = _uiPointValue; }
		
	/***********************
	* SetMaxForce: Set the Max force value
	* @author: Jc Fowles
	* @Parameter: float _fMaxForce: Value to set max force to
	* @return: void: 
	********************/
	void SetMaxForce(float _fMaxForce) { m_fMaxForce = _fMaxForce; }

	/***********************
	* SetAcceleration: Set the acceleration vector
	* @author: Jc Fowles
	* @Parameter: float3 _f3Acceleration: Value to set the acceleration vector to
	* @return: void:
	********************/
	void SetAcceleration(float3 _f3Acceleration) { m_f3Acceleration = _f3Acceleration; }

	/***********************
	* SetAcceleration: Set the Desired velocity vector
	* @author: Jc Fowles
	* @Parameter: float3 _f3DesiredVelocity: Value to set the Desired velocity vector to
	* @return: void:
	********************/
	void SetF3DesiredVelocity(float3 _f3DesiredVelocity) { m_f3DesiredVelocity = _f3DesiredVelocity; }

	//Member variables
protected:
	UINT m_uiPoints;
	float m_fMaxForce;
	
	float3 m_f3Acceleration;
	float3 m_f3DesiredVelocity;
	
};

#endif //__INTELLIGENT_OBJ_H__