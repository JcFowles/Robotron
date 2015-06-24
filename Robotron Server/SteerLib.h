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

//TO DO: comment headers

#ifndef __STEER_H__
#define __STEER_H__


/***********************
* ApplyForce: Apply the force 
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter:  float3* _pf3Position: Enemy position of the Object
* @parameter:  float3* _pf3Position: Enemy dirtection of the Object
* @parameter:  float fDt: Current Delta tick
* @return: void:
********************/
inline void ApplyForce(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float3 _f3Force, float fDt)
{

	//Apply steer force acceleration
	_f3Force = _f3Force.Limit(_SteeringInfo->fMaxForce*fDt);
	_SteeringInfo->f3Acceleration += _f3Force;
	//_SteeringInfo->f3Acceleration = _SteeringInfo->f3Acceleration.Limit(_SteeringInfo->fMaxAccel* fDt);


};

/***********************
* Update: Updates objects movement
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter:  float3* _pf3Position: Enemy position of the Object
* @parameter:  float3* _pf3Position: Enemy dirtection of the Object
* @parameter:  float fDt: Current Delta tick
* @return: float3: Force vector
********************/
inline void Update(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float fDt)
{
	if (fDt != 0.0f)
	{
		//Update enemy position based on acceleration and velocity
		_SteeringInfo->f3Velocity += _SteeringInfo->f3Acceleration;
		//_SteeringInfo->f3Velocity = _SteeringInfo->f3Velocity  * fDt*_SteeringInfo->fMaxSpeed;
		_SteeringInfo->f3Velocity = _SteeringInfo->f3Velocity.Limit(_SteeringInfo->fMaxSpeed * fDt);
		_SteeringInfo->f3Acceleration = _SteeringInfo->f3Acceleration * 0.0f;

		*_pf3Position += _SteeringInfo->f3Velocity;
		*_pf3Direction = _SteeringInfo->f3Velocity;
		*_pf3Direction = (_pf3Direction->Normalise());
	}

};

/***********************
* Flee: Flee steering for AI
* @author: Jc Fowles
* @parameter: _steeringInfo: Steering Info structure of the Object
* @parameter: v3Pos: Enemy Info structure of the Object
* @parameter: v3Dir: Enemy Info structure of the Object
* @parameter: _fDT: The current Delta Tick
* @return: void
********************/
inline void Flee(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float fDt)
{
	if (fDt != 0.0f)
	{
		//Calculate the Desired Velocity
		float3 f3Desired = (_SteeringInfo->f3TargetPosition - *_pf3Position) * -1.0f;
		f3Desired = f3Desired.Normalise();
		f3Desired = f3Desired * _SteeringInfo->fMaxSpeed;

		//Calculate the steering force towards the desired velocity
		float3 f3Steer = f3Desired - _SteeringInfo->f3Velocity.Normalise();
		f3Steer = f3Steer.Limit(_SteeringInfo->fMaxForce);
		
		//Apply the steer force
		ApplyForce(_SteeringInfo, _pf3Position, _pf3Direction, f3Steer, fDt);

		Update(_SteeringInfo, _pf3Position, _pf3Direction, fDt);

		
	}
};

/***********************
* Evade: Evade steering for AI
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter: float3* _pf3Position: Enemy Info structure of the Object
* @parameter: float3* _pf3Direction: Enemy Info structure of the Object
* @parameter: float fDt: The current Delta Tick
* @return: void
********************/
inline void Evade(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float fDt)
{
	//Calculate the targets future position based on delta tick if they continued on the same path
	float fDistance = (_SteeringInfo->f3TargetPosition - *_pf3Position).Magnitude();
	float fFuture = fDistance * _SteeringInfo->fTargetSpeed * fDt;
	float3 f3TargetFuturePos = _SteeringInfo->f3TargetPosition + (_SteeringInfo->f3TargetVelocity.Normalise() * fFuture);

	// Set the Future Position as the target and flee from it
	_SteeringInfo->f3TargetPosition = f3TargetFuturePos;
	Flee(_SteeringInfo, _pf3Position, _pf3Direction, fDt);
};

/***********************
* Seek: Seek steering for AI
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter:  float3* _pf3Position: Enemy position of the Object
* @parameter:  float3* _pf3Position: Enemy dirtection of the Object
* @parameter:  float fDt: Current Delta tick
* @return: float3: Force vector
********************/
inline float3 Seek(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float fDt)
{
	float3 Desired = _SteeringInfo->f3TargetPosition - (*_pf3Position);
	Desired = Desired.Normalise();
	Desired = Desired * _SteeringInfo->fMaxSpeed;

	float3 steer = Desired - _SteeringInfo->f3Velocity.Normalise();
	steer = steer.Limit(_SteeringInfo->fMaxForce);

	return steer;
};
 
