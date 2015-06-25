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
#include "C3DObject.h"

class CProjectileObj :
	public C3DObject
{
public:

	/***********************
	* CProjectileObj: Default Constructor for Projectile Object class
	* @author: Jc Fowles
	* @return:
	********************/
	CProjectileObj();
		
	/***********************
	* ~CProjectileObj: Destructor for Projectile Object class
	* @author: Jc Fowles
	* @return:
	********************/
	~CProjectileObj();

	//Getters
		
	/***********************
	* GetPlayerName: Get the ID of the player that released this projectile
	* @author: Jc Fowles
	* @return: UINT: The ID of the player that released this projectile
	********************/
	UINT GetPlayerID() const { return m_uiPlayerID; }

	//Setters
	
		
private:
	
	UINT m_uiPlayerID;
};

#endif //__CPROJECTILE_OBJ_H__