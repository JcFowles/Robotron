//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CMySemaphore.h
// Description : Contains the header file for Semaphore Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CMYSEMAPHORE_H__
#define __CMYSEMAPHORE_H__

//Library include
#include <condition_variable>
#include <mutex>


class CMySemaphore
{
public:
	//Member Functions

	/***********************
	* CMySemaphore: Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CMySemaphore();

	/***********************
	* CMySemaphore: overloaded Constructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	CMySemaphore(int _iValue);

	/***********************
	* ~CMySemaphore: Destructor of the Game instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CMySemaphore();

	//Prototypes

	/***********************
	* Wait: Atomic operation that waits for the semaphore value to be positive and
			then decrements it by one.
	* @author: Jc Fowles
	* @author: Asma Shakil
	* @return: void
	********************/
	void Wait();

	/***********************
	* Signal: Atomic operation that increments the semaphore value by one.
			  Which when positive, wakes up a waiting Semaphore
	* @author: Jc Fowles
	* @author: Asma Shakil
	* @return: void
	********************/
	void Signal();

	// Member variables
private:
	
	int m_iValue;
	std::condition_variable m_Condition; 
	std::mutex m_Mutex;

};

#endif //__CMYSEMAPHORE_H__
