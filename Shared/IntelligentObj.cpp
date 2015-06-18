/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : IntelegentObj.cpp
* Description : Implementation for the functions of the Player Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "IntelligentObj.h"


CIntelligentObj::CIntelligentObj()
{
	m_uiPoints = 0;
	m_fMaxForce = 0.0f;

	m_f3Acceleration = { 0.0f, 0.0f, 0.0f };
	m_f3DesiredVelocity = { 0.0f, 0.0f, 0.0f };

}

CIntelligentObj::~CIntelligentObj()
{
}
