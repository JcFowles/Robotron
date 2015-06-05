//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CMySemaphore.cpp
// Description : Contains the main funtionality of the CMySemaphore Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

//This include
#include "MySemaphore.h"

/***********************
* CMySemaphore: Constructor of the Game instance
* @author: Jc Fowles
* @return:
********************/
CMySemaphore::CMySemaphore()
{
	m_iValue = 0;
}

/***********************
* CMySemaphore: overloaded Constructor of the Game instance
* @author: Jc Fowles
* @return:
********************/
CMySemaphore::CMySemaphore(int _iValue)
{
	m_iValue = _iValue;
}

/***********************
* ~CMySemaphore: Destructor of the Game instance
* @author: Jc Fowles
* @return:
********************/
CMySemaphore::~CMySemaphore()
{

}

/***********************
* Wait: Atomic operation that waits for the semaphore value to be positive and 
		then decrements it by one.
* @author: Jc Fowles
* @author: Asma Shakil
* @return: void
********************/
void CMySemaphore::Wait()
{
	//Lock the mutex, as m_iValue is a critical section, with a auto unlock
	std::unique_lock<std::mutex> _unqlckguard(m_Mutex);

	m_Condition.wait(_unqlckguard, [this](){return (m_iValue > 0); });
		
	m_iValue--;
}

/***********************
* Signal: Atomic operation that increments the semaphore value by one.
		  Which when positive, wakes up a waiting Semaphore
* @author: Jc Fowles
* @author: Asma Shakil
* @return: void
********************/
void CMySemaphore::Signal()
{
	std::lock_guard<std::mutex> _lckguard(m_Mutex);
	m_iValue++;
	m_Condition.notify_one();
}
