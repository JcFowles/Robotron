//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CClock.h
// Description : Header file containing declarations for CClock Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CCLOCK_H__
#define __CCLOCK_H__

// Library Includes
#include <windows.h>

class CClock
{
	//Member Function
public:
	/***********************
	* CClock: Constructor of the Clock class
	* @author: Jc Fowles
	* @return:
	********************/
	CClock();

	/***********************
	* CClock: Destructor of the clock class
	* @author: Jc Fowles
	* @return:
	********************/
	~CClock();

	/***********************
	* Initialise: Initializes the clock class
	* @author: Jc Fowles
	* @return: bool: return success true
	********************/
	bool Initialise();

	/***********************
	* Process: Process the clock class, Ie procces the Delta tick
	* @author: Jc Fowles
	* @return: void
	********************/
	void Process();

	/***********************
	* GetDeltaTick: Return the delta tick. Time taken for one frame in seconds
	* @author: Jc Fowles
	* @return: float: Return delta tick. Time taken for a single frame in seconds
	********************/
	float GetDeltaTick();

protected:

private:
	/***********************
	* CClock: Copy Constructor of the Clock class
	* @author: Jc Fowles
	* @return:
	********************/
	CClock(const CClock& _kr);
	
	CClock& operator= (const CClock& _kr);
	
	// Member Variables
public:

protected:
	
	float m_fTimeElapsed;
	float m_fDeltaTime;
	float m_fLastTime;
	float m_fCurrentTime;

private:
};

#endif //__CCLOCK_H__