/***********************
* Wander: Wandering steering for AI
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter:  float3* _pf3Position: Enemy position of the Object
* @parameter:  float3* _pf3Position: Enemy dirtection of the Object
* @parameter:  float fDt: Current Delta tick
* @return: bool: True if the object is contained, false if it is not
********************/
inline void Wander(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, float fDt)
{
	//Don't calculate if delta tick is 0
	if (fDt != 0)
	{
		float3 f3CirclePos = *_pf3Position + ((_pf3Direction->Normalise()) * ((_SteeringInfo->fSize) * 2));
		float fCircleRadius = (_SteeringInfo->fSize);

		//Transform angle to relative axis instead of world axis
		float fRotation = -(atan2(_pf3Direction->z, _pf3Direction->x) - atan2(1.0f, 0.0f));

		//Calculate a random degree of movement
		int iRandChange = rand() % 20;
		//Convert to Radians
		float fAngleChange = DegreesToRadians((float)iRandChange);
		//Calculate weather to add or subtract the degree of movement
		int iRand = rand() % 2;
		(iRand == 1) ? _SteeringInfo->fWanderAngle += fAngleChange : _SteeringInfo->fWanderAngle -= fAngleChange;

		//Add the Wander angle to the relative axis
		fRotation += _SteeringInfo->fWanderAngle;

		//Calculate point on the circle
		float3 f3Displacment;
		f3Displacment.x = (fCircleRadius * (float)sin(fRotation));
		f3Displacment.y = 0;
		f3Displacment.z = (fCircleRadius * (float)cos(fRotation));
		//Translate point
		f3Displacment += f3CirclePos;
				
		//Calculate desired
		float3 f3Desired;
		f3Desired = f3Displacment - *_pf3Position;
		f3Desired = f3Desired.Normalise();
			
		//Calculate Steer
		float3 f3Steer;
		f3Steer = f3Desired - _SteeringInfo->f3Velocity.Normalise();
		f3Steer = f3Steer.Limit(_SteeringInfo->fMaxForce* fDt);
				
		//return f3Steer;
		
		//Apply steer force acceleration
		_SteeringInfo->f3Acceleration += f3Steer;
		_SteeringInfo->f3Acceleration = _SteeringInfo->f3Acceleration.Limit(_SteeringInfo->fMaxAccel* fDt);
		
		//Update enemy position based on acceleration and velocity
		_SteeringInfo->f3Velocity += _SteeringInfo->f3Acceleration;
		_SteeringInfo->f3Velocity = _SteeringInfo->f3Velocity.Limit(_SteeringInfo->fMaxSpeed * fDt);
		_SteeringInfo->f3Acceleration = _SteeringInfo->f3Acceleration * 0.0f;
		
		//Apply Seek to direction and position
		*_pf3Position += _SteeringInfo->f3Velocity;
		*_pf3Direction = _SteeringInfo->f3Velocity;
		*_pf3Direction = (_pf3Direction->Normalise());


	}
};

/***********************
* Contain: Containment steering for AI
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter:  float3* _pf3Position: Enemy Info structure of the Object
* @parameter: float2 _f2ContainmentField: Containment field of the object
* @return: bool: True if the object is contained, false if it is not
********************/
inline bool Contain(SteeringStates* _SteeringInfo, float3* _pf3Position, float2 _f2ContainmentField)
{
	// Calculate a smaller containment field than the terrain requires
	float fMinX = -(_f2ContainmentField.u / 2.0f) + 5;
	float fMinZ = -(_f2ContainmentField.v / 2.0f) + 5;
	float fMaxX =  (_f2ContainmentField.u / 2.0f) - 5;
	float fMaxZ =  (_f2ContainmentField.v / 2.0f) - 5;
		
	if ( (_pf3Position->x < fMinX) ||
		 (_pf3Position->x > fMaxX) ||
		 (_pf3Position->z < fMinZ) ||
		 (_pf3Position->z > fMaxZ) )
	{
		_SteeringInfo->f3TargetPosition = { 0.0f, 1.0f, 0.0f };
		return false;
	}

	return true;
};

