/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ProjectileObj.cpp
* Description : Implementation for the functions of the Projectile Object class
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Local Includes
#include "ProjectileObj.h"

CProjectileObj::CProjectileObj(std::string _strPlayerName, float3 _f3Position, float3 _f3Direction)
{
	m_strPlayerName = _strPlayerName;

	m_f3Position = _f3Position;
	m_f3Direction = _f3Direction;

	//TO DO:
	//Velocity, damage etc
}


CProjectileObj::~CProjectileObj()
{
}
