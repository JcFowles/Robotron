/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : EnemyObj.cpp
* Description : Implementation for the functions of the Enemy Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "PowerUpObj.h"


CPowerUpObj::CPowerUpObj(ePowerType _eType)
{
	switch (_eType)
	{
	case PU_TEN:
	{
		m_uiPoints = 10;
	}
	break;
	case PU_FIFTY:
	{
		m_uiPoints = 50;
	}
	break;
	case PU_HUNDRED:
	{
		m_uiPoints = 100;
	}
	break;
	default:
		break;
	}
	
}

CPowerUpObj::~CPowerUpObj()
{
}
