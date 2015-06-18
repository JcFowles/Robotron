/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : PlayerObj.h
* Description : Contains the header file for Player object Class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/
#pragma once

#ifndef __PLAYER_OBJ_H__
#define __PLAYER_OBJ_H__ 

//local includes
#include "..\Shared\C3DObject.h"

class CPlayerObj :
	public C3DObject
{
public:
	
	/***********************
	* C3DObject: Default Constructor for Player object class
	* @author: Jc Fowles
	* @return:
	********************/
	CPlayerObj();
		
	/***********************
	* C3DObject: Default destructor for Player object class
	* @author: Jc Fowles
	* @return:
	********************/
	~CPlayerObj();

	//Getter
	
	/***********************
	* GetScore: Return the players current score
	* @author: Jc Fowles
	* @return: UINT: Players current score
	********************/
	UINT GetScore() const { return m_uiScore; }

	/***********************
	* GetF3UpVector: Return the players Up direction vector
	* @author: Jc Fowles
	* @return: float3: The players Up direction vector
	********************/
	float3 GetUpVector() const { return m_f3UpVector; }

	/***********************
	* GetIsAlive: Return the living state of the player
	* @author: Jc Fowles
	* @return: bool: The living state of the player
	********************/
	bool GetIsAlive() const { return m_bIsAlive; }
	
	/***********************
	* GetHealth: Return the current health value of the player
	* @author: Jc Fowles
	* @return: UINT: The current health value of the player
	********************/
	UINT GetHealth() const { return m_uiHealth; }
	
	//Setter
		
	/***********************
	* SetScore: Set the players score value
	* @author: Jc Fowles
	* @Parameter: UINT _uiScore: value of score to be set
	* @return: void: 
	********************/
	void SetScore(UINT _uiScore) { m_uiScore = _uiScore; }

	/***********************
	* SetHealth: Set the players current health value
	* @author: Jc Fowles
	* @Parameter: UINT _Health: value of current health to be set
	* @return: void:
	********************/
	void SetHealth(UINT _Health) { m_uiHealth = _Health; }

	/***********************
	* SetIsAlive: Set the living state of the player
	* @author: Jc Fowles
	* @Parameter: bool _IsAlive: value of the living state to be set
	* @return: void:
	********************/
	void SetIsAlive(bool _IsAlive) { m_bIsAlive = _IsAlive; }

	//member variables
private:

	float3 m_f3UpVector;
		
	bool m_bIsAlive;
	
	UINT m_uiHealth;
	UINT m_uiMaxHealth;
	UINT m_uiScore;
};

#endif //__PLAYER_OBJ_H__