/***********************
* Separate: Separate the flock individuals from the rest of the flock
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter: float3* _pf3Position: Current Objects Position
* @parameter:  float3* _pf3Direction: Current Objects Direction
* @parameter: std::map<UINT, EnemyStates>* _pEnemies: The map of all enemies currently in play
* @parameter: float fDT: The current Delta Tick
* @return: float3: Force vector
********************/
inline float3 Separate(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, std::map<UINT, EnemyStates>* _pEnemies, float fDT)
{
	//Calculate the Desired Separation value based on the size of the Object
	float fDesiredSeparation = (_SteeringInfo->fSize * 2.0f) * 1.4f; 

	//Initial Variable values
	float3 f3SumOfSurroundings = { 0.0f, 0.0f, 0.0f };
	int iObjectCount = 0;

	//Cycle through the map of all enemies
	std::map<UINT, EnemyStates>::iterator iterEnemy = _pEnemies->begin();
	while (iterEnemy != _pEnemies->end())
	{
		//check only for demon type enemies
		if (iterEnemy->second.Etype == ET_LUST)
		{
			//Calculate the distance between the current enemy and the calling object
			float3 f3PosDifference = ((*_pf3Position) - iterEnemy->second.f3Positions);
			float fDistance = f3PosDifference.Magnitude();

			//If the distance is less than the desired separation
			if ((fDistance > 0) && (fDistance < fDesiredSeparation))
			{
				//Apply Force to separate individuals
				float3 f3Difference = f3PosDifference;
				f3Difference.Normalise();
				f3Difference = f3Difference / fDistance;

				//Sum all individual forces that are within the desired separation circle
				f3SumOfSurroundings += f3Difference;
				iObjectCount++;
			}
		}
		iterEnemy++;
	}
	//Calculate only if other objects were in separation circle
	if (iObjectCount > 0)
	{
		// Calculate the average of all forces
		f3SumOfSurroundings = f3SumOfSurroundings / (float)iObjectCount;
		f3SumOfSurroundings.Normalise();
		f3SumOfSurroundings = f3SumOfSurroundings * _SteeringInfo->fMaxSpeed;// *fDT);

		// Calculate the steering vector
		float3 f3Steer = f3SumOfSurroundings - _SteeringInfo->f3Velocity.Normalise();
		f3Steer.Limit(_SteeringInfo->fMaxForce);
		
		return f3Steer;
	}
	else
	{
		// Seek the Closest Avatar
		//return (_SteeringInfo->f3TargetPosition - (*_pf3Position));
		return float3(0.0f, 0.0f, 0.0f);
	}
}

/***********************
* Align: Align the flock individuals with the rest of the flock
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter: float3* _pf3Position: Current Objects Position
* @parameter:  float3* _pf3Direction: Current Objects Direction
* @parameter: std::map<UINT, EnemyStates>* _pEnemies: The map of all enemies currently in play
* @parameter: float fDT: The current Delta Tick
* @return: float3: Force vector
********************/
inline float3 Align(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, std::map<UINT, EnemyStates>* _pEnemies, float fDT)
{
	//Calculate the Desired Neighbor distance value based on the size of the Object
	float fNeighborDistance = (_SteeringInfo->fSize * 4.0f) * 1.4f;

	float3 f3SumOfSurroundings = { 0.0f, 0.0f, 0.0f };
	
	int iObjectCount = 0;


	//Cycle through the map of all enemies
	std::map<UINT, EnemyStates>::iterator iterEnemy = _pEnemies->begin();
	while (iterEnemy != _pEnemies->end())
	{
		//check only for demon type enemies
		if (iterEnemy->second.Etype == ET_LUST)
		{
			//Calculate the distance between the current enemy and the calling object
			float3 f3PosDifference = ((*_pf3Position) - iterEnemy->second.f3Positions);
			float fDistance = f3PosDifference.Magnitude();

			//If the distance is less than the desired separation
			if ((fDistance > 0) && (fDistance < fNeighborDistance))
			{
				
				f3SumOfSurroundings += iterEnemy->second.SteeringInfo.f3Velocity;
				
				iObjectCount++;
			}
		}
		iterEnemy++;
	}
	//Calculate only if other objects were in alignment circle
	if (iObjectCount > 0)
	{
		// Calculate the average of all forces
		f3SumOfSurroundings = f3SumOfSurroundings / (float)iObjectCount;
		f3SumOfSurroundings.Normalise();
		f3SumOfSurroundings = f3SumOfSurroundings * _SteeringInfo->fMaxSpeed;// *fDT);

		// Calculate the steering vector
		float3 f3Steer = f3SumOfSurroundings - _SteeringInfo->f3Velocity.Normalise();
		f3Steer.Limit(_SteeringInfo->fMaxForce);

		return f3Steer;
	}
	else
	{
		//Seek the Closest Avatar
		//return (_SteeringInfo->f3TargetPosition - (*_pf3Position));
		return float3(0.0f, 0.0f, 0.0f);
	}

}

