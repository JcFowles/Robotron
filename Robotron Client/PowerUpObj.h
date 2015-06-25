/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : PowerUpObj.h
* Description : Header for the functions of the  power up Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __POWER_UP_OBJ_H__
#define __POWER_UP_OBJ_H__

//Local includes
#include "IntelligentObj.h"

class CPowerUpObj :
	public CIntelligentObj
{
public:

	/***********************
	* CPowerUp: Default Constructor for power up Obj object class
	* @author: Jc Fowles
	* @parameter: ePowerType upType: Type of power up to be created
	* @return: :
	********************/
	CPowerUpObj(ePowerType _etype);

	/***********************
	* CPowerUp: Default Destructor for power up Obj object class
	* @author: Jc Fowles
	* @return: :
	********************/
	~CPowerUpObj();

	//Getters
			
	/***********************
	* GetType: Get the enum type of the power up object
	* @author: Jc Fowles
	* @return: ePowerType: The enum type of the power up type object
	********************/
	ePowerType GetType() const { return m_eType; }

	//Setters
		

	//member variables
private:
		
	ePowerType m_eType;
	
};

#endif //__POWER_UP_OBJ_H__