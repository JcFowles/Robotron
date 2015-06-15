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

	//Creation/Deletion
	CMySemaphore();
	CMySemaphore(int _iValue);
	~CMySemaphore();

	//Functionality
	void Wait();
	void Signal();

private:
	// Member variables
	int m_iValue;
	std::condition_variable m_Condition; 
	std::mutex m_Mutex;

};

#endif //__CMYSEMAPHORE_H__
