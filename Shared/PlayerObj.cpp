/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : PlayerObj.cpp
* Description : Implementation for the functions of the Player Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "PlayerObj.h"


CPlayerObj::CPlayerObj()
{
	m_f3UpVector = { 0.0f, 1.0f, 0.0f };
		
	m_bIsAlive = true;

	m_uiMaxHealth = 100;
	m_uiHealth = m_uiMaxHealth;
	m_uiScore = 0;
	
}

CPlayerObj::~CPlayerObj()
{
}
