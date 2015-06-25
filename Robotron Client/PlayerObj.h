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
#include "C3DObject.h"

class CPlayerObj :
	public C3DObject
{
public:
	
	/***********************
	* C3DObject: Default Constructor for Player object class
	* @author: Jc Fowles
	* @parameter: IRenderer* _pRenderManager: RenderMangeer used to create the light
	* @return:
	********************/
	CPlayerObj(IRenderer* _pRenderManager);
		
	/***********************
	* C3DObject: Default destructor for Player object class
	* @author: Jc Fowles
	* @return:
	********************/
	~CPlayerObj();

	//Getter
	
	/***********************
	* GetLightID: Return the players light ID
	* @author: Jc Fowles
	* @return: UINT: Players Light ID
	********************/
	UINT GetLightID() const { return m_uiLightID; }

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
	
	/***********************
	* GetLightRange: Get the range of the point light
	* @author: Jc Fowles
	* @return: float: The range of the point light
	********************/
	float GetLightRange() const { return m_fLightRange; }

	/***********************
	* GetLifeCount: Get the Life count of the player
	* @author: Jc Fowles
	* @return: float: The Life count of the player
	********************/
	int GetLifeCount() const { return m_iLifeCount; }

	//Setter
	
	/***********************
	* SetLifeCount: Set the Life count
	* @author: Jc Fowles
	* @Parameter: int _LifeCount: value to set the Life count to 
	* @return: void:
	********************/
	void SetLifeCount(int _LifeCount) { m_iLifeCount = _LifeCount; }

	/***********************
	* SetLightRange: Set the range of the point light
	* @author: Jc Fowles
	* @Parameter: float _LightRange: value to set the range to 
	* @return: void: 
	********************/
	void SetLightRange(float _LightRange) { m_fLightRange = _LightRange; }

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
	int m_iLifeCount;
	
	UINT m_uiScore;
	

	UINT m_uiLightID;
	float m_fLightRange;
	
};

#endif //__PLAYER_OBJ_H__