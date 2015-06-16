/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : EnemyObj.h
* Description : Header for the functions of the  Enemy Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

#ifndef __ENEMY_OBJ_H__
#define __ENEMY_OBJ_H__

//Local includes
#include "IntelligentObj.h"

class CEnemyObj :
	public CIntelligentObj
{
public:
	CEnemyObj();
	CEnemyObj(eEnemyTypes _eEnemyType);
	~CEnemyObj();

	//Getters
		
	/***********************
	* GetMaxHealth: Get the max heath of the enemy object
	* @author: Jc Fowles
	* @return: float: The max heath of th enemy object
	********************/
	float GetMaxHealth() const { return m_fMaxHealth; }

	/***********************
	* GetHealth: Get the current heath of the enemy object
	* @author: Jc Fowles
	* @return: float: The current heath of th enemy object
	********************/
	float GetHealth() const { return m_fHealth; }

	/***********************
	* GetDamage: Get the Damage dealt by the enemy object
	* @author: Jc Fowles
	* @return: float: The Damage dealt by the enemy object
	********************/
	float GetDamage() const { return m_fDamage; }

	/***********************
	* GetEnemyType: Get the enum type of the enemy object
	* @author: Jc Fowles
	* @return: eEnemyTypes: The enum type of the enemy object
	********************/
	eEnemyTypes GetEnemyType() const { return m_eEnemyType; }

	//Setters
		
	/***********************
	* SetMaxHealth: Set the max heath of the enemy object
	* @author: Jc Fowles
	* @Parameter: float _MaxHealth: value to set max health to
	* @return: void: 
	********************/
	void SetMaxHealth(float _MaxHealth) { m_fMaxHealth = _MaxHealth; }

	/***********************
	* SetHealth: Set the current heath of the enemy object
	* @author: Jc Fowles
	* @Parameter: float _Health: value to set current health to
	* @return: void:
	********************/
	void SetHealth(float _Health) { m_fHealth = _Health; }

	/***********************
	* SetDamage: Set the Damage dealt by the enemy object
	* @author: Jc Fowles
	* @Parameter: float _Damage: value to set damage dealt to
	* @return: void: 
	********************/
	void SetDamage(float _Damage) { m_fDamage = _Damage; }

	//member variables
private:

	float m_fMaxHealth;
	float m_fHealth;
	float m_fDamage;

	eEnemyTypes m_eEnemyType;
	
};

#endif //__ENEMY_OBJ_H__