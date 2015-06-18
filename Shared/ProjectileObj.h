/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ProjectileObj.h
* Description : Header for the functions of the Projectile Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __CPROJECTILE_OBJ_H__
#define __CPROJECTILE_OBJ_H__

//Local Includes
#include "H:\GitHub\Robotron\Shared\C3DObject.h"

class CProjectileObj :
	public C3DObject
{
public:

	/***********************
	* CProjectileObj: Default Constructor for Projectile Object class
	* @author: Jc Fowles
	* @parameter: std::string _strPlayerName: The player that created this projectile
	* @parameter: float3 _f3Position: The initial position of this projectile
	* @parameter: float3 _f3Direction: The initial direction of this projectile
	* @return:
	********************/
	CProjectileObj(std::string _strPlayerName, float3 _f3Position, float3 _f3Direction);
		
	/***********************
	* ~CProjectileObj: Destructor for Projectile Object class
	* @author: Jc Fowles
	* @return:
	********************/
	~CProjectileObj();

	//Getters
		
	/***********************
	* GetDamage: Returns the damage dealt by the object
	* @author: Jc Fowles
	* @return: float: The damage dealt by the object
	********************/
	float GetDamage() const { return m_fDamage; }
	
	//Setters
	
	/***********************
	* SetDamage: Sets the damage dealt by the object
	* @author: Jc Fowles
	* @Parameter: float _fDamage: Value to set the damage dealt to
	* @return: void: 
	********************/
	void SetDamage(float _fDamage) { m_fDamage = _fDamage; }

private:
	float m_fDamage;
	
	std::string m_strPlayerName;
};

#endif //__CPROJECTILE_OBJ_H__