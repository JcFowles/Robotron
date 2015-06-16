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
#include "EnemyObj.h"


CEnemyObj::CEnemyObj()
{
}

CEnemyObj::CEnemyObj(eEnemyTypes _eEnemyType)
{

	m_eEnemyType = _eEnemyType;
	switch (_eEnemyType)
	{
	case ET_LUST:
	{
		m_fSize = 0.5f;
		m_fMaxSpeed = 10.0f;
		m_fDamage = 2.0f;
		m_uiPoints = 10;

		m_fMaxHealth = 50.0f;
		m_fHealth = m_fMaxHealth;

		//Everything else
	}
		break;
	case ET_PRIDE:
		break;
	case ET_WRATH:
		break;
	case ET_GREED:
		break;
	case ET_SLOTH:
		break;
	case ET_ENVY:
		break;
	case ET_GLUTTONY:
		break;
	default:
		break;
	}
}

CEnemyObj::~CEnemyObj()
{
}
