/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : SteerLib.h
* Description : Library for steering functionality
* Author :	Jc Fowles
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

#pragma once

//Local Includes
#include "../Shared/App_Defines.h"
#include "../Shared/Network_Defines.h"


#ifndef __STEER_H__
#define __STEER_H__

inline void seek(EnemyStates* _EnemyStates)
{
	float3 f3Desired;
	f3Desired = _EnemyStates->f3Target - _EnemyStates->f3Positions;
	f3Desired = f3Desired.Normalise();
	f3Desired = f3Desired*_EnemyStates->fMaxSpeed;
	
	float3 f3Steer;
	f3Steer = f3Desired - _EnemyStates->f3Velocity;
	f3Steer.Limit(_EnemyStates->fMaxForce);
	f3Steer = f3Steer* 0.01667f; //DT;
	

	//Apply force
	_EnemyStates->f3Acceleration += f3Steer; //DT;

	/*_EnemyStates->f3Direction = _EnemyStates->f3Target - _EnemyStates->f3Positions;
	_EnemyStates->f3Direction =  _EnemyStates->f3Direction.Normalise();
	_EnemyStates->f3Velocity =   _EnemyStates->f3Direction * _EnemyStates->fMaxSpeed;


	_EnemyStates->f3Positions += _EnemyStates->f3Velocity;*/
	
};





#endif //__STEER_H__