/***********************
* Cohesion: try achieve Cohesion with the flock individuals
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter: float3* _pf3Position: Current Objects Position
* @parameter:  float3* _pf3Direction: Current Objects Direction
* @parameter: std::map<UINT, EnemyStates>* _pEnemies: The map of all enemies currently in play
* @parameter: float fDT: The current Delta Tick
* @return: float3: Force vector
********************/
inline float3 Cohesion(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, std::map<UINT, EnemyStates>* _pEnemies, float fDT)
{
	float fNeighborDistance = (_SteeringInfo->fSize * 5.0f) * 1.4f;
	float3 f3SumOfSurroundings = { 0.0f, 0.0f, 0.0f };

	int iObjectCount = 0;
	
	//Cycle through the map of all enemies
	std::map<UINT, EnemyStates>::iterator iterEnemy = _pEnemies->begin();
	while (iterEnemy != _pEnemies->end())
	{
		//check only for demon type enemies
		if (iterEnemy->second.Etype == ET_LUST)
		{
			//Calculate the distance between the current enemy and the calling object
			float3 f3PosDifference = ((*_pf3Position) - iterEnemy->second.f3Positions);
			float fDistance = f3PosDifference.Magnitude();

			//If the distance is less than the desired separation
			if ((fDistance > 0) && (fDistance < fNeighborDistance))
			{
				f3SumOfSurroundings += iterEnemy->second.f3Positions;
				iObjectCount++;
			}
		}
		iterEnemy++;
	}
	if (iObjectCount > 0)
	{
		// Calculate the average of all forces
		f3SumOfSurroundings = f3SumOfSurroundings / (float)iObjectCount;
		f3SumOfSurroundings.Normalise();
		
		SteeringStates newSteering = *_SteeringInfo;
		newSteering.f3TargetPosition = f3SumOfSurroundings;
		newSteering.f3TargetPosition.y = 1.0f;

		float3 f3Steer = Seek(&newSteering, _pf3Position, _pf3Direction, fDT);

		return f3Steer;
	}
	else
	{
		return float3(0.0f, 0.0f, 0.0f);
	}
}

/***********************
* flocking: Flocking AI for autonomous Enemies
* @author: Jc Fowles
* @parameter: SteeringStates* _SteeringInfo: Steering Info structure of the Object
* @parameter: float3* _pf3Position: Current Objects Position
* @parameter:  float3* _pf3Direction: Current Objects Direction
* @parameter: std::map<UINT, EnemyStates>* _pEnemies: The map of all enemies currently in play
* @parameter: float fDT: The current Delta Tick
* @return: void:
********************/
inline void flocking(SteeringStates* _SteeringInfo, float3* _pf3Position, float3* _pf3Direction, std::map<UINT, EnemyStates>* _pEnemies, float fDT)
{
	if (fDT > 0)
	{
		float3 f3Separation = Separate(_SteeringInfo, _pf3Position, _pf3Direction, _pEnemies, fDT);
		float3 f3Align = Align(_SteeringInfo, _pf3Position, _pf3Direction, _pEnemies, fDT);
		float3 f3Coh = Cohesion(_SteeringInfo, _pf3Position, _pf3Direction, _pEnemies, fDT);
		float3 seek = Seek(_SteeringInfo, _pf3Position, _pf3Direction, fDT);

		f3Separation = f3Separation*(1.5f);
		f3Align = f3Align*(1.0f);
		f3Coh = f3Coh*(1.0f);
		seek = seek *(2.5f);

		ApplyForce(_SteeringInfo, _pf3Position, _pf3Direction, f3Separation, fDT);
		ApplyForce(_SteeringInfo, _pf3Position, _pf3Direction, f3Align, fDT);
		ApplyForce(_SteeringInfo, _pf3Position, _pf3Direction, f3Coh, fDT);
		ApplyForce(_SteeringInfo, _pf3Position, _pf3Direction, seek, fDT);

		Update(_SteeringInfo, _pf3Position, _pf3Direction, fDT);

		//return seek;
	}
	

}

#endif //__STEER_H__
